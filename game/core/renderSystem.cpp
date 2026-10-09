#include "renderSystem.hpp"
#include "entity.hpp"


void renderSprites(EntityManager& em, Renderer *renderer, int gameplayDrawWidth, glm::vec2 camera, float time)
{
  for (int i = 0; i < em.sprites.itemCount(); i++)
  {
    Entity& entity = em.entities[em.sprites.getEntitySlotID(i)];
    //TODO use actual hitbox for culling
    if (entity.position.x - camera.x < -16.0f || entity.position.x - camera.x > gameplayDrawWidth + 16.0f)
      continue;
    em.sprites.items[i].setPostion(entity.position[0] - camera[0], entity.position[1] - camera[1]);
    em.sprites.items[i].draw(renderer);
  }
}