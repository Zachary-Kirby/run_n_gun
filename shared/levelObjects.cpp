#include "levelObjects.hpp"

std::string LevelPoint::serialize()
{
  std::string data;
  data.reserve(sizeof(char) + sizeof(x) + sizeof(y) + sizeof(int) + type.size() + sizeof(int) + parameters.size());
  
  //The object type header
  data.append("P");
  
  data.append(reinterpret_cast<char*>(&x), sizeof(x));
  data.append(reinterpret_cast<char*>(&y), sizeof(y));
  
  int typeSize = type.size();
  data.append(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
  data.append(type);

  int paramSize = parameters.size();
  data.append(reinterpret_cast<char*>(&paramSize), sizeof(paramSize));
  data.append(parameters);
  
  return data;
}

void LevelPoint::deserialize(std::ifstream& file)
{
  file.read(reinterpret_cast<char*>(&x), sizeof(x));
  file.read(reinterpret_cast<char*>(&y), sizeof(y));
  int typeSize;
  file.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
  type.resize(typeSize);
  file.read(type.data(), typeSize);
  int paramSize;
  file.read(reinterpret_cast<char*>(&paramSize), sizeof(paramSize));
  parameters.resize(paramSize);
  file.read(parameters.data(), paramSize);
}

std::string LevelRect::serialize()
{
  std::string data;
  data.reserve(sizeof(char) + sizeof(left) + sizeof(top) + sizeof(right) + sizeof(bottom) + sizeof(int) + type.size() + sizeof(int) + parameters.size());
  
  //The object type header
  data.append("R");
  
  data.append(reinterpret_cast<char*>(&left), sizeof(left));
  data.append(reinterpret_cast<char*>(&top), sizeof(top));
  data.append(reinterpret_cast<char*>(&right), sizeof(right));
  data.append(reinterpret_cast<char*>(&bottom), sizeof(bottom));
  
  int typeSize = type.size();
  data.append(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
  data.append(type);

  int paramSize = parameters.size();
  data.append(reinterpret_cast<char*>(&paramSize), sizeof(paramSize));
  data.append(parameters);
  
  return data;
}

void LevelRect::deserialize(char* data)
{
  int cursor = 0;
  left = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(left);
  top = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(top);
  right = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(right);
  bottom = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(bottom);
  int typeSize = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(typeSize);
  type = std::string(data+cursor, typeSize); cursor += type.size();
  int paramSize = *reinterpret_cast<const int*>(data+cursor); cursor += sizeof(paramSize);
  parameters = std::string(data+cursor, paramSize);
}
