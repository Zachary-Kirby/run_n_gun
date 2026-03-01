#include <iostream>
#include <algorithm>
#include <cmath>
#include "sprite.hpp"
#include "rect.hpp"
#include "level.hpp"
#include "renderer.hpp"
#include "player.hpp"


Player::Player()
{
  hitbox = {0, 0, 16, 16};
  spriteOffset = glm::ivec2(0, 0);
  velocity = {0, 0};
}

void Player::init(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset)
{
  hitbox.x = pos.x; hitbox.y = pos.y;
  hitbox.w = hitboxWidth;
  hitbox.h = hitboxHeight;
  sprite.init(isprite);
  this->spriteOffset = spriteOffset;
}

Player::Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset)
{
  sprite = isprite;
  hitbox.x = pos.x; hitbox.y = pos.y;
  hitbox.w = hitboxWidth;
  hitbox.h = hitboxHeight;
  
  
  velocity = glm::vec2(0.0f);
}

void Player::jump()
{
  if (grounded){
    grounded = false;
    velocity.y = -5.0f*60.0f;
  }
}

void Player::jumpLetGo()
{
  if (velocity.y < 0.0f)
  {
    velocity.y *= 0.5f;
  }
}

void Player::update(Level &level, float delta)
{
  int collisionMask = 0b00000;
  if (controlStickY > 0.0f) collisionMask = 0b00010;
  for (int i=0; i<2; i++)
  {
    MoveCollision collision = move(hitbox, velocity*delta/2.0f, level, collisionMask);
    hitbox = collision.position;
    if (collision.cancelVec.x != 0) velocity.x = 0;
    if (collision.cancelVec.y != 0) velocity.y = 0;
    if (collision.cancelVec.y > 0) grounded = true;
    if (velocity.y > 0.0f) grounded = false;
  }
  if (velocity.x < 0.0f) facingLeft = true;
  if (velocity.x > 0.0f) facingLeft = false;
  if (std::abs(controlStickX) < deadzone)
  {
    if (std::abs(velocity.x*delta) >= 100.0f)
    {
      velocity.x = velocity.x - std::copysignf(0.4f*5000.0f, velocity.x) * delta;
    }
    else
    {
      velocity.x = 0;
    }
  }
  else
  {
    
  //keep the player from accelerating more than a certain amount while not capping velocity from other sources
  float cap = 0.7f*0.75f*250.0f;
  if (controlStickX > 0.0f && velocity.x < cap){velocity.x = std::min(velocity.x+controlStickX*1600.0f*delta, cap);}
  if (controlStickX < 0.0f && velocity.x > -cap){velocity.x = std::max(velocity.x+controlStickX*1600.0f*delta, -cap);}
  
  //velocity.x += controlStickX * 0.02f;
    
  }
  
  velocity += glm::vec2(0.0f, 800.0f) * delta;
  
  
  
}

void Player::draw(Renderer *renderer, glm::vec2 camera, float second)
{
    if (!grounded)
    {
      sprite.setDefinition(renderer->spriteAnimations.getFrame("Player_Air", 0, facingLeft, false));
    }
    else if (std::abs(velocity.x) > 0.1f)
    {
      sprite.setDefinition(renderer->spriteAnimations.getAnimationFrame("Player_Run", second, facingLeft, false));
    }
    else
    {
      sprite.setDefinition(renderer->spriteAnimations.getFrame("Player_Idle", 0, facingLeft));
    }
  sprite.setPostion(hitbox.x+spriteOffset.x-camera.x, hitbox.y+spriteOffset.y-camera.y);
  sprite.draw(renderer);
}
