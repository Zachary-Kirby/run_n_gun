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
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;
  float duration = 0;
  SpriteDefinition() = default;
  SpriteDefinition(FileSpriteDefinition& fileSprite);
};



class Animations{
  unsigned short atlasW;
  unsigned short atlasH;
public:
  std::unordered_map<std::string, std::vector<unsigned short>> animationDefinitions;
  std::unordered_map<std::string, float> animationTotalDuration;
  std::vector<SpriteDefinition> spriteDefinitions;
  Animations(const char* path);
  SpriteDefinition getFrame(const std::string& animationName, int frameNumber, bool xFlip = false, bool yFlip = false);
  SpriteDefinition getAnimationFrame(const std::string& animationName, float time, bool xFlip = false, bool yFlip = false);
};