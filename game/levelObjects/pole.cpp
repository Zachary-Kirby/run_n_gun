#include "pole.hpp"
#include "engine.hpp"
//#include "renderer.hpp"
/*
 * Generally this is the first system that interacts between three systems indepth.
 * meaning that either the code is in the main loop or I need something more special...
 * A player move needs to know that a pole platform is something that is can collide with?
 * I could try the naive approach of just moving the player up if it is colliding
 * */

#include <cmath>

void Pole::update(float delta)
{
  
  auto preEndPoint = glm::vec2(pivotX + std::cos(angle)*length, pivotY + std::sin(angle)*length);
  
  angle += angularVelocity*delta;
  
  auto endPoint = glm::vec2(pivotX + std::cos(angle)*length, pivotY + std::sin(angle)*length);
  
  if (engine->player.hitbox.collide(endPoint) && engine->player.velocity.y >= 0.0f)
  {
    if (angle+3.14159f/2.0f>0) angularVelocity+=std::sin(angle+3.14159f/2.0f)*200.0f*delta;
    if (angle+3.14159f/2.0f<0) angularVelocity+=std::sin(angle+3.14159f/2.0f)*200.0f*delta;
    angularVelocity += -engine->player.velocity.x/length*delta*60.0f;
    if (abs(angle+3.14159f/2.0f) < 3.14159f/8.0f)
    {
      angularVelocity += -engine->player.velocity.x/length*delta*60.0f;
      angularVelocity *= 1.0f;
    }
    angularVelocity = std::min(std::max(angularVelocity, -3.14159f), 3.14159f);
    engine->player.velocity.y = std::min(engine->player.velocity.y, 0.0f);
    engine->player.hitbox.y = endPoint.y-engine->player.hitbox.h;
    engine->player.grounded = true;
  }
  else
  {
    angularVelocity -= std::sin(angle+3.14159f/2.0f)*delta*200.0f;
    angularVelocity = angularVelocity * 0.99;
  }
  endPoint = glm::vec2(pivotX + std::cos(angle)*length, pivotY + std::sin(angle)*length);
  auto dif = endPoint - preEndPoint;
  if (
    engine->player.hitbox.x+ engine->player.hitbox.w>endPoint.x && 
    engine->player.hitbox.x < endPoint.x && 
    engine->player.hitbox.y < endPoint.y && 
    engine->player.hitbox.y > endPoint.y-18)
  {
    if (engine->player.velocity.y > 0)
    {
      engine->player.grounded = true;
    }
    engine->player.hitbox.x += dif.x;
    engine->player.hitbox.y += dif.y;
  }
}

//void RenderCircle(Renderer* renderer, float x, float y, float radius, float z=0.0f);

void Pole::draw(Renderer* renderer, glm::vec2 camera)
{
  float endX = pivotX + std::cos(angle)*length;
  float endY = pivotY + std::sin(angle)*length;
  //debug code
  //SDL_FRect src{5*8, 8, 8, 8};
  SDL_FRect dst{pivotX-camera.x, pivotY-camera.y, 8,length};
  RenderCopy(renderer, &src,  &dst, 0.0f, angle-3.14159f/2.0f, -0.5, 0);
  //RenderCircle(renderer, pivotX-camera.x, pivotY-camera.y, 4.0f);
  //RenderCircle(renderer, endX-camera.x, endY-camera.y, 4.0f);
}
