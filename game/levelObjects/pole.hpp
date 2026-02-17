#pragma once
#include "glm/vec2.hpp"

class Renderer;
class Engine;

class Pole {
  Engine* engine;
public:
  float pivotX;
  float pivotY;
  float length;
  float angle = -3.141593f/2.0f;
  float angularVelocity = 0.0f;
  Pole(Engine* mainEngine, float pivotX, float pivotY, float length) : engine(mainEngine), pivotX(pivotX), pivotY(pivotY), length(length) {}
  void update(float delta);
  void draw(Renderer* renderer, glm::vec2 camera);
};