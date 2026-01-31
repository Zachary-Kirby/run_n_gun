#include <iostream>
#include <algorithm>
#include <cmath>
#include "sprite.hpp"
#include "rect.hpp"
#include "level.hpp"

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
    velocity.y = -1.0f;
  }
}

void Player::jumpLetGo()
{
  if (velocity.y < 0.0f)
  {
    velocity.y *= 0.5f;
  }
}

void Player::update(Level &level)
{
  int collisionMask = 0b00000;
  if (controlStickY > 0.0f) collisionMask = 0b00010;
  MoveCollision collision = move(hitbox, velocity, level, collisionMask);
  hitbox = collision.position;
  if (collision.cancelVec.x != 0) velocity.x = 0;
  if (collision.cancelVec.y != 0) velocity.y = 0;
  if (collision.cancelVec.y > 0) grounded = true;
  if (velocity.y > 0.0f) grounded = false;
  if (std::abs(controlStickX) < deadzone)
  {
    if (std::abs(velocity.x) >= 0.11f)
    {
      velocity.x = velocity.x - std::copysignf(0.02f, velocity.x);
    }
    else
    {
      velocity.x = 0;
    }
  }
  else
  {
    
  //keep the player from accelerating more than a certain amount while not capping velocity from other sources
  float cap = 0.7f*0.75f;
  if (controlStickX > 0.0f && velocity.x < cap){velocity.x = std::min(velocity.x+controlStickX*0.04f, cap);}
  if (controlStickX < 0.0f && velocity.x > -cap){velocity.x = std::max(velocity.x+controlStickX*0.04f, -cap);}
  
  //velocity.x += controlStickX * 0.02f;
    
  }
  
  velocity += glm::vec2(0.0f, 0.01f);
  
  
  
}

void Player::draw(SDL_Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x+spriteOffset.x-camera.x, hitbox.y+spriteOffset.y-camera.y);
  sprite.draw(renderer);
}
