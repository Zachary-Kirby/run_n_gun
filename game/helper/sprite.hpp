#pragma once
#include "SDL2/SDL.h"
#include "animations.hpp"

class Renderer;

class Sprite
{
  //Sprite class to handle sprite drawing
  //It doesn't handle the texture lifecycle
  int scale;
  public:
  SDL_FRect dst;
  SDL_FRect src;
  
  Sprite();
  
  Sprite(const Sprite& other);
  Sprite(SpriteDefinition spriteDefinition);
  Sprite& operator= (const Sprite& other);
  
  Sprite (int x, int y, int w, int h, int scale); 
  
  void setDefinition(SpriteDefinition spriteDefinition);
  
  void init (Sprite other);
  
  void setPostion(int x, int y);
  
  void draw(Renderer* renderer);
  
};