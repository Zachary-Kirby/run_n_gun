#include "bulletfrog.hpp"

BulletFrog::BulletFrog()
{
  active = false;
  health = 1;
}

BulletFrog::BulletFrog(const BulletFrog &bulletFrog)
{
  engine = bulletFrog.engine;
  active = bulletFrog.active;
  hitbox = bulletFrog.hitbox;
  health = bulletFrog.health;
  sprite = Sprite(bulletFrog.sprite);
}

BulletFrog &BulletFrog::operator=(const BulletFrog &other)
{
  this->engine = other.engine;
  active = other.active;
  this->sprite = other.sprite;
  this->hitbox = other.hitbox;
  health = other.health;
  return *this;
}

BulletFrog::BulletFrog(Engine *engine, Sprite sprite, glm::vec2 position)
{
  this->engine = engine;
  active = true;
  this->sprite.init(sprite);
  hitbox.x = position.x;
  hitbox.y = position.y;
}

void BulletFrog::init(Sprite sprite, glm::vec2 position)
{
  health = 1;
  hitbox = fRect(position.x, position.y, 8.0f, 8.0f);
  this->sprite = Sprite(sprite);
  active = true;
}

void BulletFrog::update(float delta)
{
  //BulletFrog will have two things it does. It will shoot bullets until the player is close, then it will resort
  //to jumping toward the player. It will have a timer for shooting. In total there would be two states for animation.
}