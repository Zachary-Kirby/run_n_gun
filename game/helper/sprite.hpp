#pragma once
#include "SDL2/SDL.h"

class Sprite
{
  //Sprite class to handle sprite drawing
  //It doesn't handle the texture lifecycle
  SDL_Rect src;
  SDL_Rect dst;
  SDL_Texture* atlas;
  int scale;
  public:
  
  Sprite();
  
  Sprite(Sprite& other);
  
  Sprite (SDL_Texture* atlas, int x, int y, int w, int h, int scale); 
  
  void init (Sprite other);
  
  void setPostion(int x, int y);
  
  void draw(SDL_Renderer* renderer);
  
};