#include "bird.hpp"

Bird::Bird()
{
}

void Bird::init(Sprite sprite, glm::vec2 position)
{
  
  active = true;
  this->sprite.init(sprite);
  hitbox.x = position.x - hitbox.w / 2;
  hitbox.y = position.y - hitbox.h / 2;
}

void Bird::update(glm::vec2 target)
{
  if (!active) return;
  
}

void Bird::draw(SDL_Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x-camera.x, hitbox.y-camera.y);
  sprite.draw(renderer);
}
