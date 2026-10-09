#pragma once

#include "glm/vec2.hpp"

enum class BirdState {
  STATIC,
  SWOOPLOW,
  SWOOPHIGH,
  RECOVER,
  FOLLOW,
  DEAD
};

struct BirdAI{
  BirdState state;
  float swoopX = 0.0f;
  float swoopProgress = 0.0f;
  glm::vec2 swoopStartPos{0,0};
  glm::vec2 swoopTargetPos{0,0};
};