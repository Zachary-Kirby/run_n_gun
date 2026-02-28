#pragma once
#include "glm/vec2.hpp"
#include "SDL2/SDL_rect.h"

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
  SDL_FRect src;
  Pole(Engine* mainEngine, SDL_FRect textureSrc, float pivotX, float pivotY, float length) : engine(mainEngine), src(textureSrc), pivotX(pivotX), pivotY(pivotY), length(length) {}
  void update(float delta);
  void draw(Renderer* renderer, glm::vec2 camera);
};