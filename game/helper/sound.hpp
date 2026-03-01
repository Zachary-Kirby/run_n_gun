#include "SDL_mixer.h"

class Sound
{
  Mix_Chunk* data = nullptr;
  
public:
  void init(const char* soundFilePath);
  void play(int channel = -1, int times = 0);
  ~Sound();
  
  
};

class SoundScope
{
public:
  ~SoundScope(){Mix_Quit();}
};