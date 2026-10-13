#include "bulletfrog.hpp"
#include "bulletfrogaicomponent.hpp"
#include "bullet.hpp"
#include "entity.hpp"
#include "damagetypes.hpp"
#include "healthcomponent.hpp"
#include "glm/vec2.hpp"
#include "physicscomponent.hpp"

#include <cmath>

void handleFrogAI(EntityManager& em, Animations& animations, float playerX, float delta){
  const float jumpVelocityY = 90.5f*4.0f;
  const float jumpVelocityX = 90.5f*1.0f;

  for (int k = 0; k < em.bulletFrogAI.itemCount(); k++){
    int slotID = em.bulletFrogAI.getEntitySlotID(k);
    Entity& entity = em.entities[slotID];
    BulletFrogAI& frogAI = em.bulletFrogAI.items[k];
    Health& health = em.health[slotID];
    glm::vec2& velocity = em.velocities[slotID];
    Physics& physics = em.physics[slotID];
    fRect& hitbox = em.hitboxs[slotID];
    Sprite& sprite = em.sprites[slotID];
    
    if (health.health <= 0)
      entity.remove = true;
    
    if (!physics.grounded) {
      if (velocity.y > 0) sprite.setDefinition(animations.getFrame("BulletFrog_Down", 0, frogAI.lookingRight));
      else sprite.setDefinition(animations.getFrame("BulletFrog_Up", 0, frogAI.lookingRight));

    } else {
      if (frogAI.prepareShotTimer > 0.0f){
        sprite.setDefinition(animations.getFrame("BulletFrog_Shoot", 0, frogAI.lookingRight));
      } else{
        sprite.setDefinition(animations.getFrame("BulletFrog_Idle", 0, frogAI.lookingRight));
      }
    }
    
    float xdir = playerX-em.entities[slotID].position.x;
    if ( std::abs(xdir) > frogAI.range) continue;
    
    frogAI.lookingRight = xdir > 0;
    
    float xv = -1;
    if (frogAI.lookingRight)
      xv = 1.0f;
    if (frogAI.actionTimer >= frogAI.actionTime && frogAI.prepareShotTimer <= 0.0f){
      if (std::abs(xdir) <= frogAI.range*0.33f && physics.grounded){
        velocity.y = -jumpVelocityY;
        velocity.x = xv * jumpVelocityX;
        physics.grounded = false;
      } else{
        frogAI.prepareShotTimer = 1.5f;
      }
      frogAI.actionTimer = 0.0f;
    }
    
    if (frogAI.prepareShotTimer > 0.0f && frogAI.prepareShotTimer < 1.0f){
      createBullet(animations.getFrame("Bullet_Bullet", 0), em, hitbox.centerX(), hitbox.centerY(), xv*frogAI.range*2.0f, 0.0f, DamageType::Enemy, DamageType::Player, 4.0f);
      frogAI.prepareShotTimer = 0.0f;
    }

    
    entity.position.x = hitbox.x;
    entity.position.y = hitbox.y;

    frogAI.actionTimer += delta;
    frogAI.prepareShotTimer -= delta;
  }
}

int createFrog(EntityManager& em, Animations& animations, float x, float y){
  int slotID = em.popSlot();
  if (slotID < 0)
    return slotID;
  em.entities[slotID].ID = em.nextID(); //not sure if I actually need this
  em.entities[slotID].position = {x, y};
  em.sprites.insert(animations.getFrame("BulletFrog_Idle", 0), slotID);
  em.hitboxs.insert(fRect(x, y, 16, 16), slotID);
  
  em.bulletFrogAI.insert({}, slotID);
  em.velocities.insert({0.0f, 0.0f}, slotID);
  em.physics.insert({800.0f, 800.0f}, slotID);

  em.contactDamage.insert({4}, slotID);
  em.health.insert({5, 0, DamageType::Player}, slotID);
  return slotID;
}