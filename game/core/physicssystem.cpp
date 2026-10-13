#include "physicssystem.hpp"

#include <algorithm>
#include <cmath>
#include "entity.hpp"
#include "rect.hpp"

//I'm considering whether having position and hitbox being separate is best, but maybe it's fine.
//Each entity just will need to copy the hitbox position to the entity position which is used for the sprite and such
void handlePhysics(EntityManager &em, Level &level, float delta)
{
  for (int k = 0; k < em.physics.itemCount(); k++){
    int slotID = em.physics.getEntitySlotID(k);
    //uneeded but maybe who knows I may reconsider
    //if (!em.hitboxs.has(slotID) || !em.velocities.has(slotID)) continue;
    Physics& physics = em.physics.items[k];
    fRect& hitbox = em.hitboxs[slotID];
    glm::vec2& velocity = em.velocities[slotID];

    for (int i = 0; i < 2; i++){
      MoveCollision collision = move(hitbox, velocity*delta/2.0f, level, physics.collisionMask);
      hitbox = collision.position;
      if (collision.cancelVec.x != 0) velocity.x = 0;
      if (collision.cancelVec.y != 0) velocity.y = 0;
      if (collision.cancelVec.y > 0) physics.grounded = true;
      if (velocity.y > 0.0f) physics.grounded = false;
    }

    if ((physics.grounded && !physics.useAirFriction) || physics.useAirFriction)
      velocity.x = velocity.x - std::copysignf(std::min(physics.friction * delta, std::abs(velocity.x)), velocity.x);
    velocity.y += physics.gravity * delta;
  }
}
