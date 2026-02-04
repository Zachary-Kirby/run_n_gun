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

void RenderRect(Renderer *renderer, float x, float y, float width, float height, float z)
{
  glUseProgram(renderer->defaultShaderProgramID);
  glBindVertexArray(renderer->vao);
  glUniform3f(renderer->uniformLocations["position"], x, y, z);
  glUniform2f(renderer->uniformLocations["scale"], width, height);
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