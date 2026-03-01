#pragma once

#include <glm/glm.hpp>
#include "sprite.hpp"
#include "rect.hpp"

class Engine;
class Renderer;
class Level;

class Player{
  glm::ivec2 spriteOffset;
  Engine* engine;
  public:
  int health = 40;
  int maxHealth = 40;
  bool grounded = false;
  float healthRegenTimer = 0.0f;
  Sprite sprite;
  float controlStickX = 0.0f;
  float controlStickY = 0.0f;
  float deadzone = 0.1f;
  fRect hitbox;
  glm::vec2 velocity;
  bool facingLeft = false;
  
  Player();
  
  void init(Engine* theEngine, glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);
  
  Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset);

  void jump();

  void jumpLetGo();
  
  void dealDamage();
  
  void drainHealth();
  
  void deathAnimation();

  void update(Level& level, float delta);
  
  void draw(Renderer* renderer, glm::vec2 camera, float second);
};