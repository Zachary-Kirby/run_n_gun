#pragma once

#include "rect.hpp"
#include "sprite.hpp"
#include "glm/vec2.hpp"

class Engine;

class Bird {
  Engine* engine = nullptr;
public:
  int health = 1;
  fRect hitbox {0.0f, 0.0f, 8.0f, 8.0f};
  Sprite sprite;
  int active = false;
  
  Bird();
  Bird(const Bird& bird);
  Bird& operator=(const Bird& other);
  Bird(Engine* engine, Sprite sprite, glm::vec2 position);
  
  void init(Sprite sprite, glm::vec2 position);
  void update(glm::vec2 target);
  void draw(SDL_Renderer* renderer, glm::vec2 camera);
};