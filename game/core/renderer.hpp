#pragma once

#include "glad.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <unordered_map>


class Renderer {
  static constexpr unsigned char indices[] = {0, 2, 1, 2, 3, 0};
  static constexpr float verticies[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };
  
  public:
  unsigned int vbo, vao, ebo;
  unsigned int defaultShaderProgramID;
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
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
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
    
    projectionMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    //it's fine to leave this buffer bound as it's the only one needed in this 2D game
  }
};

void RenderRect(Renderer* renderer, float x, float y, float width, float height);