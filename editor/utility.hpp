#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

inline int clamp(int value, int min, int max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

inline bool collideRect(SDL_Rect area, int x, int y)
{
  return (x >= area.x && x < area.x + area.w &&
      y >= area.y && y < area.y + area.h);
}

inline glm::vec2 rotate(glm::vec2 v, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return glm::vec2(c * v.x - s * v.y, s * v.x + c * v.y);
}
