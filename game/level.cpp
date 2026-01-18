#include <fstream>
#include "level.hpp"


void Level::init(SDL_Texture *loadedAtlas)
{
  atlas = loadedAtlas;
}

void Level::set(int x, int y, int tile)
{
  if (x<0 || x>=width || y<0 || y>=height) return;
  data[x+y*width] = tile;
}

int Level::get(int x, int y)
{
  if (x<0 || x>=width || y<0 || y>=height) return 0;
  return data[x+y*width];
}

int Level::uget(int x, int y)
{
  //unsafe version of get
  return data[x+y*width];
}

void Level::draw(SDL_Renderer *renderer, int offsetX, int offsetY, int cameraX, int cameraY)
{
  //TODO optimize drawing area to only tiles on screen
  SDL_Rect drawRect {0, 0, tileSize*scale, tileSize*scale};
  for (int y=0; y<height; y++)
  {
    for (int x=0; x<width; x++)
    {
      int tile = uget(x, y);
      if (tile == 0) continue;
      
      drawRect.x = x*tileSize*scale+offsetX - cameraX;
      drawRect.y = y*tileSize*scale+offsetY - cameraY;
      SDL_Rect srcRect {
        (tile % tileSize)*tileSize,
        (tile / tileSize)*tileSize,
        tileSize,
        tileSize
      };
      
      
      SDL_RenderCopy(renderer, atlas, &srcRect, &drawRect);
    }
  }
}

void Level::save(char *levelName)
{
  char* path = new char[strlen(levelName)+5];
  strcpy(path, "levels/");
  strcat(path, levelName);
  std::ofstream file(path, std::ios_base::binary);
  delete path;
  if (file.is_open())
  {
    //file << (unsigned int)width << (unsigned int) height;
    for (int y=0; y<height; y++)
    {
      for (int x=0; x<width; x++)
      {
        file << (unsigned char)uget(x,y);
      }
    }
    file.close();
  }
}

void Level::load(const char *levelName)
{
  char* path = new char[strlen(levelName)+5];
  strcpy(path, "levels/");
  strcat(path, levelName);
  std::ifstream file(path, std::ios_base::binary);
  if (file.is_open())
  {
    char* buffer = new char[width * height];
    file.read(buffer, width * height);
    for (int y=0; y<height; y++)
    {
      for (int x=0; x<width; x++)
      {
        set(x,y,(unsigned char)buffer[y * width + x]);
      }
    }
    delete buffer;
    file.close();
  }
}

resolvedCollision Level::resolveCollision(fRect previousRect, fRect rect, bool verticalMove)
{
  
  int left {PTT(rect.x)};
  int top  {PTT(rect.y)};
  int right{PTT(rect.x + rect.w)};
  int bottom{PTT(rect.y + rect.h)};
  
  int xCancel = 0;
  int yCancel = 0;
  
  for (int y=top; y <= bottom; y++)
  {
    for (int x=left; x <= right; x++)
    {
      if (get(x,y))
      {
        Rect tileRect{x*tileSize, y*tileSize, tileSize, tileSize};
        if (rect.collide(tileRect) && !previousRect.collide(tileRect))
        {
          
          if (verticalMove)
          {
            if (rect.y < previousRect.y)
            {
              //Collision above
              rect.y = tileRect.y + tileRect.h;
              yCancel = -1;
            }
            else
            {
              //Collision below
              rect.y = tileRect.y - rect.h;
              yCancel = 1;
            }
          }
          else
          {
            if (rect.x < previousRect.x)
            {
              //Collision left
              rect.x = tileRect.x + tileRect.w;
              xCancel = 1;
            }
            else
            {
              //Collision right
              rect.x = tileRect.x - rect.w;
              xCancel = -1;
            }
          }
        }
        
      }
    }
  }
  return {{xCancel,yCancel}, rect};
}

MoveCollision move(glm::vec2 position, glm::vec2 move, fRect hitbox, Level &level)
{
  //Horizontal Move
  fRect previousRect = hitbox+position;
  fRect newRect = previousRect+glm::vec2(move.x, 0);
  resolvedCollision collision;
  collision = level.resolveCollision(previousRect, newRect, false);
  int xCol = collision.cancelVec.x;
  
  previousRect = collision.newRect;
  newRect = previousRect+glm::vec2(0, move.y);
  collision = level.resolveCollision(previousRect, newRect, true);
  int yCol = collision.cancelVec.y;
  
  return {{collision.newRect.x, collision.newRect.y}, {xCol, yCol}};
}
