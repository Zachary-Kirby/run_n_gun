#include "bird.hpp"
#include "engine.hpp"
#include <cmath>

glm::vec2 swoop(float startX, float startY, float targetX, float targetY, float t)
{
  float directionX = std::copysignf(1.0f, targetX - startX);
  float x = std::lerp(std::lerp(startX, startX-directionX*100, t), std::lerp(startX, targetX, t), t);
  float y = std::lerp(std::lerp(startY, targetY+40, t), std::lerp(targetY, targetY, t), t);
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

void Bird::update(float delta)
{
  if (!active) return;
  for (auto it = engine->bullets.begin(); it != engine->bullets.end(); it++)
  {
    Bullet& bullet = *it;
    if (bullet.active && hitbox.collide(bullet.hitbox))
    {
      health--;
      bullet.active = false;
      if (health <= 0)
      {
        state = BirdState::DEAD;
        active = false; //TODO temporary until death animation is added
      }
    }
  }
  
  Player& player = engine->player;
  bool notAtHeight = swoopStartPos.y < hitbox.centerY();
  int direction = 1;
  switch (state)
  {
  case BirdState::STATIC:
    if (std::abs(player.hitbox.centerX() - hitbox.centerX()) < 80.0f)
    {
      swoopProgress = 0.0f;
      swoopTargetPos = player.hitbox.center();
      state = BirdState::SWOOPLOW;
      swoopStartPos = hitbox.center();
    }
    break;
  case BirdState::SWOOPLOW:
    swoopProgress += delta;
    hitbox = swoop(swoopStartPos.x, swoopStartPos.y, swoopTargetPos.x, swoopTargetPos.y, swoopProgress);
    //check if bird got so far past the player on the other side of the start pos
    if (swoopProgress > 1.4f)
    {
      state = BirdState::RECOVER;
    }
    break;
  case BirdState::RECOVER:
    
    if (swoopTargetPos.y < swoopStartPos.y) {notAtHeight = swoopStartPos.y > hitbox.centerY(); direction = -1;}
    if (notAtHeight)
    {
      hitbox.x += std::copysignf(160.0f, swoopTargetPos.x - swoopStartPos.x) * delta;
      hitbox.y -= 320.0f * direction * delta;
    } else
    {
      hitbox.y = swoopStartPos.y - hitbox.h/2.0f;
      state = BirdState::FOLLOW;
    }
    break;
  case BirdState::FOLLOW:
    hitbox.x += std::copysignf(320.0f, player.hitbox.centerX() - hitbox.centerX()) * delta;
    if (std::abs(player.hitbox.centerX() - hitbox.centerX()) < 80.0f )
    {
      swoopProgress = 0.0f;
      swoopTargetPos = player.hitbox.center();
      state = BirdState::SWOOPLOW;
      swoopStartPos = hitbox.center();
    }
    break;
  default:
    break;
  }
}

void Bird::draw(Renderer *renderer, glm::vec2 camera)
{
  sprite.setPostion(hitbox.x-camera.x, hitbox.y-camera.y);
  sprite.draw(renderer);
}
