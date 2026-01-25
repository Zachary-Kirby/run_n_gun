#include "engine.hpp"

#include <thread>
#include <iostream>



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
  level.load("test.lvl");
  
  
  gameplayDrawTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, gameplayDrawWidth, gameplayDrawHeight);
  
}

void Engine::run()
{
  last_frame_time = std::chrono::steady_clock().now();
  while (exit_game == false)
  {
    input();
    
    player.update(level);
    
    for (auto& levelPoint : level.points)
    {
      if (!levelPoint.active) continue;
      if (levelPoint.type == "bird")
      {
        lastBirdIndex += 1;
        Sprite birdSprite{atlas, 0, 7*8, 8, 8, 1};
        birds[lastBirdIndex].init(birdSprite, {levelPoint.x, levelPoint.y});
        levelPoint.active = false; // too lazy to just remove the point from the vector. Honestly it would be better as a linked list probably
      }
    }
    
    for (Bullet& bullet : bullets)
      if (bullet.active) bullet.update();
    
    //Draw gameplay
    
    

    SDL_SetRenderTarget(renderer, gameplayDrawTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    for (auto bullet : bullets) if (bullet.active) bullet.draw(renderer);
    player.draw(renderer);
    for (auto bird : birds) if (bird.active) bird.draw(renderer);
    level.draw(renderer, 0, 0, 0, 0);
    SDL_SetRenderTarget(renderer, NULL);
    
    //TODO draw UI
    
    SDL_RenderCopy(renderer, gameplayDrawTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    std::this_thread::sleep_until(last_frame_time + frame_time);
    last_frame_time = std::chrono::steady_clock().now();
    
    //SDL_RenderPresent(renderer);
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
    if (event.type == SDL_KEYDOWN && event.key.repeat == false)
    {
      if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
      {
        player.velocity.y = -1.0f;
      }
      if (event.key.keysym.scancode == SDL_SCANCODE_P)
      {
        int freeBullet = -1;
        for (int i = 0; i<BULLETLIMIT; i++)
        {
          if (bullets[i].active == false)
          {
            freeBullet = i;
            break;
          }
        }
        std::cout << "free bullet " << freeBullet << std::endl;
        if (freeBullet != -1)
        {
          bullets[freeBullet].init({atlas, 2*8, 4*8, 8, 8, 1}, player.position, {2.0f, 0.0f});
        }
        
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