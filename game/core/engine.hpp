#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <chrono>
#include <list>

#include "level.hpp"
#include "player.hpp"
#include <iostream>
#include "bird.hpp"
#include "bullet.hpp"
#include "renderer.hpp"

class Engine {
  SDL_GLContext context;
  SDL_Window* window;
  Renderer rendererGL;
  
  //SDL_Renderer* rendererSDL;
  SDL_Texture* atlas;
  SDL_Texture* gameplayDrawTexture;
  SDL_Texture* background;
  SDL_Texture* clouds;
  std::chrono::steady_clock::time_point last_frame_time;
  std::chrono::duration<double, std::ratio<1, 240>> frame_time {1};
  int gameplayDrawScale = 2;
  int windowWidth = 640;
  int windowHeight = 640*9/16;
  SDL_Vertex skyVerts[4] {
    { 0, 0, 0x37, 0x7f, 0xff, 0xff, 0, 0},
    { 1, 0, 0x37, 0x7f, 0xff, 0xff, 1, 0},
    { 0, 1, 0x92, 0xb8, 0xfc, 0xff, 0, 1},
    { 1, 1, 0x92, 0xb8, 0xfc, 0xff, 1, 1}
  };
  SDL_Vertex backgroundVerts[4] {
    { 0, 0, 255, 255, 255, 255,  0, 0},
    { 16, 0, 255, 255, 255, 255,  1, 0},
    { 0, 9, 255, 255, 255, 255,  0, 1},
    { 16, 9, 255, 255, 255, 255,  1, 1}
  };
  int backgroundIndices[6] {0, 1, 2, 1, 2, 3};
  int backgroundHeight, backgroundWidth;
  
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
  std::vector<Bullet> bullets;
  glm::vec2 camera {.0f, 0.0f};
  
  glm::vec2 aimPoint = {.0f, .0f}; //used for player shooting
  glm::vec2 secretAimPoint = {.0f, .0f}; //used to smooth aim point
  float delta = 0.1f;
  
  bool exit_game{false};
  Engine();
  
  void init();
  
  void run();
  
  void input();
  
  ~Engine();
};