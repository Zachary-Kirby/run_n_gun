#pragma once

#include <SDL2/SDL.h>
#include <memory>

class Level
{
  SDL_Texture* atlas;
  SDL_Texture* collisionAtlas;
  public:
  SDL_Rect tileEditArea;
  int width = 512;
  int height = 256;
  int layerCount = 2;
  std::unique_ptr<int[]> data = std::make_unique<int[]>(width * height * layerCount);
  int tileSize = 8;
  int scale = 2;
  
  void init(SDL_Texture* loadedAtlas, SDL_Texture* loadedCollisionAtlas, SDL_Rect editArea);
  
  void set(int x, int y, int tile, int layer = 0);
  
  int get(int x, int y, int layer);
  
  int uget(int x, int y, int layer);
  
  void draw(SDL_Renderer* renderer, int offsetX, int offsetY, int cameraX, int cameraY, int selectedLayer);
  
  void save(char* levelName);
  
  void load(char* levelName);
};