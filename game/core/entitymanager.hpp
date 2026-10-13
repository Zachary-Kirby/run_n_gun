#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "entity.hpp"
#include "sprite.hpp"
#include "rect.hpp"
#include "birdaicomponent.hpp"
#include "wimdybirdaicomponent.hpp"
#include "sparseset.hpp"
#include "contactdamagecomponent.hpp"
#include "healthcomponent.hpp"
#include "bulletfrogaicomponent.hpp"
#include "physicscomponent.hpp"

#include <iostream>


#define MAXENT 200


//TODO figure out how to store different AI types ( I think the easiest way to treat has a as just existence in one of the other sets )
//TODO migrate bird to Entity manager.
//updating each AI is then as simple as creating a system for each AI component that loops through them.
//TODO figure out how I want to structure systems. Do I want many functions that take the entity manager? How much is just duplicate code for looping?

class EntityManager{
    int ID = 0;
    
  public:
    std::vector<Entity> entities;
    std::vector<int> freeSlots;

    SparseSet<Sprite> sprites{MAXENT};
    SparseSet<glm::vec2> velocities{MAXENT};
    SparseSet<BirdAI> birdAI{MAXENT};
    SparseSet<WimdyBirdAI> wimdyBirdAI{MAXENT};
    SparseSet<fRect> hitboxs{MAXENT};
    SparseSet<ContactDamage> contactDamage{MAXENT};
    SparseSet<Health> health{MAXENT};
    SparseSet<float> lifetime{MAXENT};
    SparseSet<BulletFrogAI> bulletFrogAI{MAXENT};
    SparseSet<Physics> physics{MAXENT};
    
    
    EntityManager()
    {
      entities.resize(MAXENT);
      for (int i = 0; i < MAXENT; i++)
      {
        freeSlots.push_back(i);
      }
    }
    
    void clear(){
      //remove every entity and component, used when reloading the level
      for (int i = 0; i < entities.size(); i++)
      {
        entities[i] = Entity{};
      }
      freeSlots.clear();
      for (int i = MAXENT-1; i >= 0; i--)
      {
        freeSlots.push_back(i);
      }
      sprites.clear();
      velocities.clear();
      birdAI.clear();
      wimdyBirdAI.clear();
      hitboxs.clear();
      contactDamage.clear();
      health.clear();
      lifetime.clear();
      bulletFrogAI.clear();
      physics.clear();
    }

    int nextID(){
      return ID++;
    }
    
    int popSlot(){
      //function that is supposed to find a free slot ID for a new entity.
      if (freeSlots.size() <= 0) return -1;
      int slot = freeSlots[freeSlots.size()-1]; freeSlots.pop_back();
      return slot;
    }
    
    void handleRemove(){
      for (int i = 0; i < entities.size(); i++)
      {
        Entity& entity = entities[i];
        if (entity.remove){
          freeSlots.push_back(i);
          sprites.remove(i);
          velocities.remove(i);
          birdAI.remove(i);
          wimdyBirdAI.remove(i);
          hitboxs.remove(i);
          contactDamage.remove(i);
          health.remove(i);
          lifetime.remove(i);
          bulletFrogAI.remove(i);
          physics.remove(i);
          entity.remove = false;
        }
      }
    }
};