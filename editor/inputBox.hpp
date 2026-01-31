#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "text.hpp"
#include "rect.hpp"

class InputBox
{
  Text* textRenderer = nullptr;
public:
  bool active = true;
  bool selected = false;
  Rect hitbox;
  std::string label;
  std::string text;
  
  InputBox() {};
  void init(const char* label, int x, int y, int width, int height, Text* textRenderer);
  void draw(SDL_Renderer* renderer);
  void handleEvent(SDL_Event event);
};
