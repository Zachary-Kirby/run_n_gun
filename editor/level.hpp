#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <string_view>

struct levelPoint{
  int x = 0;
  int y = 0;
  //type could be anything spawnpoint, or savepoint, etc.
  std::string type;
  //parameters could be anything like health for a spawnpoint
  //it will need to be parsed by the game however.
  std::string parameters;
  std::string serialize()
  {
    std::string data;
    data.reserve(sizeof(char) + sizeof(x) + sizeof(y) + sizeof(int) + type.size() + sizeof(int) + parameters.size());
    
    //The object type header
    data.append("P");
    
    data.append(reinterpret_cast<char*>(&x), sizeof(x));
    data.append(reinterpret_cast<char*>(&y), sizeof(y));
    
    int typeSize = type.size();
    data.append(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
    data.append(type);

    int paramSize = parameters.size();
    data.append(reinterpret_cast<char*>(&paramSize), sizeof(paramSize));
    data.append(parameters);
    
    return data;
  }
  
  void deserialize(std::string_view data)
  {
    int cursor = 0;
    x = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(x);
    y = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(y);
    int typeSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(typeSize);
    type = std::string(data.data()+cursor, typeSize); cursor += type.size();
    int paramSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(paramSize);
    parameters = std::string(data.data()+cursor, paramSize);
  }
};

struct levelRect
{
  int left;
  int top;
  int right;
  int bottom;
  //type could be anything spawnpoint, or savepoint, etc.
  std::string type;
  //parameters could be anything like health for a spawnpoint
  //it will need to be parsed by the game however.
  std::string parameters;
  std::string serialize()
  {
    std::string data;
    data.reserve(sizeof(char) + sizeof(left) + sizeof(top) + sizeof(right) + sizeof(bottom) + sizeof(int) + type.size() + sizeof(int) + parameters.size());
    
    //The object type header
    data.append("R");
    
    data.append(reinterpret_cast<char*>(&left), sizeof(left));
    data.append(reinterpret_cast<char*>(&top), sizeof(top));
    data.append(reinterpret_cast<char*>(&right), sizeof(right));
    data.append(reinterpret_cast<char*>(&bottom), sizeof(bottom));
    
    int typeSize = type.size();
    data.append(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
    data.append(type);

    int paramSize = parameters.size();
    data.append(reinterpret_cast<char*>(&paramSize), sizeof(paramSize));
    data.append(parameters);
    
    return data;
  }
  
  void deserialize(std::string_view data)
  {
    int cursor = 0;
    left = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(left);
    top = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(top);
    right = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(right);
    bottom = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(bottom);
    int typeSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(typeSize);
    type = std::string(data.data()+cursor, typeSize); cursor += type.size();
    int paramSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(paramSize);
    parameters = std::string(data.data()+cursor, paramSize);
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
  std::unique_ptr<levelPoint[]> points = std::make_unique<levelPoint[]>(1024);
  std::unique_ptr<levelRect[]> rects = std::make_unique<levelRect[]>(1024);
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