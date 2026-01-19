#include "text.hpp"

#include <SDL_image.h>

void Text::init(SDL_Renderer *renderer)
{
  atlas = IMG_LoadTexture(renderer, "./Assets/text.png");
}

void Text::draw(SDL_Renderer *renderer, const char *text, int x, int y, int w, int scale = 1)
{
  int i=0;
  SDL_Rect srcRect {0, 0, 8, 8};
  SDL_Rect dstRect {x, y, 8*scale, 8*scale};
  while (text[i] != '\0')
  {
    int character = (int)(text[i]);
    srcRect.x = (character % 32)*8;
    srcRect.y = (character / 32)*8;
    SDL_RenderCopy(renderer, atlas, &srcRect, &dstRect);
    if (text[i] == '\n') {dstRect.y+=8*scale; dstRect.x=x;} else dstRect.x+=8*scale;
    if (dstRect.x >= x+w) {dstRect.x = x; dstRect.y+=8*scale;}
    i++;
  }
}
