#include "animations.hpp"
#include <fstream>
#include <iostream>
/*
Sprite definition list:
  2 bytes: atlas width
  2 bytes: atlas height
  2 bytes: number of sprite definitions
  2 bytes: size of each sprite definition
  
Sprite definition:
  2 bytes: sprite ID
  2 bytes: x
  2 bytes: y
  2 bytes: width
  2 bytes: height
  2 bytes: duration

Animation definition list:
  2 bytes: number of animation definitions

Animation definition:
  2 bytes: length of name
  N bytes: name
  2 bytes: number of frames
  2 bytes: frame ID
*/



Animations::Animations(const char *path)
{
  std::ifstream file{path, std::ios::binary | std::ios::in};
  FileSpriteDefinition tempDefintion;
  if (file.is_open())
  {
    char buffer[1024]{"\0"};
    
    file.readsome((char*)&atlasW, sizeof(short));
    file.readsome((char*)&atlasH, sizeof(short));
    
    file.readsome(buffer, sizeof(int));
    unsigned short definitonCount = *(unsigned short*)buffer;
    spriteDefinitions.reserve(definitonCount);
    for (int i=0; i<definitonCount; i++)
    {
      file.readsome((char*)&tempDefintion, sizeof(FileSpriteDefinition));
      spriteDefinitions.emplace_back(tempDefintion);
    }
    
    unsigned short animationCount;
    file.readsome((char*)&animationCount, sizeof(short));
    
    for (int i=0; i<animationCount; i++)
    {
      unsigned char nameLength;
      file.readsome((char*)&nameLength, 1);
      std::string name;
      name.resize(nameLength);
      file.readsome(name.data(), nameLength);
      unsigned short frameCount;
      file.readsome((char*)&frameCount, 2);
      std::vector<unsigned short> frames;
      frames.resize(frameCount);
      file.readsome((char*)frames.data(), frameCount*sizeof(short));
      animationDefinitions[name] = frames;
    }
    file.close();
  }
}

SpriteDefinition::SpriteDefinition(FileSpriteDefinition &fileSprite)
{
  x = fileSprite.x;
  y = fileSprite.y;
  this->w = fileSprite.w;
  this->h = fileSprite.h;
  duration = fileSprite.duration/1000.0f; //since aseprite outputs milliseconds
}
