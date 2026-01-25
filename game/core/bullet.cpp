#include "bullet.hpp"

//TODO this is mostly duplicate code
void Bullet::init(Sprite sprite, glm::vec2 position, glm::vec2 velocity)
{
  this->velocity = velocity;
  this->sprite = sprite;
  this->hitbox.x = position.x;
  this->hitbox.y = position.y;
  active = true;
  lifetime = 120;
}

void Bullet::update()
{
  hitbox.x += velocity.x;
  hitbox.y += velocity.y;
  lifetime--;
  if (lifetime <= 0){
    active = false;
  }
}

//TODO this is duplicate code
void Bullet::draw(SDL_Renderer *renderer)
{
  sprite.setPostion(hitbox.x, hitbox.y);
  sprite.draw(renderer);
}
