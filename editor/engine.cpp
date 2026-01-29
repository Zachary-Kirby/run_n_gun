#include <string>
#include "engine.hpp"
#include <filesystem>

#include "utility.hpp"
#include "levelObjects.hpp"

Engine::Engine()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_SHOWN, &window, &renderer);
  SDL_SetWindowTitle(window, "Run And Gun Editor");
  atlas = IMG_LoadTexture(renderer, "./Assets/Atlas.png");
  collisionAtlas = IMG_LoadTexture(renderer, "./Assets/collision.png");
  pallete.init(atlas, collisionAtlas);
  textRenderer.init(renderer);
  SDL_Rect tileEditArea {0, 0, (windowWidth-160)/windowScale, windowHeight/windowScale};
  level.init(atlas, collisionAtlas, tileEditArea);
  screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth/windowScale, windowHeight/windowScale);
  std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
  SDL_StopTextInput();
}

void Engine::run()
{
  lastFrameTime = std::chrono::steady_clock::now();
  while (exit_game == false)
  {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    
    // Handle input
    input();
    
    // Update
    pallete.update();
    
    // Render
    SDL_SetRenderTarget(renderer, screenTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    
    SDL_RenderSetClipRect(renderer, &level.tileEditArea);
    level.draw(renderer, 0, 0, camera.x, camera.y, selectedLayer);
    SDL_RenderSetClipRect(renderer, NULL);
    pallete.draw(renderer, selectedLayer);
    
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    
    if (noInteractionsYet) textRenderer.draw(renderer, 
      "Run And Gun Level Editor\n"
      "Left Click: Place Tile\n"
      "Right Click: Erase Tile\n"
      "TAB: Open Pallete\n"
      "Middle Click: Drag Camera"
      , 4, 4, windowWidth - 8, 2);
    
    if (saveDialogueEnabled) 
    {
      SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
      SDL_Rect saveDialogueRect {0, windowHeight - 32, windowWidth, 32};
      SDL_RenderFillRect(renderer, &saveDialogueRect);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      textRenderer.draw(renderer, "Press Enter to Save", windowWidth / 2 - 16*strlen("Press Enter to Save")/2, windowHeight - 32, 16*strlen("Press Enter to Save"), 2);
      textRenderer.draw(renderer, levelName, 0, windowHeight - 16, windowWidth - 16, 2);
    }
    
    if (loadDialogueEnabled) 
    {
      SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
      SDL_Rect loadDialogueRect {0, windowHeight - 32, windowWidth, 32};
      SDL_RenderFillRect(renderer, &loadDialogueRect);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      textRenderer.draw(renderer, "Press Enter to Load", windowWidth / 2 - 16*strlen("Press Enter to Load")/2, windowHeight - 32, 16*strlen("Press Enter to Load"), 2);
      textRenderer.draw(renderer, levelName, 0, windowHeight - 16, windowWidth - 16, 2);
    }
    
    //render the layer number
    if (!noInteractionsYet)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
      char layer[10] = "layer: 0\0";
      std::string numbers = std::to_string(selectedLayer);
      strncpy(&layer[7], numbers.c_str(), 1);
      textRenderer.draw(renderer, layer, 0, 0, 11*16, 2);
    }
    
    std::this_thread::sleep_until(lastFrameTime + frameDuration);
    lastFrameTime = std::chrono::steady_clock::now();
    
    
    //std::cout << "Current FPS: " << 1.0 / std::chrono::duration<double>(lastFrameTime - now).count() << "\n";
    
    
    SDL_RenderPresent(renderer);
  }
}

