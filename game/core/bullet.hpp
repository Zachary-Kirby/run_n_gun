#pragma once

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include "sprite.hpp"
#include "rect.hpp"
#include "entity.hpp"

class Bullet : public Entity {
  Sprite sprite;
  int lifetime;
  public:
  glm::vec2 velocity;
  fRect hitbox;
  bool active = false;
  Bullet(Sprite sprite, glm::vec2 position, glm::vec2 velocity)
  {
    init(sprite, position, velocity);
  }
  Bullet(const Bullet& other)
  {
    this->sprite = other.sprite;
    this->hitbox = other.hitbox;
    this->velocity = other.velocity;
    this->lifetime = other.lifetime;
    this->active = other.active;
  }
  Bullet& operator=(const Bullet& other)
  {
    this->sprite = other.sprite;
    this->hitbox = other.hitbox;
    this->velocity = other.velocity;
    this->lifetime = other.lifetime;
    this->active = other.active;
    return *this;
  }
  void init(Sprite sprite, glm::vec2 position, glm::vec2 velocity);
  void update();
  void draw(SDL_Renderer* renderer, glm::vec2 camera);
};