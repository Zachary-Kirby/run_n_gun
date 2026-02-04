#pragma once
#include "SDL2/SDL.h"

class Renderer;

class Sprite
{
  //Sprite class to handle sprite drawing
  //It doesn't handle the texture lifecycle
  SDL_Rect src;
  SDL_Rect dst;
  int scale;
public:
  
  Sprite();
  
  Sprite(const Sprite& other);
  Sprite& operator= (const Sprite& other);
  
  Sprite (int x, int y, int w, int h, int scale); 
  
  void init (Sprite other);
  
  void setPostion(int x, int y);
  
  void draw(Renderer* renderer);
  
};