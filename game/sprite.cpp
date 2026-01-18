#include "sprite.hpp"

Sprite::Sprite()
{
  src = {0, 0, 0, 0};
  dst = {0, 0, 0, 0};
  atlas = nullptr;
  scale = 1;
}

Sprite::Sprite(Sprite &other)
{
  src = other.src;
  dst = other.dst;
  atlas = other.atlas;
  scale = other.scale;
}

Sprite::Sprite(SDL_Texture *atlas, int x, int y, int w, int h, int scale = 1) : atlas(atlas), src(x, y, w, h), dst(0, 0, w * scale, h * scale)
{
  
}

void Sprite::init(Sprite other)
{
  src = other.src;
  dst = other.dst;
  atlas = other.atlas;
  scale = other.scale;
}

void Sprite::setPostion(int x, int y)
{
  dst.x = x;
  dst.y = y;
}

void Sprite::draw(SDL_Renderer *renderer)
{
  SDL_RenderCopy(renderer, atlas, &src, &dst);
}
