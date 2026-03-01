#include "sound.hpp"
#include <iostream>

void Sound::init(const char *soundFilePath)
{
  data = Mix_LoadWAV(soundFilePath);
  if (!data)
  {
    std::cerr << "sound didn't load: " << soundFilePath;
  }
}

void Sound::play(int channel, int times)
{
  Mix_PlayChannel(channel, data, times);
}

Sound::~Sound()
{
  Mix_FreeChunk(data);
}
