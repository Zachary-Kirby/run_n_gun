#include "sprite.hpp"
#include <glm/vec2.hpp>
#include "renderer.hpp"
#include <string>

class Particle
{
public:
  glm::vec2 velocity;
  glm::vec2 position;
  Sprite sprite;
  std::string animationName;
  float lifetime;
  
  Particle(glm::vec2 initialPosition, std::string animationName, float lifetime, glm::vec2 startVelocity) : animationName(animationName), lifetime(lifetime), velocity(startVelocity) 
  {
    position = initialPosition;
  }
  
  void update(float delta) 
  {
    lifetime -= delta;
    position += velocity*delta;
  }
  
  void draw(Renderer* renderer, glm::vec2 camera, float time) 
  {
    sprite.setDefinition(renderer->spriteAnimations.getAnimationFrame(animationName, time));
    sprite.dst.w = sprite.src.w;
    sprite.dst.h = sprite.src.h;
    sprite.setPostion(position.x-camera.x, position.y-camera.y);
    sprite.draw(renderer);
  }
};