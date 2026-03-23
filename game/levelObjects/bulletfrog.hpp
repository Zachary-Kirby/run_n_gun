#pragma once

#include "rect.hpp"
#include "sprite.hpp"
#include "glm/vec2.hpp"


class Engine;
class Renderer;

class BulletFrog {
  Engine* engine = nullptr;
public:
  int health = 1;
  fRect hitbox {0.0f, 0.0f, 8.0f, 8.0f};
  Sprite sprite;
  int active = false;
  
  BulletFrog();
  BulletFrog(const BulletFrog& bulletFrog);
  BulletFrog& operator=(const BulletFrog& other);
  BulletFrog(Engine* engine, Sprite sprite, glm::vec2 position);
  
  void init(Sprite sprite, glm::vec2 position);
  void update(float delta);
  void draw(Renderer* renderer, glm::vec2 camera, float time);
};