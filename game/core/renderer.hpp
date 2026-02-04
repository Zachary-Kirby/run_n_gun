#pragma once

#include "glad.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>


class Renderer {
public:
  unsigned char indices[6]{0, 1, 2, 0, 2, 3};
  float verticies[4*7]{
     0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
  };
  unsigned int vbo, vao, ebo;
  unsigned int defaultShaderProgramID;
  unsigned int circleShaderProgramID;
  unsigned int coloredShaderProgramID;
  unsigned int atlasID, atlasWidth, atlasHeight;
  std::string loadShaderFromFile(const std::string& filepath);
  std::unordered_map<std::string, unsigned int> uniformLocations;
  glm::mat4 projectionMatrix;
  unsigned int createShader(const char* path, unsigned int type);
  void init(int screenWidth, int screenHeight);
};

void SetColor(Renderer* renderer, float r, float g, float b, float a);

void RenderRect(Renderer* renderer, float x, float y, float width, float height, float z=0.0f);

void RenderCopy(Renderer* renderer, float* src, float* dst, float z=0.0f);

void RenderCopy(Renderer *renderer, const std::array<float, 4>& src, const std::array<float, 4>& dst, float z=0.0f);

void RenderCopy(Renderer *renderer, SDL_Rect* src, SDL_Rect* dst, float z=0.0f);

void RenderCircle(Renderer* renderer, float x, float y, float radius, float z=0.0f);