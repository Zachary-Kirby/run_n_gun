#include "engine.hpp"

#include <thread>
#include <iostream>
#include "constants.hpp"
#include "renderCircle.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "animations.hpp"

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
    {rendererGL.spriteAnimations.getFrame("Player_Idle", 0)},//Sprite
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
    if (point.type == "pole")
    {
      int length = std::stoi(point.parameters);
      poles.emplace_back(this, (SDL_FRect){32, 0, 8, 24}, point.x, point.y+length/2, length);
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
  std::chrono::time_point startTime = std::chrono::steady_clock().now();
  last_frame_time = std::chrono::steady_clock().now();
  float shadowCamX = camera.x;
  while (exit_game == false)
  {
    float second = std::chrono::duration_cast<std::chrono::milliseconds>(last_frame_time-startTime).count()/1000.0f;
    input();
    
    
    player.update(level, delta);
    { // Camera Follow
      float followPoint = (player.hitbox.x+secretAimPoint.x*8.0f);
      if (followPoint > rendererGL.gameplayDrawWidth/2+player.hitbox.w+camera.x)
        camera.x += followPoint - (rendererGL.gameplayDrawWidth/2+player.hitbox.w+camera.x);
      if (followPoint < rendererGL.gameplayDrawWidth/2-player.hitbox.w+camera.x)
        camera.x += followPoint - (rendererGL.gameplayDrawWidth/2-player.hitbox.w+camera.x);
    }
    
    for (Pole& pole: poles)
    {
      pole.update(delta);
    }
    
    if (!player.grounded)
    {
      player.sprite.setDefinition(rendererGL.spriteAnimations.getFrame("Player_Air", 0, player.facingLeft, false));
    }
    else if (std::abs(player.velocity.x) > 0.1f)
    {
      player.sprite.setDefinition(rendererGL.spriteAnimations.getAnimationFrame("Player_Run", second, player.facingLeft, false));
    }
    else
    {
      player.sprite.setDefinition(rendererGL.spriteAnimations.getFrame("Player_Idle", 0, player.facingLeft));
    }
    
    for (Bullet& bullet : bullets)
      if (bullet.active) bullet.update(delta);
    for (Bird& bird : birds)
      if (bird.active) bird.update(delta);
    
    //Draw gameplay
    rendererGL.gameplayRenderTarget.bind();
    
    //glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,rendererGL.gameplayDrawWidth, rendererGL.gameplayDrawHeight);
    proj = glm::ortho(0.0f, (float)rendererGL.gameplayDrawWidth, (float)rendererGL.gameplayDrawHeight, 0.0f, -1.0f, 1.0f);
    rendererGL.setProjectionMatrix(proj);
    
    { // Sky gradient
      float r1 =  99.0f/255.0f;
      float g1 = 155.0f/255.0f;
      float b1 = 255.0f/255.0f;
      float a1 = 255.0f/255.0f;
      float r2 = 190.0f/255.0f;
      float g2 = 214.0f/255.0f;
      float b2 = 255.0f/255.0f;
      float a2 = 255.0f/255.0f;
      SetGradient(&rendererGL, r1, g1, b1, a1, r2, g2, b2, a2);
      RenderRect(&rendererGL, 0, 0, (float)rendererGL.gameplayDrawWidth, (float)rendererGL.gameplayDrawHeight);
      SetColor(&rendererGL, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    
    {// Clouds Layer 1
      //TODO cache and save uniform locations in the renderer class instead of looking them up every time
      //TODO rename atlas to texture0 since it is more general than the atlas
      
      glUseProgram(rendererGL.defaultShaderProgramID);
      glUniform1i(rendererGL.uniformLocations["atlas"], 1);
      rendererGL.textureSize[0] = (float)rendererGL.cloudsTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.cloudsTexture.h;
      RenderCopy(&rendererGL,
       {camera.x*0.5f,0.0f,(float)rendererGL.cloudsTexture.w,(float)rendererGL.cloudsTexture.h}, 
       {0.0f,0.0f,(float)rendererGL.cloudsTexture.w,(float)rendererGL.cloudsTexture.h},
       0.0f
      );
      rendererGL.textureSize[0] = (float)rendererGL.atlasTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.atlasTexture.h;
      glUniform1i(rendererGL.uniformLocations["atlas"], 0);
    }
    
    { // Background
      glUseProgram(rendererGL.defaultShaderProgramID);
      glUniform1i(rendererGL.uniformLocations["atlas"], 2);
      rendererGL.textureSize[0] = (float)rendererGL.backgroundTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.backgroundTexture.h;
      RenderCopy(&rendererGL,
       {camera.x*0.8f,0.0f,(float)rendererGL.backgroundTexture.w,(float)rendererGL.backgroundTexture.h}, 
       {0.0f,0.0f,(float)rendererGL.backgroundTexture.w,(float)rendererGL.backgroundTexture.h},
       0.0f
      );
      rendererGL.textureSize[0] = (float)rendererGL.atlasTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.atlasTexture.h;
      glUniform1i(rendererGL.uniformLocations["atlas"], 0);
    }

    { // Clouds Layer 2
      glUseProgram(rendererGL.defaultShaderProgramID);
      glUniform1i(rendererGL.uniformLocations["atlas"], 1);
      rendererGL.textureSize[0] = (float)rendererGL.cloudsTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.cloudsTexture.h;
      RenderCopy(&rendererGL,
       {camera.x*0.9f,50.0f,(float)rendererGL.cloudsTexture.w,(float)rendererGL.cloudsTexture.h}, 
       {0.0f,0.0f,(float)rendererGL.cloudsTexture.w,(float)rendererGL.cloudsTexture.h},
       0.0f
      );
      rendererGL.textureSize[0] = (float)rendererGL.atlasTexture.w;
      rendererGL.textureSize[1] = (float)rendererGL.atlasTexture.h;
      glUniform1i(rendererGL.uniformLocations["atlas"], 0);
    }

    level.draw(&rendererGL, 0, 0, camera.x, camera.y);
    

    for (auto it = bullets.begin(); it != bullets.end(); )
    {
      Bullet& bullet = *it;
      if (bullet.active) {
        bullet.draw(&rendererGL, camera); 
        it++;
      }
      else {it = bullets.erase(it);}
    }
    
    rendererGL.spriteAtlas.bind();
    rendererGL.textureSize[0] = rendererGL.spriteAtlas.w;
    rendererGL.textureSize[1] = rendererGL.spriteAtlas.h;
    player.draw(&rendererGL, camera);
    
    rendererGL.atlasTexture.bind();
    rendererGL.textureSize[0] = rendererGL.atlasTexture.w;
    rendererGL.textureSize[1] = rendererGL.atlasTexture.h;
    
    
    for (auto bird : birds)
    {
      if (bird.active) bird.draw(&rendererGL, camera);
    }
    RenderCircle(&rendererGL,
      (int)(player.hitbox.x+player.hitbox.w/2.0f+secretAimPoint.x*8.0f-camera.x), 
      (int)(player.hitbox.y+player.hitbox.h/2.0f+secretAimPoint.y*8.0f-camera.y),
      6.0f);
    
    for (Pole& pole: poles)
    {
      pole.draw(&rendererGL, camera);
    }
    
    
    
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
      if (event.key.keysym.sym == SDLK_F11){
        unsigned int flags = SDL_GetWindowFlags(window);
        if (SDL_WINDOW_FULLSCREEN_DESKTOP & flags)
          SDL_SetWindowFullscreen(window, 0);
        else
          SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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