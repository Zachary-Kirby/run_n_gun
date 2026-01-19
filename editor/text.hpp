#pragma once

#include <SDL2/SDL.h>

class Text
{
  SDL_Texture* atlas;
public:
  void init(SDL_Renderer* renderer);
  
  void draw(SDL_Renderer* renderer, const char* text, int x, int y, int w, int scale);
  
};
