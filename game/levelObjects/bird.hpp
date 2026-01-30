#pragma once

#include "rect.hpp"
#include "sprite.hpp"
#include "glm/vec2.hpp"
#include "entity.hpp"

class Engine;

enum class BirdState {
  STATIC,
  SWOOPLOW,
  SWOOPHIGH,
  RECOVER,
  FOLLOW,
  DEAD
};

class Bird : public Entity {
  Engine* engine = nullptr;
  float swoopX = 0.0f;
  glm::vec2 swoopStartPos{0,0};
  glm::vec2 swoopTargetPos{0,0};
  float swoopSpeed = 0.01f;
  float swoopProgress = 0.0f;
public:
  BirdState state = BirdState::STATIC;
  int health = 1;
  fRect hitbox {0.0f, 0.0f, 8.0f, 8.0f};
  Sprite sprite;
  int active = false;
  
  Bird();
  Bird(const Bird& bird);
  Bird& operator=(const Bird& other);
  Bird(Engine* engine, Sprite sprite, glm::vec2 position);
  
  void init(Sprite sprite, glm::vec2 position);
  void update();
  void draw(SDL_Renderer* renderer, glm::vec2 camera);
};