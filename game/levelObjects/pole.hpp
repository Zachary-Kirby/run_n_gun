#pragma once
#include "glm/vec2.hpp"

class Renderer;

class Pole {
public:
  float pivotX;
  float pivotY;
  float length;
  float angle = -3.141593f/2.0f;
  float angularVelocity = 0.0f;
  Pole(float pivotX, float pivotY, float length) : pivotX(pivotX), pivotY(pivotY), length(length) {}
  void update(float delta);
  void draw(Renderer* renderer, glm::vec2 camera);
};