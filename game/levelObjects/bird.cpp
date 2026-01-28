#include "bird.hpp"
#include "engine.hpp"
#include <cmath>

//NEXT pass a subsystem to bird so it can use the player position and maybe check against all bullets?


/* Reference python code for swoop path
target_x = 20
target_y = -20

def lerp(x1, x2, t):
    return (1-t)*x1 + t*x2

def swoop(x):
    midx = target_x / 2
    midy = target_y*3 / 3
    
    t = x/target_x
    return lerp(lerp(0, midy, t*4), lerp(midy, target_y, t), t)
*/

//float lerp(float a, float b, float t)
//{
//  return (1-t)*a + t*b;
//}


glm::vec2 swoop(float startX, float startY, float targetX, float targetY, float t)
{
  /*
  float relTargetX = std::abs(targetX - startX);
  float relTargetY = targetY - startY;
  float relX = std::abs(x - startX);
  float midx = relTargetX / 2.0f;
  float midy = relTargetY / 2.0f;
  float t = relX/relTargetX;
  return startY+lerp(lerp(0.0f, midy, t), lerp(midy, relTargetY, t), t);
  */
  float x = std::lerp(std::lerp(startX, startX, t), std::lerp(startX, targetX, t), t);
  float y = std::lerp(std::lerp(startY, targetY, t), std::lerp(targetY, targetY, t), t);
  return {x, y};
}
  
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

void Bird::update()
{
  if (!active) return;
  Player& player = engine->player;
  switch (state)
  {
  case BirdState::STATIC:
    if (std::abs(player.hitbox.centerX() - hitbox.centerX()) < 80)
    {
      swoopProgress = 0.0f;
      swoopTargetPos = player.hitbox.center();
      state = BirdState::SWOOPLOW;
      swoopStartPos = hitbox.center();
    }
    break;
  case BirdState::SWOOPLOW:
    swoopProgress += 0.01f;
    hitbox = swoop(swoopStartPos.x, swoopStartPos.y, swoopTargetPos.x, swoopTargetPos.y, swoopProgress);
    //check if bird got so far past the player on the other side of the start pos
    if (std::abs(swoopTargetPos.x - hitbox.x) > 80.0f)
    {
      state = BirdState::RECOVER;
    }
    break;
  case BirdState::RECOVER:
    if (swoopStartPos.y < hitbox.centerY())
    {
      hitbox.y -= 1.0f;
    } else
    {
      hitbox.y = swoopStartPos.y - hitbox.h/2.0f;
      state = BirdState::STATIC;
    }
    break;
  default:
    break;
  }
}

void Bird::draw(SDL_Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x-camera.x, hitbox.y-camera.y);
  sprite.draw(renderer);
}
