#include "level.hpp"

#include <fstream>

/*
what the level structure should be like:

int width
int height
char[width*height] tiles
[int pointByteCount
point ...]
[int rectByteCount
rect ...]
*/


void Level::init(SDL_Texture *loadedAtlas, SDL_Texture *loadedCollisionAtlas, SDL_Rect editArea)
{
  tileEditArea = editArea;
  atlas = loadedAtlas;
  collisionAtlas = loadedCollisionAtlas;
}

void Level::set(int x, int y, int tile, int layer)
{
  if (x<0 || x>=width || y<0 || y>=height || layer<0 || layer>=layerCount) return;
  data[x+y*width+layer*width*height] = tile;
}

int Level::get(int x, int y, int layer = 0)
{
  if (x<0 || x>=width || y<0 || y>=height || layer<0 || layer>=layerCount) return 0;
  return data[x+y*width+layer*width*height];
}

int Level::uget(int x, int y, int layer = 0)
{
  //unsafe version of get
  return data[x+y*width+layer*width*height];
}

void transform(SDL_Vertex* verticies, int count, float scale, float translateX, float translateY)
{
  for (int i=0; i<count; i++)
  {
    verticies[i].position.x = verticies[i].position.x * scale + translateX;
    verticies[i].position.y = verticies[i].position.y * scale + translateY;
  }
}

const SDL_Vertex circlePoints[17] = {
  {1.0f, 0.0f, 255, 0, 0, 255},     
  {0.9239f, 0.3827f, 255, 0, 0, 255},
  {0.7071f, 0.7071f, 255, 0, 0, 255},
  {0.3827f, 0.9239f, 255, 0, 0, 255},
  {0.0f, 1.0f, 255, 0, 0, 255},     
  {-0.3827f, 0.9239f, 255, 0, 0, 255},
  {-0.7071f, 0.7071f, 255, 0, 0, 255},
  {-0.9239f, 0.3827f, 255, 0, 0, 255},
  {-1.0f, 0.0f, 255, 0, 0, 255},    
  {-0.9239f, -0.3827f, 255, 0, 0, 255},
  {-0.7071f, -0.7071f, 255, 0, 0, 255}, 
  {-0.3827f, -0.9239f, 255, 0, 0, 255}, 
  {0.0f, -1.0f, 255, 0, 0, 255},     
  {0.3827f, -0.9239f, 255, 0, 0, 255}, 
  {0.7071f, -0.7071f, 255, 0, 0, 255}, 
  {0.9239f, -0.3827f, 255, 0, 0, 255}, 
  {0.0f, 0.0f, 255, 0, 0, 255}          
};

const int circleIndices[16*3] = {
  0, 1, 16,
  1, 2, 16,
  2, 3, 16,
  3, 4, 16,
  4, 5, 16,
  5, 6, 16,
  6, 7, 16,
  7, 8, 16,
  8, 9, 16,
  9, 10, 16,
  10, 11, 16,
  11, 12, 16,
  12, 13, 16,
  13, 14, 16,
  14, 15, 16,
  15, 0, 16
};

void Level::draw(SDL_Renderer* renderer, int offsetX, int offsetY, int cameraX, int cameraY, int selectedLayer)
{
  //TODO optimize drawing area to only tiles on screen
  SDL_Rect drawRect {0, 0, tileSize*scale, tileSize*scale};
  
  SDL_Texture* layerAtlas = atlas; 
  for (int layer=0; layer<layerCount; layer++)
  {
    if (layer == 1 ) layerAtlas = collisionAtlas; else layerAtlas = atlas;
    if (layer == 1 && selectedLayer != 1) continue; //skip collision layer if selected layer is not collision layer
    
    for (int y=0; y<height; y++)
    {
      for (int x=0; x<width; x++)
      {
        int tile = uget(x, y, layer);
        if (tile == 0) continue;
        
        drawRect.x = x*tileSize*scale+offsetX - cameraX;
        drawRect.y = y*tileSize*scale+offsetY - cameraY;
        SDL_Rect srcRect {
          (tile % tileSize)*tileSize,
          (tile / tileSize)*tileSize,
          tileSize,
          tileSize
        };
        
        
        SDL_RenderCopy(renderer, layerAtlas, &srcRect, &drawRect);
      }
    }
  }
  SDL_Vertex transformedCirclePoints[17];
  memcpy(transformedCirclePoints, circlePoints, sizeof(circlePoints));
  transform(transformedCirclePoints, 17, 8.0f, 16, 16);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderGeometry(renderer, NULL, transformedCirclePoints, 17, circleIndices, 16*3);
}

void Level::save(char *levelName)
{
  //TODO remove or update
  char* path = new char[strlen(levelName)+5];
  strcpy(path, "levels/");
  strcat(path, levelName);
  std::ofstream file(path, std::ios_base::binary);
  delete path;
  if (file.is_open())
  {
    //file << (unsigned int)width << (unsigned int) height;
    for (int layer=0; layer<layerCount; layer++)
    {
      for (int y=0; y<height; y++)
      {
        for (int x=0; x<width; x++)
        {
          file << (unsigned char)uget(x,y,layer);
        }
      }
    }
    file.close();
  }
}

void Level::load(char* levelName)
{
  char* path = new char[strlen(levelName)+5];
  strcpy(path, "levels/");
  strcat(path, levelName);
  std::ifstream file(path, std::ios_base::binary);
  if (file.is_open())
  {
    char* buffer = new char[width * height * layerCount];
    file.read(buffer, width * height * layerCount);
    for (int layer=0; layer<layerCount; layer++)
    {
      for (int y=0; y<height; y++)
      {
        for (int x=0; x<width; x++)
        {
          set(x,y,(unsigned char)buffer[y * width + x + layer * width * height], layer);
        }
      }
    }
    delete buffer;
    file.close();
  }
}

std::string LevelPoint::serialize()
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

void LevelPoint::deserialize(std::string_view data)
{
  int cursor = 0;
  x = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(x);
  y = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(y);
  int typeSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(typeSize);
  type = std::string(data.data()+cursor, typeSize); cursor += type.size();
  int paramSize = *reinterpret_cast<const int*>(data.data()+cursor); cursor += sizeof(paramSize);
  parameters = std::string(data.data()+cursor, paramSize);
}

std::string LevelRect::serialize()
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

void LevelRect::deserialize(std::string_view data)
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


