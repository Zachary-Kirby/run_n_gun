#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include "level.hpp"
#include "glm/glm.hpp"
#include "pallete.hpp"
#include "text.hpp"
#include "inputBox.hpp"


class Engine {
public:
  SDL_Window* window;
  SDL_Texture* screenTexture;
  int windowScale = 2;
  int windowWidth = 640*2;
  int windowHeight = 480*2;
  SDL_Renderer* renderer;
  bool exit_game{false};
  SDL_Texture* atlas;
  SDL_Texture* collisionAtlas;
  bool noInteractionsYet = true;
  glm::ivec2 mousePos, preMousePos;
  int mouseState = 0, preMouseState = 0;
  
  glm::ivec2 camera {0,0};
  
  Level level;
  Pallete pallete;
  Text textRenderer;
  std::chrono::steady_clock::time_point lastFrameTime;
  std::chrono::duration<long long, std::ratio<1, 240>> frameDuration{1};
  
  int selectedLayer = 0;
  
  
  char levelName[256]{'\0'};
  bool saveDialogueEnabled = false;
  bool loadDialogueEnabled = false;
  
  InputBox objectTypeBox;
  InputBox objectParametersBox;
  
  
  Engine();
  
  void run();
  
  void input();
  
  ~Engine();
};