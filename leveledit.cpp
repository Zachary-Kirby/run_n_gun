#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <thread>
#include <cmath>
#include <fstream>
#include "glm/vec2.hpp"

int clamp(int value, int min, int max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

bool collideRect(SDL_Rect area, int x, int y)
{
  return (x >= area.x && x < area.x + area.w &&
      y >= area.y && y < area.y + area.h);
}

glm::vec2 rotate(glm::vec2 v, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return glm::vec2(c * v.x - s * v.y, s * v.x + c * v.y);
}

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

class Pallete
{
  int tile_size = 8;
  SDL_Texture* atlas;
  public:
  int atlasWidth = 0;
  int atlasHeight = 0;
  int scale = 2;
  int x = 0;
  int y = 0;
  int mouseX = 0;
  int mouseY = 0;
  bool enabled = false;
  int selectedTile = 1;
  void init(SDL_Texture* loadedAtlas)
  {
    atlas = loadedAtlas;
    SDL_QueryTexture(atlas, NULL, NULL, &atlasWidth, &atlasHeight);
  }
  
  int getSelectedTile(int mouseX, int mouseY)
  {
    //handles the mouse in window coordinates
    int relX = mouseX - x;
    int relY = mouseY - y;
    int atlasTileWidth = atlasWidth/tile_size;
    int atlasTileHeight = atlasHeight/tile_size;
    int tileX = relX / tile_size / scale;
    int tileY = relY / tile_size / scale;
    int tileID = tileX + tileY * atlasTileWidth;
    return tileID;
  }
  
  void update()
  {
    if (!enabled) return;
    selectedTile = getSelectedTile(mouseX, mouseY);
  }
  
  void draw(SDL_Renderer* renderer)
  {
    if (!enabled) return;
    SDL_Rect dstRect {x, y, atlasWidth*scale, atlasHeight*scale};
    SDL_Rect clipRect {x, y, 128, 128};
    int relX = mouseX - x;
    int relY = mouseY - y;
    int scaled_tile_size = tile_size * scale;
    SDL_Rect selectedRect {
      x + (relX/scaled_tile_size)*scaled_tile_size, 
      y + (relY/scaled_tile_size)*scaled_tile_size, 
      scaled_tile_size, 
      scaled_tile_size};
    SDL_RenderSetClipRect(renderer, &clipRect);
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
    SDL_RenderFillRect(renderer, &dstRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderCopy(renderer, atlas, NULL, &dstRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &selectedRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderSetClipRect(renderer, NULL);
    
  }
};

class Text
{
  SDL_Texture* atlas;
public:
  void init(SDL_Renderer* renderer)
  {
    atlas = IMG_LoadTexture(renderer, "./Assets/text.png");
  }
  
  void draw(SDL_Renderer* renderer, const char* text, int x, int y, int w, int scale=1)
  {
    int i=0;
    SDL_Rect srcRect {0, 0, 8, 8};
    SDL_Rect dstRect {x, y, 8*scale, 8*scale};
    while (text[i] != '\0')
    {
      int character = (int)(text[i]);
      srcRect.x = (character % 32)*8;
      srcRect.y = (character / 32)*8;
      SDL_RenderCopy(renderer, atlas, &srcRect, &dstRect);
      if (text[i] == '\n') {dstRect.y+=8*scale; dstRect.x=x;} else dstRect.x+=8*scale;
      if (dstRect.x >= x+w) {dstRect.x = x; dstRect.y+=8*scale;}
      i++;
    }
  }
};

class Engine {
public:
  SDL_Window* window;
  SDL_Texture* screenTexture;
  int windowScale = 2;
  int windowWidth = 640;
  int windowHeight = 480;
  SDL_Renderer* renderer;
  bool exit_game{false};
  SDL_Texture* atlas;
  bool noInteractionsYet = true;
  glm::ivec2 mousePos, preMousePos;
  int mouseState = 0, preMouseState = 0;
  
  glm::ivec2 camera;
  
  Level level;
  Pallete pallete;
  Text textRenderer;
  std::chrono::steady_clock::time_point lastFrameTime;
  std::chrono::duration<long long, std::ratio<1, 240>> frameDuration{1};
  
  char levelName[256]{'\0'};
  bool saveDialogueEnabled = false;
  bool loadDialogueEnabled = false;
  
  Engine()
  {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Run And Gun Editor");
    atlas = IMG_LoadTexture(renderer, "./Assets/Atlas.png");
    pallete.init(atlas);
    textRenderer.init(renderer);
    SDL_Rect tileEditArea {0, 0, (windowWidth-160)/windowScale, windowHeight/windowScale};
    level.init(atlas, tileEditArea);
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth/windowScale, windowHeight/windowScale);
  }
  
  void run()
  {
    lastFrameTime = std::chrono::steady_clock::now();
    while (exit_game == false)
    {
      std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
      
      // Handle input
      input();
      
      // Update
      pallete.update();
      
      // Render
      SDL_SetRenderTarget(renderer, screenTexture);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      
      
      SDL_RenderSetClipRect(renderer, &level.tileEditArea);
      level.draw(renderer, 0, 0, camera.x, camera.y);
      SDL_RenderSetClipRect(renderer, NULL);
      pallete.draw(renderer);
      
      SDL_SetRenderTarget(renderer, NULL);
      SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
      
      if (noInteractionsYet) textRenderer.draw(renderer, "Run And Gun Level Editor\nLeft Click: Place Tile\nRight Click: Erase Tile\nTAB: Open Pallete\nMiddle Click: Drag Camera", 4, 4, windowWidth - 8, 2);
      
      if (saveDialogueEnabled) 
      {
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
        SDL_Rect saveDialogueRect {0, windowHeight - 32, windowWidth, 32};
        SDL_RenderFillRect(renderer, &saveDialogueRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        textRenderer.draw(renderer, "Press Enter to Save", windowWidth / 2 - 16*strlen("Press Enter to Save")/2, windowHeight - 32, 16*strlen("Press Enter to Save"), 2);
        textRenderer.draw(renderer, levelName, 0, windowHeight - 16, windowWidth - 16, 2);
      }
      
      if (loadDialogueEnabled) 
      {
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
        SDL_Rect loadDialogueRect {0, windowHeight - 32, windowWidth, 32};
        SDL_RenderFillRect(renderer, &loadDialogueRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        textRenderer.draw(renderer, "Press Enter to Load", windowWidth / 2 - 16*strlen("Press Enter to Load")/2, windowHeight - 32, 16*strlen("Press Enter to Load"), 2);
        textRenderer.draw(renderer, levelName, 0, windowHeight - 16, windowWidth - 16, 2);
      }
      
      std::this_thread::sleep_until(lastFrameTime + frameDuration);
      lastFrameTime = std::chrono::steady_clock::now();
      
      
      //std::cout << "Current FPS: " << 1.0 / std::chrono::duration<double>(lastFrameTime - now).count() << "\n";
      
      
      SDL_RenderPresent(renderer);
    }
  }
  
  void input()
  {
    
    preMouseState = mouseState;
    preMousePos.x = mousePos.x;
    preMousePos.y = mousePos.y;
    mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
    mousePos.x = mousePos.x  / windowScale;
    mousePos.y = mousePos.y / windowScale;
    
    if (collideRect(level.tileEditArea, mousePos.x, mousePos.y))
    {
      if (mouseState & SDL_BUTTON(1)) {level.set((mousePos.x+camera.x)/16, (mousePos.y+camera.y)/16, pallete.selectedTile); noInteractionsYet=false;};
      if (mouseState & SDL_BUTTON(3)) {level.set((mousePos.x+camera.x)/16, (mousePos.y+camera.y)/16, 0); noInteractionsYet=false;};
      if (mouseState & SDL_BUTTON(2)) 
      {
        //Camera Movement
        camera.x -= mousePos.x - preMousePos.x;
        camera.y -= mousePos.y - preMousePos.y;
        noInteractionsYet=false;
      }
    }
    
    glm::vec2 position{mousePos.x, mousePos.y};
    
    
    pallete.mouseX = mousePos.x;
    pallete.mouseY = mousePos.y;
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        exit_game = true;
      }
      if (event.type == SDL_KEYUP && event.key.repeat == 0)
      {
        if (event.key.keysym.sym == SDLK_TAB) pallete.enabled = false;
      }
      if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
      {
        
        if (event.key.keysym.sym == SDLK_TAB)
        {
          pallete.enabled = true;
          pallete.x = clamp(mousePos.x - (pallete.atlasWidth * pallete.scale) / 2, 0, windowWidth/windowScale - (pallete.atlasWidth * pallete.scale));
          pallete.y = clamp(mousePos.y - (pallete.atlasHeight * pallete.scale) / 2, 0, windowHeight/windowScale - (pallete.atlasHeight * pallete.scale));
        }
        
        
        
        if (SDL_IsTextInputActive())
        {
          if (event.key.keysym.sym == SDLK_RETURN)
          {
            if (saveDialogueEnabled) {
              std::cout << levelName << " saved" << std::endl;
              level.save(levelName);
              saveDialogueEnabled = false;
            }
            else if (loadDialogueEnabled)
            {
              std::cout << levelName << " loaded" << std::endl;
              
              level.load(levelName);
              loadDialogueEnabled = false;
            }
            SDL_StopTextInput();
          }
          if (event.key.keysym.sym == SDLK_BACKSPACE)
          {
            int len = strlen(levelName);
            levelName[len-1] = '\0';
          }
        }
        
        
        
        bool dialogueOpen = saveDialogueEnabled || loadDialogueEnabled;
        
        if (!dialogueOpen)
        {
          if (event.key.keysym.sym == SDLK_s)
          {
            saveDialogueEnabled = true;
            SDL_StartTextInput();
          }
          
          if (event.key.keysym.sym == SDLK_l)
          {
            loadDialogueEnabled = true;
            SDL_StartTextInput();
          }
        }
        
        
        
      }
      if (event.type == SDL_TEXTINPUT)
      {
        strncat(levelName, event.text.text, 255-strlen(levelName));
      }
      if (event.type == SDL_TEXTEDITING)
      {
        
      }
      
      
      
      
    }
  }
  
  ~Engine()
  {
    SDL_DestroyTexture(atlas);
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