#pragma once

#include <glm/glm.hpp>
#include "sprite.hpp"
#include "rect.hpp"

class Renderer;
class Level;

class Player{
  glm::ivec2 spriteOffset;
  public:
  bool grounded = false;
  Sprite sprite;
  float controlStickX = 0.0f;
  float controlStickY = 0.0f;
  float deadzone = 0.1f;
  fRect hitbox;
  glm::vec2 velocity;
  bool facingLeft = false;
  
  Player();
  
  void init(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);
  
  Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);

  void jump();

  void jumpLetGo();

  void update(Level& level, float delta);
  
  void draw(Renderer* renderer, glm::vec2 camera, float second);
};