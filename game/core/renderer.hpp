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
  static constexpr unsigned char indices[] = {0, 1, 2, 0, 2, 3};
  static constexpr float verticies[] = {
     0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
  };
  
  public:
  unsigned int vbo, vao, ebo;
  unsigned int defaultShaderProgramID;
  unsigned int atlasID, atlasWidth, atlasHeight;
  std::string loadShaderFromFile(const std::string& filepath);
  std::unordered_map<std::string, unsigned int> uniformLocations;
  glm::mat4 projectionMatrix;
  void init(int screenWidth, int screenHeight) {
    int version = gladLoadGLLoader(SDL_GL_GetProcAddress);
    if (version == 0){
      std::cerr << "Failed to initialize GLAD" << std::endl;
    } else{
      std::cout << "OpenGL Version: " << version << std::endl;
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderString = loadShaderFromFile("shaders/vertexShader.vert");
    const char* tmp = vertexShaderString.c_str(); 
    glShaderSource(vertexShaderID, 1, &tmp, NULL);
    glCompileShader(vertexShaderID);
    
    int success;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      int log_length = 0;
      char infoLog[512];
      glGetShaderInfoLog(vertexShaderID, 512, &log_length, infoLog);
      std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderString = loadShaderFromFile("shaders/fragmentShader.frag");
    tmp = fragmentShaderString.c_str();
    glShaderSource(fragmentShaderID, 1, &tmp, NULL);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      int log_length = 0;
      char infoLog[512];
      glGetShaderInfoLog(fragmentShaderID, 512, &log_length, infoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    defaultShaderProgramID = glCreateProgram();
    glAttachShader(defaultShaderProgramID, vertexShaderID);
    glAttachShader(defaultShaderProgramID, fragmentShaderID);
    glLinkProgram(defaultShaderProgramID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glUseProgram(defaultShaderProgramID);

    uniformLocations["projection"] = glGetUniformLocation(defaultShaderProgramID, "projection");
    uniformLocations["position"] = glGetUniformLocation(defaultShaderProgramID, "position");
    uniformLocations["scale"] = glGetUniformLocation(defaultShaderProgramID, "scale");
    uniformLocations["src"] = glGetUniformLocation(defaultShaderProgramID, "src");
    uniformLocations["atlas"] = glGetUniformLocation(defaultShaderProgramID, "atlas");
    
    projectionMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    //it's fine to leave this buffer bound as it's the only one needed in this 2D game
    
    
    SDL_Surface* atlas = IMG_Load("Assets/Atlas.png");
    glGenTextures(1, &atlasID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, atlas->w, atlas->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    atlasWidth = atlas->w;
    atlasHeight = atlas->h;
    SDL_FreeSurface(atlas);
    
    glUniform1i(uniformLocations["atlas"], 0);
    glUniform4f(uniformLocations["src"], 0.0f, 0.0f, 1.0f/4.0f, 1.0f/4.0f);
  }
};

void RenderRect(Renderer* renderer, float x, float y, float width, float height, float z=0.0f);

void RenderCopy(Renderer* renderer, float* src, float* dst, float z=0.0f);

void RenderCopy(Renderer *renderer, const std::array<float, 4>& src, const std::array<float, 4>& dst, float z=0.0f);

void RenderCopy(Renderer *renderer, SDL_Rect* src, SDL_Rect* dst, float z=0.0f);