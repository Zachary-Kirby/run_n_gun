#include "sprite.hpp"
#include "renderer.hpp"

Sprite::Sprite()
{
  src = {0, 0, 0, 0};
  dst = {0, 0, 0, 0};
  scale = 1;
}

Sprite::Sprite(const Sprite &other) :
src(other.src), 
dst(other.dst), 
scale(other.scale) 
{}



Sprite &Sprite::operator=(const Sprite &other)
{
  if (this != &other)
  {
    src = other.src;
    dst = other.dst;
    scale = other.scale;
  }
  return *this;
}

Sprite::Sprite(int x, int y, int w, int h, int scale = 1) : src(x, y, w, h), dst(0, 0, w * scale, h * scale)
{
}

void Sprite::init(Sprite other)
{
  src = other.src;
  dst = other.dst;
  scale = other.scale;
}

void Sprite::setPostion(int x, int y)
{
  dst.x = x;
  dst.y = y;
}

void Sprite::draw(Renderer *renderer)
{
  RenderCopy(renderer, &src, &dst);
}
