#pragma once

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include "sprite.hpp"
#include "rect.hpp"

class Bullet{
  Sprite sprite;
  fRect hitbox;
  glm::vec2 velocity;
  int lifetime;
  public:
  bool active = false;
  void init(Sprite sprite, glm::vec2 position, glm::vec2 velocity);
  void update();
  void draw(SDL_Renderer* renderer, glm::vec2 camera);
};