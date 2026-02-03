#pragma once

#include "glad.h"
#include "SDL2/SDL.h"
#include <iostream>



class Renderer {
  static constexpr unsigned char indices[] = {0, 1, 2, 2, 3, 0};
  static constexpr float verticies[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };
  unsigned int vbo, vao, ebo;
public:
  void init() {
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
    glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
    
    //it's fine to leave this buffer bound as it's the only one needed in this 2D game
  }
};