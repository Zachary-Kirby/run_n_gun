#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "entity.hpp"
#include "sprite.hpp"

#include <iostream>

#define MAXENT 200

template <typename T>
class SparseSet
{
  /*
    Map an integer to a compact array of the items
  */
  std::vector<int> indices; // sparse mappings to the dense indirection layer
  std::vector<int> keys; // maps back to the corresponding index, I use this for knowing how to swap the real data in memory
  int itemsTop = 0;
  size_t size;
  
  public:
  std::vector<T> items;
  
  SparseSet() { resize(MAXENT) };
  
  void resize(size_t s)
  {
    size = s;
    indices.resize(size);
    indices.assign(size, -1);
    keys.resize(size);
    items.resize(size);
  };
  
  int itemCount() {
    return itemsTop;
  }
  
  int getEntityID(int k){
    return keys[k];
  }
  
  int insert(T item, int i)
  {
    // insert the item to the sparse set and return, on failure return -1
    // the idea being outside the datastructure something keeps track of free slots
    //slot collision is -1, no free space is -2
    if (indices[i] != -1) return -1;
    if (itemsTop >= size) return -2;
    indices[i] = itemsTop;
    keys[itemsTop] = i;
    items[itemsTop] = item;
    itemsTop++;
    return 0;
  };
  
  int remove(int i)
  {
    //remove the item at index i
    
    //remove index
    int keyIndex = indices[i];
    indices[i] = -1;
    if (keyIndex == -1) return -1;
    if (keyIndex == itemsTop-1) {itemsTop--; return 0;} //if the items was the top one no swap is needed
    
    //swap with last item (nobody cares about junk information so only partial swap is needed)
    items[keyIndex] = items[itemsTop-1];
    keys[keyIndex] = keys[itemsTop-1];
    
    //update index for swapee to current key index
    indices[keys[keyIndex]] = keyIndex;
    
    itemsTop--;
    
    return 0;
  };
  
  T& operator[] (int i)
  {
    return items[indices[i]];
  };
  
  bool has(int i){
    //return whether that entity ID has this component
    return indices[i] != -1;
  }
};

//TODO figure out how to store different AI types ( I think the easiest way to treat has a as just existence in one of the other sets )
//TODO migrate bird to Entity manager.
//updating each AI is then as simple as creating a system for each AI component that loops through them.
//TODO figure out how I want to structure systems. Do I want many functions that take the entity manager? How much is just duplicate code for looping?

class EntityManager{
    int ID = 0;
    
  public:
    std::vector<Entity> entities;
    std::vector<int> freeSlots;

    SparseSet<Sprite> sprites;
    SparseSet<glm::vec2> velocities;
    
    
    
    EntityManager()
    {
      entities.resize(MAXENT);
      for (int i = 0; i < MAXENT; i++)
      {
        freeSlots.push_back(i);
      }
    }
    
    int spawnTest (float x, float y)
    {
      int slotID = freeSlots[freeSlots.size()-1]; freeSlots.pop_back();
      entities[slotID].ID = ID; ID++; //not sure if I actually need this
      entities[slotID].position = {x, y};
      sprites.insert(Sprite{0, 0, 16, 16, 1}, slotID);
      return slotID;
    }
    
    void renderSprites(Renderer *renderer, glm::vec2 camera, float time)
    {
      for (int i = 0; i < sprites.itemCount(); i++)
      {
        Entity& entity = entities[sprites.getEntityID(i)];
        std::cout<<i << " " << sprites.getEntityID(i) << std::endl;
        sprites.items[i].setPostion(entity.position[0] - camera[0], entity.position[1] - camera[1]);
        sprites.items[i].draw(renderer);
      }
    }
    
    // for every component there is a dense and a sparse. The sparse maps the entity ID to the dense array of components
    
    
};