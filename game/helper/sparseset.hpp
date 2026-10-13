#pragma once

#include <vector>
#include <iostream>

template <typename T>
class SparseSet
{
  /*
    Map an integer to a compact array of the items
  */
  //TODO, I don't need the vector of items to actually be the same size as indices and keys
  std::vector<int> indices; // sparse mappings to the dense indirection layer
  std::vector<int> keys; // maps back to the corresponding index, I use this for knowing how to swap the real data in memory
  int itemsTop = 0;
  size_t size;
  
  public:
  std::vector<T> items;
  
  SparseSet(int size) { resize(size); };
  
  void resize(size_t s)
  {
    size = s;
    indices.resize(size);
    indices.assign(size, -1);
    keys.resize(size);
    items.resize(size);
  };
  
  void clear()
  {
    indices.assign(size, -1);
    itemsTop = 0;
  }

  int itemCount() {
    return itemsTop;
  }
  
  int getEntitySlotID(int k){
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
    if (i < 0 || (size_t)i >= size || indices[i] == -1)
    {
      // invalid access, log it and hand back a default object so the caller can't crash
      std::cerr << "SparseSet: invalid access at index " << i << "\n";
      static T fallback;
      fallback = T();
      return fallback;
    }
    return items[indices[i]];
  };

  bool has(int i){
    //return whether that entity ID has this component
    if (i < 0 || (size_t)i >= size) return false;
    return indices[i] != -1;
  }
};