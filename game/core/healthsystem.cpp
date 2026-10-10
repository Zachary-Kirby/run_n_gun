#include "healthsystem.hpp"

void handleHealth(EntityManager &em)
{
  for (int k = 0; k < em.health.itemCount(); k++){
    int slotID = em.health.getEntitySlotID(k);
    Health& health = em.health.items[k];
    health.health -= health.pendingDamage;
    health.pendingDamage = 0;
    if (health.health <= 0){
      em.entities[slotID].remove = true;
    }
  }
}
