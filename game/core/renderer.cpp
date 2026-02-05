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
  

  // Create Shaders
  unsigned int vertexShaderID = createShader("shaders/vertexShader.vert", GL_VERTEX_SHADER);
  unsigned int fragmentShaderID = createShader("shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
  unsigned int circleShaderID = createShader("shaders/circleShader.frag", GL_FRAGMENT_SHADER);
  unsigned int colorShaderID = createShader("shaders/colored.frag", GL_FRAGMENT_SHADER);
  
  //TODO check for link errors
  //TODO automate
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
  
  
  // glDeleteShader(colorShaderID);
  // glDeleteShader(circleShaderID);
  // glDeleteShader(vertexShaderID);
  // glDeleteShader(fragmentShaderID);
  
  
  
  
  projectionMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
  
  //Set Default Uniforms
  glUseProgram(defaultShaderProgramID);
  uniformLocations["projection"] = glGetUniformLocation(defaultShaderProgramID, "projection");
  uniformLocations["position"] = glGetUniformLocation(defaultShaderProgramID, "position");
  uniformLocations["scale"] = glGetUniformLocation(defaultShaderProgramID, "scale");
  uniformLocations["src"] = glGetUniformLocation(defaultShaderProgramID, "src");
  uniformLocations["atlas"] = glGetUniformLocation(defaultShaderProgramID, "atlas");
  glUniformMatrix4fv(uniformLocations["projection"], 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  glUniform1i(uniformLocations["atlas"], 0);
  
  glUseProgram(coloredShaderProgramID);
  glUniformMatrix4fv(glGetUniformLocation(coloredShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  
  glUseProgram(circleShaderProgramID);
  glUniformMatrix4fv(glGetUniformLocation(circleShaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  
  // Textures
  atlasTexture = Texture("Assets/Atlas.png");
  atlasAltTexture = Texture("Assets/AltAtlas.png");
  gameplayDrawWidth = screenWidth;
  gameplayDrawHeight = screenWidth;
  //TODO stop being lazy and just make a way to mirror the texture
  gameplayDrawTexture = Texture((float)gameplayDrawWidth, (float)gameplayDrawHeight, TextureType::COLOR);
  gameplayDepthTexture = Texture((float)gameplayDrawWidth, (float)gameplayDrawHeight, TextureType::DEPTH);
  gameplayRenderTarget = RenderTarget(&gameplayDrawTexture, &gameplayDepthTexture);
  atlasTexture.setTarget(0);
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
  glUniform4f(renderer->uniformLocations["src"], src[0]/renderer->atlasTexture.w, src[1]/renderer->atlasTexture.h, src[2]/renderer->atlasTexture.w, src[3]/renderer->atlasTexture.h);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (void*)0);
  glBindVertexArray(0);
}

void RenderCopy(Renderer *renderer, const std::array<float, 4>& src, const std::array<float, 4>& dst, float z)
{
  glUseProgram(renderer->defaultShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(renderer->uniformLocations["position"], dst[0], dst[1], z);
  glUniform2f(renderer->uniformLocations["scale"], dst[2], dst[3]);
  glUniform4f(renderer->uniformLocations["src"], src[0]/renderer->atlasTexture.w, src[1]/renderer->atlasTexture.h, src[2]/renderer->atlasTexture.w, src[3]/renderer->atlasTexture.h);
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
    src->x/((float)renderer->atlasTexture.w),
    src->y/((float)renderer->atlasTexture.h),
    src->w/((float)renderer->atlasTexture.w),
    src->h/((float)renderer->atlasTexture.h)
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

Texture &Texture::operator=(Texture &&other)
{
  if (this != &other){
    w = other.w;
    h = other.h;
    id = other.id;
    other.id = 0;
  }
  return *this;
}

Texture::Texture(const char *path)
{
  SDL_Surface* surface = IMG_Load(path);
  w = (float)surface->w;
  h = (float)surface->h;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  SDL_FreeSurface(surface);
}

Texture::Texture(float w, float h, TextureType type)
{
  this->w = w;
  this->h = h;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  if (type == TextureType::COLOR){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)w, (int)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
  else if (type == TextureType::DEPTH){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (int)w, (int)h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
  }
  
}

RenderTarget::RenderTarget(Texture *texture)
{
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTarget::RenderTarget(Texture *color, Texture *depth)
{
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  if (color != nullptr)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->id, 0);
  else
    std::cerr << "RENDERTARGET::COLORTEXTURE::NULL" << std::endl;
  
  if (depth != nullptr)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->id, 0);
  else
    std::cerr << "RENDERTARGET::DEPTHTEXTURE::NULL" << std::endl;
  
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cerr << "FRAMEBUFFER::INCOMPLETE - ";
    switch (status)
    {
      case GL_FRAMEBUFFER_UNDEFINED:
        std::cerr << "GL_FRAMEBUFFER_UNDEFINED";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
        break;
      default:
        std::cerr << "Unknown error code: " << status;
        break;
    }
    std::cerr << std::endl;
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