void Engine::input()
{
  
  preMouseState = mouseState;
  preMousePos.x = mousePos.x;
  preMousePos.y = mousePos.y;
  mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
  mousePos.x = mousePos.x  / windowScale;
  mousePos.y = mousePos.y / windowScale;
  
  if (collideRect(level.tileEditArea, mousePos.x, mousePos.y))
  {
    if (mouseState & SDL_BUTTON(1)) {level.set((mousePos.x+camera.x)/16, (mousePos.y+camera.y)/16, pallete.selectedTile, selectedLayer); noInteractionsYet=false;};
    if (mouseState & SDL_BUTTON(3)) {level.set((mousePos.x+camera.x)/16, (mousePos.y+camera.y)/16, 0, selectedLayer); noInteractionsYet=false;};
    if (mouseState & SDL_BUTTON(2)) 
    {
      //Camera Movement
      camera.x -= mousePos.x - preMousePos.x;
      camera.y -= mousePos.y - preMousePos.y;
      noInteractionsYet=false;
    }
  }
  
  glm::vec2 position{mousePos.x, mousePos.y};
  
  
  pallete.mouseX = mousePos.x;
  pallete.mouseY = mousePos.y;
  
  int mouseRelLevelX = (int)((mousePos.x+camera.x)/(float)level.scale);
  int mouseRelLevelY = (int)((mousePos.y+camera.y)/(float)level.scale);
  
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      exit_game = true;
    }
    if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
      if (event.key.keysym.sym == SDLK_TAB) pallete.enabled = false;
    }
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
      
      if (event.key.keysym.sym == SDLK_TAB)
      {
        pallete.enabled = true;
        pallete.x = clamp(mousePos.x - (pallete.atlasWidth * pallete.scale) / 2, 0, windowWidth/windowScale - (pallete.atlasWidth * pallete.scale));
        pallete.y = clamp(mousePos.y - (pallete.atlasHeight * pallete.scale) / 2, 0, windowHeight/windowScale - (pallete.atlasHeight * pallete.scale));
      }
      
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        loadDialogueEnabled = false;
        saveDialogueEnabled = false;
        SDL_StopTextInput();
      }
      
      
      
      if (SDL_IsTextInputActive())
      {
        if (event.key.keysym.sym == SDLK_RETURN)
        {
          if (saveDialogueEnabled) {
            std::cout << levelName << " saved" << std::endl;
            level.save(levelName);
            saveDialogueEnabled = false;
          }
          else if (loadDialogueEnabled)
          {
            std::cout << levelName << " loaded" << std::endl;
            
            level.load(levelName);
            loadDialogueEnabled = false;
          }
          SDL_StopTextInput();
        }
        if (event.key.keysym.sym == SDLK_BACKSPACE)
        {
          int len = strlen(levelName);
          levelName[len-1] = '\0';
        }
      }
      
      //Safe to use keydown since textinput is disabled
      else
      {
        
        if (event.key.keysym.sym == SDLK_UP)
        {
          selectedLayer++;
        }
        if (event.key.keysym.sym == SDLK_DOWN)
        {
          selectedLayer--;
        }
        if (event.key.keysym.sym == SDLK_e)
        { 
          level.addPoint(LevelPoint{mouseRelLevelX, mouseRelLevelY, "bird", ""});
        }
        if (event.key.keysym.sym == SDLK_r)
        {
          level.removePoint(mouseRelLevelX, mouseRelLevelY);
        }
      }
      
      
      bool dialogueOpen = saveDialogueEnabled || loadDialogueEnabled;
      
      if (!dialogueOpen)
      {
        if (event.key.keysym.sym == SDLK_s)
        {
          saveDialogueEnabled = true;
          SDL_StartTextInput();
        }
        
        if (event.key.keysym.sym == SDLK_l)
        {
          loadDialogueEnabled = true;
          SDL_StartTextInput();
        }
      }
      
      
      
    }
    if (event.type == SDL_TEXTINPUT)
    {
      strncat(levelName, event.text.text, 255-strlen(levelName));
    }
    if (event.type == SDL_TEXTEDITING)
    {
      
    }
    
    
    
    
  }
}

Engine::~Engine()
{
  SDL_DestroyTexture(atlas);
  SDL_DestroyWindow(window);
  SDL_Quit();
}