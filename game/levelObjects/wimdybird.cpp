#include "wimdybird.hpp"
#include "wimdybirdaicomponent.hpp"
#include "contactdamagecomponent.hpp"
#include <cmath>

void handleWimdyBirdAI(EntityManager& em, Animations& spriteAnimations, fRect playerHitbox, float time, float delta,
  glm::vec2& playerPush, Sound& flapSound, Sound& hurtSound, Sound& deathSound)
{
  for (int k = 0; k < em.wimdyBirdAI.itemCount(); k++)
  {
    int slotID = em.wimdyBirdAI.getEntitySlotID(k);
    Entity& entity = em.entities[slotID];
    Sprite& sprite = em.sprites[slotID];
    fRect& hitbox = em.hitboxs[slotID];
    WimdyBirdAI& ai = em.wimdyBirdAI[slotID];
    int health = em.health[slotID].health;

    if (health <= 0)
    {
      //TODO play a death animation and only then remove
      deathSound.play();
      entity.remove = true;
      continue;
    }
    if (health < ai.lastHealth) hurtSound.play(); //TODO also initiate a flicker when hit
    ai.lastHealth = health;

    ai.facingRight = playerHitbox.center().x > hitbox.center().x;

    //blow player when close enough
    if (std::abs(playerHitbox.centerX() - hitbox.centerX()) < 128.0f && std::abs(playerHitbox.centerY() - hitbox.centerY()) < 16.0f)
    {
      ai.flapping = true;
      float blowDir = playerHitbox.centerX() - hitbox.centerX();
      playerPush.x += std::copysignf(1.0f, blowDir) * 1650.0f * delta;
    }
    else
    {
      ai.flapping = false;
    }

    ai.flapTimer -= delta;
    if (ai.flapTimer <= 0.0f)
    {
      ai.flapTimer = 40.0f/1000.0f;
      if (ai.flapping) flapSound.play();
    }

    if (ai.flapping)
      sprite.setDefinition(spriteAnimations.getAnimationFrame("WimdyBird_Flap", time, ai.facingRight));
    else
      sprite.setDefinition(spriteAnimations.getFrame("WimdyBird_Idle", 0, ai.facingRight));

    entity.position.x = hitbox.x;
    entity.position.y = hitbox.y;
  }
}

int spawnWimdyBird(EntityManager& em, Sprite sprite, float x, float y)
{
  int slotID = em.popSlot();
  if (slotID < 0)
    return slotID;

  em.entities[slotID].ID = em.nextID();
  em.entities[slotID].position = {x, y};
  em.sprites.insert(sprite, slotID);
  em.hitboxs.insert(fRect(x, y, sprite.src.w, sprite.src.h), slotID);
  WimdyBirdAI ai;
  ai.lastHealth = 3;
  em.wimdyBirdAI.insert(ai, slotID);
  em.contactDamage.insert({4}, slotID);
  em.health.insert({3, 0, DamageType::Player}, slotID);
  return slotID;
}
