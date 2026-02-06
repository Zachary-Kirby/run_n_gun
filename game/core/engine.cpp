#include "engine.hpp"

#include <thread>
#include <iostream>
#include "constants.hpp"
#include "renderCircle.hpp"
#include "glm/gtc/matrix_transform.hpp"

Engine::Engine()
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Run And Gun!", SDL_WINDOWPOS_CENTERED_DISPLAY(2), SDL_WINDOWPOS_CENTERED_DISPLAY(2), 640, 640*9/16, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  context = SDL_GL_CreateContext(window);
  
  rendererGL.init(windowWidth, windowHeight);
  
  //Temporary to center on my left monitor
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED_DISPLAY(2), SDL_WINDOWPOS_CENTERED_DISPLAY(2));
  SDL_SetWindowTitle(window, "Run And Gun!");
  
  //SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Engine::init()
{
  //atlas = IMG_LoadTexture(rendererSDL, "Assets/Atlas.png");
  //if (!atlas)
  //{
  //  std::cerr << "Failed to load atlas: " << IMG_GetError() << std::endl;
  //  exit(1);
  //}
  player.init(
    {0, 0}, //Position
    16, 16, //Hitbox
    {0, 8*4, 16, 16, 1}, //Sprite
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
      
      Sprite birdSprite{0, 7*8, 8, 8, 1};
      birds.emplace_back(this, birdSprite, glm::vec2(point.x, point.y));
      point.active = false; // too lazy to just remove the point from the vector. Honestly it would be better as a linked list probably
    }
  }
  
  //gameplayDrawTexture = SDL_CreateTexture(rendererSDL, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, gameplayDrawWidth, gameplayDrawHeight);
  //background = IMG_LoadTexture(rendererSDL, "Assets/Background1.png");
  //clouds = IMG_LoadTexture(rendererSDL, "Assets/clouds.png");
  //SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
}

void Engine::run()
{
  glm::mat4 proj;
  last_frame_time = std::chrono::steady_clock().now();
  float shadowCamX = camera.x;
  while (exit_game == false)
  {
    input();
    
    player.update(level, delta);
    //{
    //  float targetX = (player.hitbox.x + player.hitbox.w/2) - rendererGL.gameplayDrawWidth/2;
    //  float dif = (targetX - shadowCamX);
    //  shadowCamX = shadowCamX + dif*2.0f*delta;
    //  camera.x = shadowCamX + (targetX - shadowCamX) * 2.0f;
    //}
    {
      float followPoint = (player.hitbox.x+secretAimPoint.x*8.0f);
      if (followPoint > gameplayDrawWidth/2+player.hitbox.w+camera.x)
        camera.x += followPoint - (gameplayDrawWidth/2+player.hitbox.w+camera.x);
      if (followPoint < gameplayDrawWidth/2-player.hitbox.w+camera.x)
        camera.x += followPoint - (gameplayDrawWidth/2-player.hitbox.w+camera.x);
    }
    
    
    
    for (Bullet& bullet : bullets)
      if (bullet.active) bullet.update(delta);
    for (Bird& bird : birds)
      if (bird.active) bird.update(delta);
    
    //Draw gameplay
    rendererGL.gameplayRenderTarget.bind();
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,rendererGL.gameplayDrawWidth, rendererGL.gameplayDrawHeight);
    proj = glm::ortho(0.0f, (float)rendererGL.gameplayDrawWidth, (float)rendererGL.gameplayDrawHeight, 0.0f, -1.0f, 1.0f);
    rendererGL.setProjectionMatrix(proj);
    //glm::mat4 proj = glm::ortho(0.0f, (float)rendererGL.gameplayDrawWidth, (float)rendererGL.gameplayDrawHeight, 0.0f, -1.0f, 1.0f);
    //rendererGL.setProjectionMatrix(proj);
    
    level.draw(&rendererGL, 0, 0, camera.x, camera.y);
    player.draw(&rendererGL, camera);
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
      Bullet& bullet = *it;
      if (bullet.active) {
        bullet.draw(&rendererGL, camera); 
        it++;
      }
      else {it = bullets.erase(it);}
    }
    for (auto bird : birds) if (bird.active) bird.draw(&rendererGL, camera);
    RenderCircle(&rendererGL,
      player.hitbox.x+player.hitbox.w/2.0f+secretAimPoint.x*8.0f-camera.x, 
      player.hitbox.y+player.hitbox.h/2.0f+secretAimPoint.y*8.0f-camera.y,
      4.0f);
    RenderTarget::unbind();
    //glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glViewport(0,windowHeight/2-(windowWidth/16*9)/2,windowWidth,windowWidth/16*9);
    proj = glm::ortho(0.0f, (float)windowWidth, (float)(windowWidth/16*9), 0.0f, -1.0f, 1.0f);
    rendererGL.setProjectionMatrix(proj);
    rendererGL.gameplayDrawTexture.setTarget(0);
    rendererGL.textureSize[0] = 1.0f;
    rendererGL.textureSize[1] = 1.0f;
    
    //TODO BUG: RenderCopy only uses the atlas for pixel coordinate sources, meaning that it doesn't
    //work with other textures as expected
    
    RenderCopy(&rendererGL,
     {0.0f,-1.0f,1.0f,1.0f}, 
     {0.0f,0.0f,(float)windowWidth,(float)(windowWidth/16*9)},
     0.0f
    );
    
    rendererGL.atlasTexture.setTarget(0);
    rendererGL.textureSize[0] = rendererGL.atlasTexture.w;
    rendererGL.textureSize[1] = rendererGL.atlasTexture.h;
    
    std::this_thread::sleep_until(last_frame_time + frame_time);
    delta = 0.001f*std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now() - last_frame_time).count();
    last_frame_time = std::chrono::steady_clock().now();
    //SDL_RenderPresent(renderer);
    SDL_GL_SwapWindow(window);
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
    if (event.type == SDL_WINDOWEVENT)
    {
      if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
      {
        windowWidth = event.window.data1;
        windowHeight = event.window.data2;
        
        std::cout << windowWidth << " " << windowHeight << std::endl;
      }
      
    }
    
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
        bullets.push_back({{2*8, 4*8, 8, 8, 1}, player.hitbox.center()+aimPoint*4.0f-glm::vec2{4, 4}, {aimPoint.x*320.0f, aimPoint.y*320.0f}});
      }
    }
  }
}

Engine::~Engine()
{
  //SDL_DestroyTexture(atlas);
  //SDL_DestroyTexture(gameplayDrawTexture);
  //SDL_DestroyRenderer(rendererSDL);
  SDL_DestroyWindow(window);
  SDL_Quit();
}