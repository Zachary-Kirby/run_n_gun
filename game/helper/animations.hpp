#pragma once

#include <vector>
#include <unordered_map>
#include <string>

struct FileSpriteDefinition{
  unsigned short definitionNumber;
  unsigned short x;
  unsigned short y;
  unsigned short w;
  unsigned short h;
  unsigned short duration;
};

struct SpriteDefinition{
  //Note: sprite definitions can be repeated with different definition numbers
  float x;
  float y;
  float w;
  float h;
  float duration;
  SpriteDefinition() = default;
  SpriteDefinition(FileSpriteDefinition& fileSprite);
};



class Animations{
  unsigned short atlasW;
  unsigned short atlasH;
public:
  std::unordered_map<std::string, std::vector<unsigned short>> animationDefinitions;
  std::vector<SpriteDefinition> spriteDefinitions;
  Animations(const char* path);
};