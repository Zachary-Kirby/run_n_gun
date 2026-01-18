#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <chrono>

#include "level.hpp"
#include "player.hpp"
#include <iostream>


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
  
  Engine();
  
  void init();
  
  void run();
  
  void input();
  
  ~Engine();
};