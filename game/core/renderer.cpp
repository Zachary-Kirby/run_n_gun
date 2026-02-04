#include "renderer.hpp"

#include <fstream>

std::string Renderer::loadShaderFromFile(const std::string &filepath)
{
  std::string programString = "";
  std::ifstream file(filepath);
  if (file.is_open()){
    size_t fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    programString.resize(fileSize);
    file.read(programString.data(), fileSize);
    file.close();
  }
  return programString;
}

unsigned int Renderer::createShader(const char *path, unsigned int type)
{
  unsigned int shaderID = glCreateShader(type);
  std::string shaderString = loadShaderFromFile(path);
  if (shaderString.length() == 0)
  {
    std::cerr << "ERROR::SHADER::LOAD" << std::endl;
  }
  const char* tmp = shaderString.c_str();
  glShaderSource(shaderID, 1, &tmp, NULL);
  glCompileShader(shaderID);
  int success;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int log_length = 0;
    char infoLog[512];
    glGetShaderInfoLog(shaderID, 512, &log_length, infoLog);
    std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  return shaderID;
}

void Renderer::init(int screenWidth, int screenHeight)
{
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
  
  unsigned int vertexShaderID = createShader("shaders/vertexShader.vert", GL_VERTEX_SHADER);
  unsigned int fragmentShaderID = createShader("shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
  unsigned int circleShaderID = createShader("shaders/circleShader.frag", GL_FRAGMENT_SHADER);
  unsigned int colorShaderID = createShader("shaders/colored.frag", GL_FRAGMENT_SHADER);
  
  //TODO check for link errors
  defaultShaderProgramID = glCreateProgram();
  glAttachShader(defaultShaderProgramID, vertexShaderID);
  glAttachShader(defaultShaderProgramID, fragmentShaderID);
  glLinkProgram(defaultShaderProgramID);
  
  circleShaderProgramID = glCreateProgram();
  glAttachShader(circleShaderProgramID, vertexShaderID);
  glAttachShader(circleShaderProgramID, circleShaderID);
  glLinkProgram(circleShaderProgramID);
  
  GLint success;
  glGetProgramiv(circleShaderProgramID, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetProgramInfoLog(circleShaderProgramID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  coloredShaderProgramID = glCreateProgram();
  glAttachShader(coloredShaderProgramID, vertexShaderID);
  glAttachShader(coloredShaderProgramID, colorShaderID);
  glLinkProgram(coloredShaderProgramID);
  
  
  //glDeleteShader(colorShaderID);
  //glDeleteShader(circleShaderID);
  //glDeleteShader(vertexShaderID);
  //glDeleteShader(fragmentShaderID);
  
  
  
  
  projectionMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
  
  glUseProgram(defaultShaderProgramID);
  uniformLocations["projection"] = glGetUniformLocation(defaultShaderProgramID, "projection");
  uniformLocations["position"] = glGetUniformLocation(defaultShaderProgramID, "position");
  uniformLocations["scale"] = glGetUniformLocation(defaultShaderProgramID, "scale");
  uniformLocations["src"] = glGetUniformLocation(defaultShaderProgramID, "src");
  uniformLocations["atlas"] = glGetUniformLocation(defaultShaderProgramID, "atlas");
  glUniformMatrix4fv(uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
 
  glUseProgram(coloredShaderProgramID);
  glUniformMatrix4fv(glGetUniformLocation(coloredShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  
  glUseProgram(circleShaderProgramID);
  glUniformMatrix4fv(glGetUniformLocation(circleShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
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

void SetColor(Renderer* renderer, float r, float g, float b, float a)
{
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
  for (int i=0; i<sizeof(renderer->verticies)/sizeof(float); i=i+7)
  {
    renderer->verticies[i+3] = r;
    renderer->verticies[i+4] = g;
    renderer->verticies[i+5] = b;
    renderer->verticies[i+6] = a;
  }
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(renderer->verticies), renderer->verticies);
}

void RenderRect(Renderer *renderer, float x, float y, float width, float height, float z)
{
  glUseProgram(renderer->coloredShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(glGetUniformLocation(renderer->coloredShaderProgramID, "position"), x, y, z);
  glUniform2f(glGetUniformLocation(renderer->coloredShaderProgramID, "scale"), width, height);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}

void RenderCopy(Renderer *renderer, float *src, float *dst, float z)
{
  glUseProgram(renderer->defaultShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(renderer->uniformLocations["position"], dst[0], dst[1], z);
  glUniform2f(renderer->uniformLocations["scale"], dst[2], dst[3]);
  glUniform4f(renderer->uniformLocations["src"], src[0]/renderer->atlasWidth, src[1]/renderer->atlasHeight, src[2]/renderer->atlasWidth, src[3]/renderer->atlasHeight);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}

void RenderCopy(Renderer *renderer, const std::array<float, 4>& src, const std::array<float, 4>& dst, float z)
{
  glUseProgram(renderer->defaultShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(renderer->uniformLocations["position"], dst[0], dst[1], z);
  glUniform2f(renderer->uniformLocations["scale"], dst[2], dst[3]);
  glUniform4f(renderer->uniformLocations["src"], src[0]/renderer->atlasWidth, src[1]/renderer->atlasHeight, src[2]/renderer->atlasWidth, src[3]/renderer->atlasHeight);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}

void RenderCopy(Renderer *renderer, SDL_Rect* src, SDL_Rect* dst, float z)
{
  glUseProgram(renderer->defaultShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(renderer->uniformLocations["position"], dst->x, dst->y, z);
  glUniform2f(renderer->uniformLocations["scale"], dst->w, dst->h);
  glUniform4f(renderer->uniformLocations["src"],
    src->x/((float)renderer->atlasWidth),
    src->y/((float)renderer->atlasHeight),
    src->w/((float)renderer->atlasWidth),
    src->h/((float)renderer->atlasHeight)
  );
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}

void RenderCircle(Renderer *renderer, float x, float y, float radius, float z)
{
  glUseProgram(renderer->circleShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(glGetUniformLocation(renderer->circleShaderProgramID, "position"), x-radius/2, y-radius/2, z);
  glUniform2f(glGetUniformLocation(renderer->circleShaderProgramID, "scale"), radius, radius);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}
