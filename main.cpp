#include <iostream>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>
#include <fstream>
#include "glm/vec2.hpp"

class Level
{
public:
  SDL_Rect tileEditArea;
  int width = 512;
  int height = 256;
  int data[512*256]{0};
  SDL_Texture* atlas;
  int tileSize = 8;
  int scale = 2;
  
  void init(SDL_Texture* loadedAtlas, SDL_Rect editArea)
  {
    tileEditArea = editArea;
    atlas = loadedAtlas;
  }
  
  void set(int x, int y, int tile)
  {
    if (x<0 || x>=width || y<0 || y>=height) return;
    data[x+y*width] = tile;
  }
  int get(int x, int y)
  {
    if (x<0 || x>=width || y<0 || y>=height) return 0;
    return data[x+y*width];
  }
  
  int uget(int x, int y)
  {
    //unsafe version of get
    return data[x+y*width];
  }
  
  void draw(SDL_Renderer* renderer, int offsetX, int offsetY, int cameraX, int cameraY)
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
  
  void save(char* levelName)
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
  
  void load(char* levelName)
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
};

class Rect
{
  public:
  int x,y,width,height;
  Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h)
  {}
  
  bool collide(Rect& other)
  {
    return (x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y);
  }
  
  bool collide(glm::vec2& point)
  {
    return (point.x > x && point.x < x + width && point.y > y && point .y < y + height);
  }
  
  void draw(SDL_Renderer* renderer, SDL_Color color)
  {
    SDL_Rect rect = {x,y,width,height};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
  }
};


class Sprite
{
  //Sprite class to handle sprite drawing
  //It doesn't handle the texture lifecycle
  SDL_Rect src;
  SDL_Rect dst;
  SDL_Texture* atlas;
  int scale;
  public:
  Sprite (SDL_Texture* atlas, int x, int y, int w, int h, int scale = 1) : atlas(atlas), src{x,y,w,h}, dst{0,0,w*scale,h*scale}
  {
    
  }
  
  void setPostion(int x, int y)
  {
    dst.x = x;
    dst.y = y;
  }
  
  void draw(SDL_Renderer* renderer)
  {
    SDL_RenderCopy(renderer, atlas, &src, &dst);
  }
  
};

class Player{
  glm::vec2 velocity;
  Sprite sprite;
  glm::ivec2 spriteOffset;
public:
  glm::vec2 position;
  Rect hitbox;
  
  
  Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset) : position(pos)
  {
    hitbox.width = hitboxWidth;
    hitbox.height = hitboxHeight;
    sprite = isprite;
    
    velocity = glm::vec2(0.0f);
  }
  
  void update()
  {
    position += velocity;
    sprite.setPostion(position.x+spriteOffset.x, position.y+spriteOffset.y);
  }
  
  void draw(SDL_Renderer* renderer)
  {
    sprite.draw(renderer);
  }
};

class Engine {
public:
  SDL_Window* window;
  SDL_Renderer* renderer;
  
  std::chrono::steady_clock::time_point last_frame_time;
  std::chrono::duration<double, std::ratio<1, 240>> frame_time {1};
  
  Player player{glm::vec2(0.0f, 0.0f)};
  
  bool exit_game{false};
  Engine()
  {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Run And Gun!");
  }
  
  void run()
  {
    last_frame_time = std::chrono::steady_clock().now();
    while (exit_game == false)
    {
      input();
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      
      std::this_thread::sleep_until(last_frame_time + frame_time);
      last_frame_time = std::chrono::steady_clock().now();
      
      SDL_RenderPresent(renderer);
    }
  }
  
  void input()
  {
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        exit_game = true;
      }
      
    }
  }
  
  ~Engine()
  {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
};

int main(int argc, char** argv)
{
  
  Engine engine;
  engine.run();
  
  return 0;
}