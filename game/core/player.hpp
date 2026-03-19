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
  float deathBarrierY = 512.0f; //default, but can be changed for different levels
  glm::vec2 lastSafePosition{0.0f, 0.0f};
  glm::vec2 checkpoint{0.0f, 0.0f};
  float invincibilityTime = 0.0f;
  int maxHealth = 20;
  int health = maxHealth;
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
  
  void dealDamage(int damage = 4);
  
  void drainHealth();
  
  void deathAnimation();

  void update(Level& level, float delta);
  
  void draw(Renderer* renderer, glm::vec2 camera, float second);
};