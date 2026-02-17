#include "pole.hpp"
//#include "renderer.hpp"

#include <cmath>

void Pole::update(float delta)
{
  
}

void RenderCircle(Renderer* renderer, float x, float y, float radius, float z=0.0f);

void Pole::draw(Renderer* renderer, glm::vec2 camera)
{
  float endX = pivotX + std::cos(angle)*length;
  float endY = pivotY + std::sin(angle)*length;
  //debug code
  
  RenderCircle(renderer, pivotX-camera.x, pivotY-camera.y, 4.0f);
  RenderCircle(renderer, endX-camera.x, endY-camera.y, 4.0f);
}
