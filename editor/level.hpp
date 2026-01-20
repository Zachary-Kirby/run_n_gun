#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>

struct levelPoint{
  int x = 0;
  int y = 0;
  //type could be anything spawnpoint, or savepoint, etc.
  std::string type;
  //parameters could be anything like health for a spawnpoint
  //it will need to be parsed by the game however.
  std::string parameters;
  char* serialize()
  {
    char* data = new char[sizeof(x)+sizeof(y)+sizeof(int)+type.size()+sizeof(int)+parameters.size()];
    memcpy(data, &x, sizeof(x));
    memcpy(data+sizeof(x), &y, sizeof(y));
    (*(data+sizeof(x)+sizeof(y))) = type.size();
    memcpy(data+sizeof(x)+sizeof(y)+sizeof(int), type.c_str(), type.size());
    (*(data+sizeof(x)+sizeof(y)+sizeof(int)+type.size())) = parameters.size();
    memcpy(data+sizeof(x)+sizeof(y)+sizeof(int)+type.size()+sizeof(int), parameters.c_str(), parameters.size());
    return data;
  }
};

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