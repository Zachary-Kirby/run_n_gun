#include "engine.hpp"

#include <thread>
#include <iostream>
#include "constants.hpp"
#include "renderCircle.hpp"

Engine::Engine()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640, 640*9/16, SDL_WINDOW_SHOWN, &window, &renderer);
  
  
  //Temporary to center on my left monitor
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(2), SDL_WINDOWPOS_CENTERED_DISPLAY(2));
  SDL_SetWindowTitle(window, "Run And Gun!");
  
  //SDL_SetRelativeMouseMode(SDL_TRUE);
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
  level.load("good.lvl");
  for (auto point : level.points)
  {
    if (point.type == "start") 
    {
      player.hitbox.x = point.x-player.hitbox.w/2;
      player.hitbox.y = point.y-player.hitbox.h/2;
    }
    if (point.type == "bird")
    {
      
      Sprite birdSprite{atlas, 0, 7*8, 8, 8, 1};
      birds.emplace_back(this, birdSprite, glm::vec2(point.x, point.y));
      point.active = false; // too lazy to just remove the point from the vector. Honestly it would be better as a linked list probably
    }
  }
  
  gameplayDrawTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, gameplayDrawWidth, gameplayDrawHeight);
  background = IMG_LoadTexture(renderer, "Assets/Background1.png");
  clouds = IMG_LoadTexture(renderer, "Assets/clouds.png");
  SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
}

void Engine::run()
{
  last_frame_time = std::chrono::steady_clock().now();
  while (exit_game == false)
  {
    input();
    
    player.update(level, delta);
    
    if (player.hitbox.x > gameplayDrawWidth/2+player.hitbox.w*1+camera.x)
      camera.x += player.hitbox.x - (gameplayDrawWidth/2+player.hitbox.w*1+camera.x);
    if (player.hitbox.x < gameplayDrawWidth/2-player.hitbox.w*1+camera.x)
      camera.x += player.hitbox.x - (gameplayDrawWidth/2-player.hitbox.w*1+camera.x);
    
    
    
    for (Bullet& bullet : bullets)
      if (bullet.active) bullet.update();
    for (Bird& bird : birds)
      if (bird.active) bird.update();
    
    //Draw gameplay
    
    
    SDL_SetRenderTarget(renderer, gameplayDrawTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    
    skyVerts[1].position.x = gameplayDrawWidth;
    skyVerts[2].position.y = gameplayDrawHeight;
    skyVerts[3].position.x = gameplayDrawWidth;
    skyVerts[3].position.y = gameplayDrawHeight;
    SDL_RenderGeometry(renderer, NULL, skyVerts, 4, backgroundIndices, 6);
    
    SDL_Vertex scaledBackgroundPoints[4];
    
    for (int x=0; x<4; x++)
    {
      memcpy(scaledBackgroundPoints, backgroundVerts, sizeof(backgroundVerts));
      transformVerticies(scaledBackgroundPoints, 4, backgroundHeight/9.0f, (int)-camera.x*0.25f+x*backgroundWidth, backgroundHeight*0.3f);
      SDL_RenderGeometry(renderer, clouds, scaledBackgroundPoints, 4, backgroundIndices, 6);
      memcpy(scaledBackgroundPoints, backgroundVerts, sizeof(backgroundVerts));
      transformVerticies(scaledBackgroundPoints, 4, backgroundHeight/9.0f, (int)-camera.x*0.5f+x*backgroundWidth, 0);
      SDL_RenderGeometry(renderer, background, scaledBackgroundPoints, 4, backgroundIndices, 6); 
      transformVerticies(scaledBackgroundPoints, 4, 1, (int)-camera.x*0.25f, 0);
      SDL_RenderGeometry(renderer, clouds, scaledBackgroundPoints, 4, backgroundIndices, 6);
      
    }
    
    level.draw(renderer, 0, 0, camera.x, camera.y);
    for (auto it = bullets.begin(); it != bullets.end(); it++)
    {
      Bullet& bullet = *it;
      if (bullet.active) bullet.draw(renderer, camera);
      else {it = bullets.erase(it); it--;}
    }
    player.draw(renderer, camera);
    for (auto bird : birds) if (bird.active) bird.draw(renderer, camera);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    renderCircle(renderer, {
      player.hitbox.x+player.hitbox.w/2.0f+aimPoint.x*8.0f-camera.x, 
      player.hitbox.y+player.hitbox.h/2.0f+aimPoint.y*8.0f-camera.y}, 2.0f);
    renderCircle(renderer, {
      player.hitbox.x+player.hitbox.w/2.0f+secretAimPoint.x*8.0f-camera.x, 
      player.hitbox.y+player.hitbox.h/2.0f+secretAimPoint.y*8.0f-camera.y}, 2.0f);
    
    
    SDL_SetRenderTarget(renderer, NULL);
    
    //TODO draw UI
    
    
    SDL_RenderCopy(renderer, gameplayDrawTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    std::this_thread::sleep_until(last_frame_time + frame_time);
    delta = 0.001f*std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now() - last_frame_time).count();
    last_frame_time = std::chrono::steady_clock().now();
    std::cout << delta << std::endl;
    //SDL_RenderPresent(renderer);
  }
}

void Engine::input()
{
  int x, y;
  unsigned int buttonState = SDL_GetMouseState(&x, &y);
  if (buttonState & SDL_BUTTON_LEFT)
  {
  }
    
  
  
  const unsigned char* keys = SDL_GetKeyboardState(NULL);
  
  player.controlStickX = 0.0f;
  player.controlStickY = 0.0f;
  if (keys[SDL_SCANCODE_A])
  {
    player.controlStickX = -1.0f;
  }
  if (keys[SDL_SCANCODE_D])
  {
    player.controlStickX = 1.0f;
  }
  if (keys[SDL_SCANCODE_W])
  {
    player.controlStickY = -1.0f;
  }
  if (keys[SDL_SCANCODE_S])
  {
    player.controlStickY = 1.0f;
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
        player.jump();
      }
      if (event.key.keysym.sym == SDLK_ESCAPE) exit_game = true;
      if (event.key.keysym.sym == SDLK_r) {player.hitbox.x = 0; player.hitbox.y = 0; }
      if (event.key.keysym.sym == SDLK_BACKQUOTE) {
        SDL_bool mode = SDL_GetRelativeMouseMode();
        SDL_SetRelativeMouseMode(mode ? SDL_FALSE : SDL_TRUE);
      }
    }
    if (event.type == SDL_KEYUP && event.key.repeat == false)
    {
      if (event.key.keysym.sym == SDLK_SPACE)
      {
        player.jumpLetGo();
      }
    }
    if (event.type == SDL_MOUSEMOTION)
    {
      secretAimPoint.x += ((float)event.motion.xrel / 20.0f); //TODO make sensitivity variable
      secretAimPoint.y += ((float)event.motion.yrel / 20.0f);
      if (glm::length(secretAimPoint) > 0)
      {
        secretAimPoint = secretAimPoint / glm::length(secretAimPoint) * std::min(glm::length(secretAimPoint), 5.0f);
        aimPoint = glm::normalize(secretAimPoint);  
      }
      
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      if (event.button.button == SDL_BUTTON_LEFT)
      {
        bullets.push_back({{atlas, 2*8, 4*8, 8, 8, 1}, player.hitbox.center()+aimPoint*4.0f-glm::vec2{4, 4}, {aimPoint.x*2, aimPoint.y*2}});
      }
    }
  }
}

Engine::~Engine()
{
  SDL_DestroyTexture(atlas);
  SDL_DestroyTexture(gameplayDrawTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}