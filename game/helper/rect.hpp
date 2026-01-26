#pragma once

#include "SDL2/SDL.h"
#include "glm/glm.hpp"

#pragma once

class Rect
{
  public:
  int x,y,w,h;
  
  Rect()
  {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }
  
  Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
  {}
  
  bool collide(Rect& other)
  {
    return (x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y);
  }
  
  bool collide(glm::vec2& point)
  {
    return (point.x > x && point.x < x + w && point.y > y && point .y < y + h);
  }
  
  void draw(SDL_Renderer* renderer, SDL_Color color)
  {
    SDL_Rect rect = {x,y,w,h};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
  }
  
  Rect operator+(const glm::vec2& move)
  {
    return Rect(x + move.x,y + move.y,w,h);
  }
};

class fRect
{
  public:
  float x,y,w,h;
  
  fRect()
  {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }
  
  fRect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
  {}
  
  fRect(Rect other) : x(other.x), y(other.y), w(other.w), h(other.h){}
  
  float centerX() { return (x + w / 2); }
  float centerY() { return (y + h / 2); }
  
  bool collide(Rect& other)
  {
    return (x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y);
  }
  
  bool collide(glm::vec2& point)
  {
    return (point.x > x && point.x < x + w && point.y > y && point .y < y + h);
  }
  
  void draw(SDL_Renderer* renderer, SDL_Color color)
  {
    SDL_Rect rect = {(int)x,(int)y,(int)w,(int)h};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
  }
  
  fRect operator+(const glm::vec2& move)
  {
    return fRect(x + move.x,y + move.y,w,h);
  }
};
