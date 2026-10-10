#include "lifetimesystem.hpp"

void handleLifetime(EntityManager& em, float delta){
  for (int k = 0; k < em.lifetime.itemCount(); k++){
    em.lifetime.items[k] -= delta;
    if (em.lifetime.items[k] < 0.0f){
      em.entities[em.lifetime.getEntitySlotID(k)].remove = true;
    }
  }
}
