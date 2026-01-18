#include <iostream>
#include "glm/glm.hpp"
#include "sprite.hpp"
#include "rect.hpp"
#include "level.hpp"

#include "player.hpp"


Player::Player()
{
  position = {0, 0};
  hitbox = {0, 0, 16, 16};
  spriteOffset = glm::ivec2(0, 0);
  velocity = {0, 0};
}

void Player::init(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset)
{
  position = pos;
  hitbox.w = hitboxWidth;
  hitbox.h = hitboxHeight;
  sprite.init(isprite);
  this->spriteOffset = spriteOffset;
}

Player::Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset) : position(pos)
{
  sprite = isprite;
  hitbox.w = hitboxWidth;
  hitbox.h = hitboxHeight;
  
  
  velocity = glm::vec2(0.0f);
}

void Player::update(Level &level)
{

  MoveCollision collision = move(position, velocity, hitbox, level);
  position = collision.position;
  velocity += glm::vec2(0.0f, 0.01f);
  if (std::abs(velocity.x) >= 0.11f)
  {
    velocity.x = velocity.x - std::copysignf(0.1f, velocity.x);
  }
  else
  {
    velocity.x = 0;
  }
  
}

void Player::draw(SDL_Renderer *renderer)
{
  sprite.setPostion(position.x+spriteOffset.x, position.y+spriteOffset.y);
  sprite.draw(renderer);
}
