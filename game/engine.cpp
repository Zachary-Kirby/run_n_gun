#include "engine.hpp"

#include <thread>

Engine::Engine()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer);
  //Temporary to center on my left monitor
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1));
  SDL_SetWindowTitle(window, "Run And Gun!");
}

void Engine::init()
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

void Engine::run()
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

void Engine::input()
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

Engine::~Engine()
{
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}