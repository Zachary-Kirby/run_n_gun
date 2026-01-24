#pragma once
#include <string>
#include <fstream>

//header byte is P
struct LevelPoint{
  int x = 0;
  int y = 0;
  std::string type;
  std::string parameters;
  bool active = true;
  std::string serialize();
  void deserialize(std::ifstream& data);
};

//header byte is R
struct LevelRect
{
  int left;
  int top;
  int right;
  int bottom;
  std::string type;
  std::string parameters;
  bool active = true;
  std::string serialize();
  void deserialize(char* data); //TODO change this to using a stream too
};