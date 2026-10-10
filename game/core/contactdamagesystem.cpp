#include "contactdamagesystem.hpp"
#include "contactdamagecomponent.hpp"

//TODO Unsure if damage sources care or if damage sinks care about finding collisions
void contactDamageSystem(EntityManager& em, fRect& playerHitbox, int& playerDamage)
{
  for (int k = 0; k < em.contactDamage.itemCount(); k++){
    int slotID = em.contactDamage.getEntitySlotID(k);
    fRect& hitbox = em.hitboxs[slotID];
    ContactDamage& contactDamage = em.contactDamage.items[k];
    //The player is a special case until it is integrated into the entity manager
    if (contactDamage.damageType == DamageType::Enemy && hitbox.collide(playerHitbox)){
      playerDamage = contactDamage.damage;//TODO integrate player more
    }
    //Loop through everything that may recieve this damage type. Because every damage source is also checked
    //against every damage sink, two entities with matching types hurt each other and both are removed later
    //by the health system.
    for (int kk = 0; kk < em.health.itemCount(); kk++){
      if (em.health.items[kk].recievableDamageType != contactDamage.damageType) continue;
      int ii = em.health.getEntitySlotID(kk);
      if (ii == slotID) continue; //don't hurt yourself
      if (hitbox.collide(em.hitboxs[ii])){
        em.health.items[kk].pendingDamage += contactDamage.damage;
      }
    }
  }
}