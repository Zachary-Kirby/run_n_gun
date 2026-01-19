#pragma once

#include <SDL2/SDL.h>

class Pallete
{
  int tile_size = 8;
  SDL_Texture* atlas;
  SDL_Texture* collisionAtlas;
  public:
  int atlasWidth = 0;
  int atlasHeight = 0;
  int scale = 2;
  int x = 0;
  int y = 0;
  int mouseX = 0;
  int mouseY = 0;
  bool enabled = false;
  int selectedTile = 1;
  void init(SDL_Texture* loadedAtlas, SDL_Texture* loadedCollisionAtlas);
  
  int getSelectedTile(int mouseX, int mouseY);
  
  void update();
  
  void draw(SDL_Renderer* renderer, int selectedLayer);
};