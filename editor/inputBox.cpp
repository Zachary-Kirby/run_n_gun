#include "inputBox.hpp"

void InputBox::init(const char* label, int x, int y, int width, int height, Text *textRenderer)
{
  this->textRenderer = textRenderer;
  this->hitbox = Rect(x, y, width, height);
  this->active = true;
  this->selected = false;
  this->text = "";
  this->label = label;
}

void InputBox::draw(SDL_Renderer *renderer)
{
  if (active) {
    SDL_Rect clipRect{hitbox.x, hitbox.y, hitbox.w, hitbox.h};
    SDL_RenderSetClipRect(renderer, &clipRect);
    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
    SDL_RenderDrawRect(renderer, &clipRect);
    textRenderer->draw(renderer, label.c_str(), hitbox.x+hitbox.w/2-(label.length()*8/2), hitbox.y, hitbox.w, 1);
    textRenderer->draw(renderer, text.c_str(), hitbox.x, hitbox.y+hitbox.h/2, hitbox.w, 1);
    SDL_RenderSetClipRect(renderer, NULL);
  }
}

void InputBox::handleEvent(SDL_Event event)
{
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    auto mousePos = glm::vec2(event.button.x, event.button.y);
    bool clicked = hitbox.collide(mousePos);
    if (clicked)
    {
      selected = true;
      SDL_StartTextInput();
    }
    else
    {
      selected = false;
    }
  } 
  if (selected)
  {
    if (event.type == SDL_TEXTINPUT) {
      text += event.text.text;
    }
    
    if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty())
      {
        text.pop_back();
      }
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        selected = false;
      }
    }
  }
}