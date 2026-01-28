#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <chrono>

#include "level.hpp"
#include "player.hpp"
#include <iostream>
#include "bird.hpp"
#include "bullet.hpp"

#define BULLETLIMIT 50

class Engine {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* atlas;
  SDL_Texture* gameplayDrawTexture = nullptr;
  std::chrono::steady_clock::time_point last_frame_time;
  std::chrono::duration<double, std::ratio<1, 240>> frame_time {1};
  int gameplayDrawScale = 2;
  int windowWidth = 640;
  int windowHeight = 480;
  public:
  int gameplayDrawWidth = windowWidth / gameplayDrawScale;
  int gameplayDrawHeight = windowHeight / gameplayDrawScale;
  
  
  
  //These are all used by a system, but level does not depend on any entity and is more
  //self contained
  
  //entities may or may not need, level, player, bullets, and camera. Needed dependancies can be given
  //in function calls like update and draw.
  Level level;
  
  Player player;
  std::vector<Bird> birds;
  Bullet bullets[BULLETLIMIT]{};
  glm::vec2 camera {.0f, .0f};
  
  glm::vec2 aimPoint = {.0f, .0f}; //used for player shooting
  glm::vec2 secretAimPoint = {.0f, .0f}; //used to smooth aim point
  
  bool exit_game{false};
  Engine();
  
  void init();
  
  void run();
  
  void input();
  
  ~Engine();
};