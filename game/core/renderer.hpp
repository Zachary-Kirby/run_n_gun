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

enum class TextureType {
  COLOR,
  DEPTH
};

class Texture{
public:
  float w;
  float h;
  unsigned int id;
  Texture() {w = 0; h = 0; id = 0;}
  Texture& operator=(Texture&& other);
  Texture(const char* path);
  Texture(float w, float h, TextureType type = TextureType::COLOR);
  void bind(){
    glBindTexture(GL_TEXTURE_2D, id);
  }
  void setTarget(int target)
  {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(GL_TEXTURE_2D, id);
  }
  ~Texture(){
    if (id != 0)
    {
      glDeleteTextures(1, &id);
    }
  }
};

class RenderTarget{
public:
  unsigned int fbo;
  Texture* texture;
  RenderTarget() {fbo = 0; texture = nullptr;}
  RenderTarget(Texture* texture);
  RenderTarget(Texture* color, Texture* depth);
  RenderTarget& operator=(RenderTarget&& other){
    if (this != &other){
      fbo = other.fbo;
      texture = other.texture;
      other.fbo = 0;
      other.texture = nullptr;
    }
    return *this;
  }
  void bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  }
  static void unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  ~RenderTarget(){
    if (fbo != 0)
    {
      glDeleteFramebuffers(1, &fbo);
    }
  }
};

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
  Texture atlasTexture;
  Texture atlasAltTexture;

  Texture cloudsTexture;
  Texture backgroundTexture;
  
  Texture gameplayDrawTexture;
  Texture gameplayDepthTexture;
  unsigned int gameplayDrawWidth, gameplayDrawHeight;
  RenderTarget gameplayRenderTarget;
  //Use this to control how you want to interpret rendercopy calls
  float textureSize[2]{1.0f, 1.0f};
  

  std::string loadShaderFromFile(const std::string& filepath);
  std::unordered_map<std::string, unsigned int> uniformLocations;
  glm::mat4 projectionMatrix;
  unsigned int createShader(const char* path, unsigned int type);
  void init(int screenWidth, int screenHeight);
  void setProjectionMatrix(glm::mat4 projectionMatrix);
};

void SetColor(Renderer* renderer, float r, float g, float b, float a);

void SetGradient(Renderer* renderer, float r, float g, float b, float a, float r2, float g2, float b2, float a2);

void RenderRect(Renderer* renderer, float x, float y, float width, float height, float z=0.0f);

void RenderCopy(Renderer* renderer, float* src, float* dst, float z=0.0f);

void RenderCopy(Renderer *renderer, const std::array<float, 4>& src, const std::array<float, 4>& dst, float z=0.0f);

void RenderCopy(Renderer *renderer, SDL_Rect* src, SDL_Rect* dst, float z=0.0f);

void RenderCircle(Renderer* renderer, float x, float y, float radius, float z=0.0f);