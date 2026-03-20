#include "bullet.hpp"
#include "renderer.hpp"

//TODO this is mostly duplicate code
void Bullet::init(Sprite sprite, glm::vec2 position, glm::vec2 velocity)
{
  this->velocity = velocity;
  this->sprite = sprite;
  this->hitbox.x = position.x;
  this->hitbox.y = position.y;
  hitbox.w*=2;
  hitbox.h*=2;
  hitbox.x-=hitbox.w/2;
  hitbox.y-=hitbox.h/2;
  active = true;
  lifetime = 0.3f;
}

void Bullet::update(float delta)
{
  hitbox.x += velocity.x * delta;
  hitbox.y += velocity.y * delta;
  lifetime = lifetime - delta;
  if (lifetime <= 0){
    active = false;
  }
}

//TODO this is duplicate code
void Bullet::draw(Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x-camera.x, hitbox.y-camera.y);
  sprite.draw(renderer);
}
