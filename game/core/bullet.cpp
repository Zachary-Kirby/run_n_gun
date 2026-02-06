#include "bullet.hpp"
#include "renderer.hpp"

//TODO this is mostly duplicate code
void Bullet::init(Sprite sprite, glm::vec2 position, glm::vec2 velocity)
{
  this->velocity = velocity;
  this->sprite = sprite;
  this->hitbox.x = position.x;
  this->hitbox.y = position.y;
  active = true;
  lifetime = 2.0f;
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
