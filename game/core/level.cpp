#include <fstream>
#include "level.hpp"


void Level::init(SDL_Texture *loadedAtlas)
{
  atlas = loadedAtlas;
}

void Level::set(int x, int y, int tile, int layer = 0)
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

void Level::draw(SDL_Renderer *renderer, int offsetX, int offsetY, int cameraX, int cameraY)
{
  //TODO optimize drawing area to only tiles on screen
  SDL_Rect drawRect {0, 0, tileSize*scale, tileSize*scale};
  
  SDL_Texture* layerAtlas = atlas; 
  for (int layer=0; layer<layerCount; layer++)
  {
    if (layer == 1 ) continue;
    
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

void Level::load(const char *levelName)
{
  char* path = new char[strlen(levelName)+5];
  strcpy(path, "levels/");
  strcat(path, levelName);
  std::ifstream file(path, std::ios_base::binary);
  delete[] path;
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
    delete[] buffer;
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
    }
    file.close();
  }
}

//sides of a rectangle are controlled by these flags, later it could be verts on a square.
//flags: water left right up down
//collisionMask ignores that tile collision pattern specified
const unsigned char tileFlags[] = {0b00000, 0b01111, 0b00010, 0b10000};

resolvedCollision Level::resolveCollision(fRect previousRect, fRect rect, bool verticalMove, int collisionMask)
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
      int tile = get(x,y,1);
      if (tile >= 4) continue;
      unsigned char flags = tileFlags[tile];
      if (collisionMask == flags) continue;
      
      if (tile)
      {
        Rect tileRect{x*tileSize, y*tileSize, tileSize, tileSize};
        if (rect.collide(tileRect) && !previousRect.collide(tileRect))
        {
          
          if (verticalMove)
          {
            if (rect.y < previousRect.y && (flags & 0b00001))
            {
              //Collision above player
              rect.y = tileRect.y + tileRect.h;
              yCancel = -1;
            }
            else if ( rect.y > previousRect.y && (flags & 0b00010))
            {
              //Collision below player
              rect.y = tileRect.y - rect.h;
              yCancel = 1;
            }
          }
          else
          {
            if (rect.x < previousRect.x && (flags & 0b01000))
            {
              //Collision left player
              rect.x = tileRect.x + tileRect.w;
              xCancel = 1;
            }
            else if (rect.x > previousRect.x && (flags & 0b00100))
            {
              //Collision right player
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

MoveCollision move(fRect hitbox, glm::vec2 move, Level &level, int collisionMask)
{
  //Horizontal Move
  fRect previousRect = hitbox;
  fRect newRect = previousRect+glm::vec2(move.x, 0);
  resolvedCollision collision;
  collision = level.resolveCollision(previousRect, newRect, false, collisionMask);
  int xCol = collision.cancelVec.x;
  
  previousRect = collision.newRect;
  newRect = previousRect+glm::vec2(0, move.y);
  collision = level.resolveCollision(previousRect, newRect, true, collisionMask);
  int yCol = collision.cancelVec.y;
  
  return {{collision.newRect.x, collision.newRect.y}, {xCol, yCol}};
}
