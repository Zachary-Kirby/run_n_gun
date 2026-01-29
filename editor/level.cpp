#include "level.hpp"

#include <fstream>
#include "constants.hpp"

/*
what the level structure should be like:

☐  int width
☐  int height
☒  char[width*height] tiles
☐ int pointByteCount
☒ point ...
☐ int rectByteCount
☐ rect ...
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





/*
 * Remove all points within a certain radius of (x, y)
 */
void Level::removePoint(int x, int y)
{
  float radius = 2.0f;
  for (int i=0; i<points.size(); i++)
  {
    if ((points[i].x-x)*(points[i].x-x) + (points[i].y-y)*(points[i].y-y) < radius*radius)
    {
      points.erase(points.begin()+i);
      i--;
    }
  }
}

/*
 * Remove all rects that contain the point (x, y)
 */
void Level::removeRect(int x, int y)
{
  //TODO implement
}

void Level::draw(SDL_Renderer *renderer, int offsetX, int offsetY, int cameraX, int cameraY, int selectedLayer)
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
  for (int i=0; i<points.size(); i++)
  {
    memcpy(transformedCirclePoints, circleVertexData, sizeof(circleVertexData));
    transformVerticies(transformedCirclePoints, 17, 8.0f, points[i].x*scale-cameraX, points[i].y*scale-cameraY);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderGeometry(renderer, NULL, transformedCirclePoints, 17, circleIndices, 16*3);
  }
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
    //serialize points and rects
    for (int i=0; i<points.size(); i++)
    {
      std::string pointData = points[i].serialize();
      file.write(pointData.data(), pointData.size());
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
    //deserialize points and rects
    points.clear();
    rects.clear();
    while (file.peek() != EOF)
    {
      char header;
      file.read(&header, 1);
      if (header == 'P')
      {
        LevelPoint point;
        point.deserialize(file);
        points.push_back(point);
      }
      //TODO implement RECT loading
    }
    file.close();
  }
}



