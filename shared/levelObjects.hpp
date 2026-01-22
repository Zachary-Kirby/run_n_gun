#pragma once
#include <string>

//header byte is P
struct LevelPoint{
  int x = 0;
  int y = 0;
  std::string type;
  std::string parameters;
  std::string serialize();
  void deserialize(std::string_view data);
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
  std::string serialize();
  void deserialize(std::string_view data);
};