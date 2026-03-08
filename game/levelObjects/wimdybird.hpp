#pragma once

#include "sprite.hpp"
#include "rect.hpp"
#include "glm/vec2.hpp"

class Engine;
class Renderer;

class WimdyBird
{
  Engine* engine = nullptr;
  Sprite sprite;
  fRect hitbox;
  bool facingRight = false;
  int health = 3;
  bool flapping = false;
  float flapTimer = 0.0f;
  public:
  bool active = true;
  WimdyBird() = default;
  WimdyBird(Engine* engine, Sprite sprite, glm::vec2 pos);
  WimdyBird(const WimdyBird& other);
  void update(float delta);
  void draw(Renderer* renderer, glm::vec2 camera, float second);
};