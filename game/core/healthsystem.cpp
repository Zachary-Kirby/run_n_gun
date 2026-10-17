#include "healthsystem.hpp"

//Only applies damage. What happens at zero health (death animation, removal, etc.) is up to each entity's own AI/system.
void handleHealth(EntityManager &em)
{
  for (int k = 0; k < em.health.itemCount(); k++){
    Health& health = em.health.items[k];
    health.preHealth = health.health;
    health.health -= health.pendingDamage;
    health.pendingDamage = 0;
  }
}
