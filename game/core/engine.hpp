#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <chrono>
#include <list>

#include "renderer.hpp"
#include "level.hpp"
#include "player.hpp"
#include <iostream>
#include "bird.hpp"
#include "bullet.hpp"
#include "pole.hpp"
#include "sound.hpp"
#include "particle.hpp"

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
  std::chrono::duration<double, std::ratio<1, 244>> frame_time {1};
  int windowWidth = 640;
  int windowHeight = 640*9/16;
  
  //Sound Scope ensures that Mixer gets de-initialized after all sounds by being before the sounds
  public:
  SoundScope soundScope;
  Sound laserSound;
  Sound explodeiateSound;
  Music song1;
  
  
  
  //These are all used by a system, but level does not depend on any entity and is more
  //self contained
  
  //entities may or may not need, level, player, bullets, and camera. Needed dependancies can be given
  //in function calls like update and draw.
  Level level;
  
  Player player;
  std::vector<Bird> birds;
  std::vector<Pole> poles;
  std::vector<Bullet> bullets;
  std::vector<Particle> particles;
  glm::vec2 camera {.0f, 50.0f};
  
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