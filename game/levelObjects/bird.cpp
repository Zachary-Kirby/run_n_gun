#include "bird.hpp"
#include <cmath>
#include "birdaicomponent.hpp"
#include "sprite.hpp"
#include "contactdamagecomponent.hpp"


glm::vec2 swoop(float startX, float startY, float targetX, float targetY, float t)
{
  float directionX = std::copysignf(1.0f, targetX - startX);
  float x = std::lerp(std::lerp(startX, startX-directionX*100, t), std::lerp(startX, targetX, t), t);
  float y = std::lerp(std::lerp(startY, targetY+40, t), std::lerp(targetY, targetY, t), t);
  return {x, y};
}

//TODO handle player hitbox damage
//handle bullets hitting bird
//requires explodiate sound and state dead
void handleBirdAI(EntityManager& em, Animations& spriteAnimations, fRect playerHitbox, float time, float delta, Sound& deathSound)
{
  
  for (int k = 0; k < em.birdAI.itemCount(); k++)
  {
    int slotID = em.birdAI.getEntitySlotID(k);
    Entity& entity = em.entities[slotID];
    Sprite& sprite = em.sprites[slotID];
    fRect& hitbox = em.hitboxs[slotID];
    BirdAI& birdAI = em.birdAI[slotID];

    if (em.health[slotID].health <= 0) birdAI.state = BirdState::DEAD;

    bool notAtHeight = birdAI.swoopStartPos.y < hitbox.centerY();
    int direction = 1;
    switch (birdAI.state)
    {
    case BirdState::DEAD:
      //TODO play a death animation and only then remove
      entity.remove = true;
      break;
    case BirdState::STATIC:
      if (std::abs(playerHitbox.centerX() - hitbox.centerX()) < 80.0f)
      {
        birdAI.swoopProgress = 0.0f;
        birdAI.swoopTargetPos = playerHitbox.center();
        birdAI.state = BirdState::SWOOPLOW;
        birdAI.swoopStartPos = hitbox.center();
      }
      break;
    case BirdState::SWOOPLOW:
    case BirdState::SWOOPHIGH:
      birdAI.swoopProgress += delta;
      hitbox = swoop(birdAI.swoopStartPos.x, birdAI.swoopStartPos.y, birdAI.swoopTargetPos.x, birdAI.swoopTargetPos.y, birdAI.swoopProgress);
      //check if bird got so far past the player on the other side of the start pos
      if (birdAI.swoopProgress > 1.4f)
      {
        birdAI.state = BirdState::RECOVER;
      }
      break;
    case BirdState::RECOVER:
      
      if (birdAI.swoopTargetPos.y < birdAI.swoopStartPos.y) {notAtHeight = birdAI.swoopStartPos.y > hitbox.centerY(); direction = -1;}
      if (notAtHeight)
      {
        hitbox.x += std::copysignf(160.0f, birdAI.swoopTargetPos.x - birdAI.swoopStartPos.x) * delta;
        hitbox.y -= 320.0f * direction * delta;
      } else
      {
        hitbox.y = birdAI.swoopStartPos.y - hitbox.h/2.0f;
        birdAI.state = BirdState::FOLLOW;
      }
      break;
    case BirdState::FOLLOW:
      hitbox.x += std::copysignf(160.0f, playerHitbox.centerX() - hitbox.centerX()) * delta;
      if (std::abs(playerHitbox.centerX() - hitbox.centerX()) < 80.0f )
      {
        birdAI.swoopProgress = 0.0f;
        birdAI.swoopTargetPos = playerHitbox.center();
        birdAI.state = BirdState::SWOOPLOW;
        birdAI.swoopStartPos = hitbox.center();
      }
      break;
    default:
      break;
    }
  
  //Animation
  const char* stateNames[] = {"STATIC", "SWOOPLOW", "SWOOPHIGH", "RECOVER", "FOLLOW", "DEAD"};

    switch (birdAI.state)
    {
    case BirdState::STATIC:
    case BirdState::FOLLOW:
        sprite.setDefinition(spriteAnimations.getAnimationFrame("Bird_Flying", time, playerHitbox.centerX() > hitbox.centerX(), false));
    break;
    case BirdState::RECOVER:
    case BirdState::SWOOPLOW:
    case BirdState::SWOOPHIGH:
        sprite.setDefinition(spriteAnimations.getFrame("Bird_Flying", 3, birdAI.swoopTargetPos.x > birdAI.swoopStartPos.x, false));
      break;
    case BirdState::DEAD:
    default:
      //NOTE that if I move the bird in an animation that the contact damage component needs to be removed
      //else the bird can hurt the player even when dead.
      deathSound.play();
      break; //TODO add death animation
    }
    entity.position.x = hitbox.x;
    entity.position.y = hitbox.y;
  }
}

int spawnBird(EntityManager &em, float x, float y)
{ 
  int slotID = em.popSlot();
  if (slotID < 0)
    return slotID;

  em.entities[slotID].ID = em.nextID(); //not sure if I actually need this
  em.entities[slotID].position = {x, y};
  em.sprites.insert(Sprite(0, 0, 8, 8, 1), slotID);
  em.hitboxs.insert(fRect(x, y, 8, 8), slotID);
  em.birdAI.insert(BirdAI{}, slotID);
  em.contactDamage.insert({4}, slotID);
  em.health.insert({1, 0, DamageType::Player}, slotID);
  return slotID;
};
