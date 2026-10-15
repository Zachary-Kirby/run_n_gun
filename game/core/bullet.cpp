#include "bullet.hpp"
#include <iostream>


void handleBullets(EntityManager &em, float delta)
{
  for (int k = 0; k < em.contactDamage.itemCount(); k++){
    int slotID = em.contactDamage.getEntitySlotID(k);
    //Assuming both lifetime and contact damage components exist, then peform bullet logic
    if (em.lifetime.has(slotID)){
      //A bullet has no death animation, so it is removed as soon as its health runs out
      if (em.health[slotID].health <= 0){
        em.entities[slotID].remove = true;
        em.contactDamage[slotID].damage = 0;
      }
      em.entities[slotID].position += em.velocities[slotID] * delta;
      em.hitboxs[slotID].x = em.entities[slotID].position.x;
      em.hitboxs[slotID].y = em.entities[slotID].position.y;
    }
  }
}

int createBullet(const Sprite sprite, EntityManager& em, float x, float y, float vx, float vy, DamageType damageType, DamageType recieveDamageType, float lifetime, int damage){
  int slotID = em.popSlot();
  if (slotID < 0)
    return slotID;

  em.entities[slotID].ID = em.nextID();
  em.entities[slotID].position = {x, y};
  em.sprites.insert(sprite, slotID);
  em.hitboxs.insert(fRect(x, y, 8, 8), slotID);
  em.velocities.insert({vx, vy}, slotID);
  em.contactDamage.insert({damage, damageType}, slotID);
  em.health.insert({1, 0, recieveDamageType}, slotID); 
  em.lifetime.insert(lifetime, slotID);
  return slotID;
}