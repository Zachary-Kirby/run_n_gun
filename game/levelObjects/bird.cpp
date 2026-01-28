#include "bird.hpp"
#include "engine.hpp"

//NEXT pass a subsystem to bird so it can use the player position and maybe check against all bullets?

Bird::Bird()
{
  this->engine = nullptr;
  active = false;
  this->sprite = Sprite();
}

Bird::Bird(const Bird &bird)
{
  engine = bird.engine;
  active = bird.active;
  hitbox = bird.hitbox;
  health = bird.health;
  sprite = Sprite(bird.sprite);
}

Bird &Bird::operator=(const Bird &other)
{
  this->engine = other.engine;
  active = other.active;
  this->sprite = other.sprite;
  this->hitbox = other.hitbox;
  health = other.health;
  return *this;
}

Bird::Bird(Engine *engine, Sprite sprite, glm::vec2 position)
{
  this->engine = engine;
  active = true;
  this->sprite.init(sprite);
  hitbox.x = position.x - hitbox.w / 2;
  hitbox.y = position.y - hitbox.h / 2;
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
  Engine& engine = *this->engine;
  
}

void Bird::draw(SDL_Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x-camera.x, hitbox.y-camera.y);
  sprite.draw(renderer);
}
