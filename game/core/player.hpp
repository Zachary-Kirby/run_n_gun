#pragma once

#include <glm/glm.hpp>
#include "sprite.hpp"

class Player{
  Sprite sprite;
  glm::ivec2 spriteOffset;
  bool grounded = false;
  public:
  float controlStickX = 0.0f;
  float deadzone = 0.1f;
  glm::vec2 velocity;
  glm::vec2 position;
  fRect hitbox;
  
  Player();
  
  void init(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);
  
  Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);

  void jump();

  void jumpLetGo();

  void update(Level& level);
  
  void draw(SDL_Renderer* renderer, glm::vec2 camera);
};