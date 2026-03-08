#include "wimdybird.hpp"
#include "engine.hpp"
#include "glm/vec2.hpp"
#include  <cmath>




WimdyBird::WimdyBird(Engine* engine, Sprite sprite, glm::vec2 pos) : engine(engine), sprite(sprite)
{
  hitbox = fRect(pos.x, pos.y, sprite.src.w, sprite.src.h);
}

WimdyBird::WimdyBird(const WimdyBird &other)
{
  engine = other.engine;
  sprite = other.sprite;
  hitbox = other.hitbox;
  active = other.active;
  facingRight = other.facingRight;
  flapping = other.flapping;
  health = other.health;
}

void WimdyBird::update(float delta)
{
  Player& player = engine->player;
  if (hitbox.collide(player.hitbox))
  {
    player.dealDamage();
  }
  for (auto it = engine->bullets.begin(); it != engine->bullets.end(); it++)
  {
    Bullet& bullet = *it;
    if (bullet.active && hitbox.collide(bullet.hitbox))
    {
      health--;
      bullet.active = false;
      if (health <= 0)
      {
        active = false; //TODO temporary until death animation is added
        engine->explodeiateSound.play();
      }
      else
      {
        engine->ooSound.play();
      }
    }
  }
  
  if (player.hitbox.center().x > hitbox.center().x) facingRight = true;
  else facingRight = false;
  
  //blow player when close enough
  if (std::abs(player.hitbox.centerX() - hitbox.centerX()) < 128.0f && std::abs(player.hitbox.centerY() - hitbox.centerY()) < 16.0f)
  {
    flapping = true;
    float blowDir = player.hitbox.centerX() - hitbox.centerX();
    player.velocity.x += std::copysignf(1.0f, blowDir) * 1650.0f * delta;
  }
  else
  {
    flapping = false;
  }
  
  flapTimer -= delta;
  if (flapTimer <= 0.0f)
  {
    flapTimer = 40.0f/1000.0f;
    if (flapping)
    {
      engine->shortNoiseSound.play();
    }
  }
}

void WimdyBird::draw(Renderer *renderer, glm::vec2 camera, float second)
{
  if (!active) return;
  if (flapping)
  {
    std::cout << "flapping: " << flapping << std::endl;
    sprite.setDefinition(renderer->spriteAnimations.getAnimationFrame("WimdyBird_Flap", second, facingRight));
  }
  else
  {
    sprite.setDefinition(renderer->spriteAnimations.getFrame("WimdyBird_Idle", 0, facingRight));
  }
  sprite.setPostion(hitbox.x - camera.x, hitbox.y - camera.y);
  sprite.draw(renderer);
}
