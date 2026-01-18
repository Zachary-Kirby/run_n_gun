#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <format>
#include <chrono>
#include <thread>
#include <fstream>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/vec2.hpp"




class Rect
{
  public:
  int x,y,w,h;
  
  Rect()
  {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }
  
  Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
  {}
  
  bool collide(Rect& other)
  {
    return (x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y);
  }
  
  bool collide(glm::vec2& point)
  {
    return (point.x > x && point.x < x + w && point.y > y && point .y < y + h);
  }
  
  void draw(SDL_Renderer* renderer, SDL_Color color)
  {
    SDL_Rect rect = {x,y,w,h};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
  }
  
  Rect operator+(const glm::vec2& move)
  {
    return Rect(x + move.x,y + move.y,w,h);
  }
};

class fRect
{
  public:
  float x,y,w,h;
  
  fRect()
  {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
  }
  
  fRect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h)
  {}
  
  fRect(Rect other) : x(other.x), y(other.y), w(other.w), h(other.h){}
  
  bool collide(Rect& other)
  {
    return (x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y);
  }
  
  bool collide(glm::vec2& point)
  {
    return (point.x > x && point.x < x + w && point.y > y && point .y < y + h);
  }
  
  void draw(SDL_Renderer* renderer, SDL_Color color)
  {
    SDL_Rect rect = {(int)x,(int)y,(int)w,(int)h};
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(renderer,&rect);
  }
  
  fRect operator+(const glm::vec2& move)
  {
    return fRect(x + move.x,y + move.y,w,h);
  }
};

struct resolvedCollision
{
  glm::ivec2 cancelVec;
  fRect newRect;
};

class Level
{
public:
  int width = 512;
  int height = 256;
  int data[512*256]{0};
  SDL_Texture* atlas;
  int tileSize = 8;
  int scale = 1;
  
  void init(SDL_Texture* loadedAtlas)
  {
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
  
  void load(const char* levelName)
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
  
  /*
  *  Convert tile coordinate to pixel coordinate
  */
  int PTT(int x)
  {
    
    return (x/tileSize);
  }
  
  /*
  * Give two rects and this will resolve a move from previousRect to rect and return a new Rect that is collision free.
  * If verticalMove is true, then the collision check will only be done on the y axis, else it will be done on the x axis.
  */
  resolvedCollision resolveCollision(fRect previousRect, fRect rect, bool verticalMove = false)
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
  
  Sprite()
  {
    src = {0, 0, 0, 0};
    dst = {0, 0, 0, 0};
    atlas = nullptr;
    scale = 1;
  }
  
  Sprite(Sprite& other)
  {
    src = other.src;
    dst = other.dst;
    atlas = other.atlas;
    scale = other.scale;
  }
  
  Sprite (SDL_Texture* atlas, int x, int y, int w, int h, int scale = 1) : atlas(atlas), src{x,y,w,h}, dst{0,0,w*scale,h*scale}
  {
    
  }
  
  void init (Sprite other)
  {
    src = other.src;
    dst = other.dst;
    atlas = other.atlas;
    scale = other.scale;
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

struct MoveCollision
{
  glm::vec2 position;
  glm::ivec2 cancelVec;
};

MoveCollision move(glm::vec2 position, glm::vec2 move, fRect hitbox, Level& level)
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

class Engine;

class Player{
  Sprite sprite;
  glm::ivec2 spriteOffset;
  public:
  glm::vec2 velocity;
  glm::vec2 position;
  fRect hitbox;
  
  
  Player()
  {
    position = {0, 0};
    hitbox = {0, 0, 16, 16};
    spriteOffset = glm::ivec2(0, 0);
    velocity = {0, 0};
  }
  
  void init(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset)
  {
    position = pos;
    hitbox.w = hitboxWidth;
    hitbox.h = hitboxHeight;
    sprite.init(isprite);
    this->spriteOffset = spriteOffset;
  }
  
  Player(glm::vec2 pos, int hitboxWidth, int hitboxHeight, Sprite isprite, glm::ivec2 spriteOffset) : position(pos)
  {
    sprite = isprite;
    hitbox.w = hitboxWidth;
    hitbox.h = hitboxHeight;
    
    
    velocity = glm::vec2(0.0f);
  }
  
  void update(Level& level)
  {

    MoveCollision collision = move(position, velocity, hitbox, level);
    position = collision.position;
    if (collision.cancelVec.x!=0) {velocity.x = 0; std::cout << "CollisionX: " << collision.cancelVec.x << std::endl;}
    if (collision.cancelVec.y!=0) {velocity.y = 0; std::cout << "CollisionY: " << collision.cancelVec.y << std::endl;}
    velocity += glm::vec2(0.0f, 0.01f);
    if (std::abs(velocity.x) >= 0.11f)
    {
      velocity.x = velocity.x - std::copysignf(0.1f, velocity.x);
    }
    else
    {
      velocity.x = 0;
    }
    
  }
  
  void draw(SDL_Renderer* renderer)
  {
    sprite.setPostion(position.x+spriteOffset.x, position.y+spriteOffset.y);
    sprite.draw(renderer);
  }
};

class Engine {
public:
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* atlas;
  
  std::chrono::steady_clock::time_point last_frame_time;
  std::chrono::duration<double, std::ratio<1, 240>> frame_time {1};
  
  Level level;
  Player player;
  
  bool exit_game{false};
  Engine()
  {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer);
    //Temporary to center on my left monitor
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1));
    SDL_SetWindowTitle(window, "Run And Gun!");
  }
  
  void init()
  {
    atlas = IMG_LoadTexture(renderer, "Assets/Atlas.png");
    if (!atlas)
    {
      std::cerr << "Failed to load atlas: " << IMG_GetError() << std::endl;
      exit(1);
    }
    player.init(
      {0, 0}, //Position
      16, 16, //Hitbox
      {atlas, 0, 32, 16, 16, 1}, //Sprite
      {0, 0} //Sprite Offset
    );
    
    level.init(atlas);
    level.load("level.lvl");
    
    
    
  }
  
  void run()
  {
    last_frame_time = std::chrono::steady_clock().now();
    while (exit_game == false)
    {
      input();
      
      player.update(level);
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      
      
      player.draw(renderer);
      level.draw(renderer, 0, 0, 0, 0);
      
      std::this_thread::sleep_until(last_frame_time + frame_time);
      last_frame_time = std::chrono::steady_clock().now();
      
      SDL_RenderPresent(renderer);
    }
  }
  
  void input()
  {
    
    const unsigned char* keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_A])
    {
      player.velocity.x += -0.15f;
    }
    if (keys[SDL_SCANCODE_D])
    {
      player.velocity.x += +0.15f;
    }
    
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        exit_game = true;
      }
      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
          player.velocity.y = -1.0f;
        }
      }
      
    }
  }
  
  ~Engine()
  {
    SDL_DestroyTexture(atlas);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
};



int main(int argc, char** argv)
{
  
  Engine engine;
  engine.init();
  engine.run();
  
  return 0;
}