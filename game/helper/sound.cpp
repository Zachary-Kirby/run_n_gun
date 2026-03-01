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

void Music::init(const char *musicFilePath)
{
  data = Mix_LoadMUS(musicFilePath);
  if (!data)
  {
    std::cerr << "music didn't load: " << musicFilePath;
  }
}

void Music::play(int loops)
{
  Mix_PlayMusic(data, loops);
}

Music::~Music()
{
  Mix_FreeMusic(data);
}
