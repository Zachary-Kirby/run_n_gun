#pragma once
#include <memory>
#include <vector>
#include "rect.hpp"
#include "levelObjects.hpp"

struct resolvedCollision
{
  glm::ivec2 cancelVec;
  fRect newRect;
};

class Renderer;

class Level
{
  /*
  *  Convert tile coordinate to pixel coordinate
  */
  int PTT(int x){ return (x/tileSize); }
  public:
  std::vector<LevelPoint> points;
  int width = 512;
  int height = 256;
  int layerCount = 2;
  std::unique_ptr<int[]> data = std::make_unique<int[]>(width * height * layerCount);
  SDL_Texture* atlas;
  int tileSize = 8;
  int scale = 1;
  
  void init(SDL_Texture* loadedAtlas);
  
  void set(int x, int y, int tile, int layer);
  
  int get(int x, int y, int layer);
  
  int uget(int x, int y, int layer);
  
  void draw(Renderer* renderer, int offsetX, int offsetY, int cameraX, int cameraY);
  
  void save(char* levelName);
  
  void load(const char* levelName);
  
  /*
  * Give two rects and this will resolve a move from previousRect to rect and return a new Rect that is collision free.
  * If verticalMove is true, then the collision check will only be done on the y axis, else it will be done on the x axis.
  */
  resolvedCollision resolveCollision(fRect previousRect, fRect rect, bool verticalMove = false, int collisionMask = 0b00000);
  
};


struct MoveCollision
{
  glm::vec2 position;
  glm::ivec2 cancelVec;
};

MoveCollision move(fRect hitbox, glm::vec2 move, Level& level, int collisionMask = 0b00000);