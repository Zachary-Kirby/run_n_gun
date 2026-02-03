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

void RenderRect(Renderer *renderer, float x, float y, float width, float height)
{
  glBindVertexArray(renderer->vao);
  glUniform2f(renderer->uniformLocations["position"], x, y);
  glUniform2f(renderer->uniformLocations["scale"], width, height);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}
