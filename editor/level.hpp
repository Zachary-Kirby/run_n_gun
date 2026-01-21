#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

//header byte is P
struct LevelPoint{
  int x = 0;
  int y = 0;
  std::string type;
  std::string parameters;
  std::string serialize();
  void deserialize(std::string_view data);
};

//header byte is R
struct LevelRect
{
  int left;
  int top;
  int right;
  int bottom;
  std::string type;
  std::string parameters;
  std::string serialize();
  void deserialize(std::string_view data);
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
  std::vector<LevelPoint> points; // Points are used for objects like enemies, pickups, etc.
  std::vector<LevelRect> rects; // Rects are used for triggers and anything that might need a 2D area
  int tileSize = 8;
  int scale = 2;
  
  void init(SDL_Texture* loadedAtlas, SDL_Texture* loadedCollisionAtlas, SDL_Rect editArea);
  void set(int x, int y, int tile, int layer = 0);
  int get(int x, int y, int layer);
  int uget(int x, int y, int layer);
  void addPoint(LevelPoint point) {points.push_back(point);};
  void addRect(LevelRect rect) {rects.push_back(rect);};
  void removePoint(int x, int y);
  void removeRect(int x, int y);
  void draw(SDL_Renderer* renderer, int offsetX, int offsetY, int cameraX, int cameraY, int selectedLayer);
  void save(char* levelName);
  void load(char* levelName);
};