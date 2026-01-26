#pragma once

#include <SDL2/SDL.h>

const SDL_Vertex circleVertexData[17] = {
  {1.0f, 0.0f, 255, 0, 0, 255},     
  {0.9239f, 0.3827f, 255, 0, 0, 255},
  {0.7071f, 0.7071f, 255, 0, 0, 255},
  {0.3827f, 0.9239f, 255, 0, 0, 255},
  {0.0f, 1.0f, 255, 0, 0, 255},     
  {-0.3827f, 0.9239f, 255, 0, 0, 255},
  {-0.7071f, 0.7071f, 255, 0, 0, 255},
  {-0.9239f, 0.3827f, 255, 0, 0, 255},
  {-1.0f, 0.0f, 255, 0, 0, 255},    
  {-0.9239f, -0.3827f, 255, 0, 0, 255},
  {-0.7071f, -0.7071f, 255, 0, 0, 255}, 
  {-0.3827f, -0.9239f, 255, 0, 0, 255}, 
  {0.0f, -1.0f, 255, 0, 0, 255},     
  {0.3827f, -0.9239f, 255, 0, 0, 255}, 
  {0.7071f, -0.7071f, 255, 0, 0, 255}, 
  {0.9239f, -0.3827f, 255, 0, 0, 255}, 
  {0.0f, 0.0f, 255, 0, 0, 255}          
};

const SDL_FPoint circlePoints[17] = {
  {1.0f, 0.0f},
  {0.9239f, 0.3827f},
  {0.7071f, 0.7071f},
  {0.3827f, 0.9239f},
  {0.0f, 1.0f},
  {-0.3827f, 0.9239f},
  {-0.7071f, 0.7071f},
  {-0.9239f, 0.3827f},
  {-1.0f, 0.0f},
  {-0.9239f, -0.3827f},
  {-0.7071f, -0.7071f},
  {-0.3827f, -0.9239f},
  {0.0f, -1.0f},
  {0.3827f, -0.9239f},
  {0.7071f, -0.7071f},
  {0.9239f, -0.3827f},
  {1.0f, 0.0f}
};

const int circleIndices[16*3] = {
  0, 1, 16,
  1, 2, 16,
  2, 3, 16,
  3, 4, 16,
  4, 5, 16,
  5, 6, 16,
  6, 7, 16,
  7, 8, 16,
  8, 9, 16,
  9, 10, 16,
  10, 11, 16,
  11, 12, 16,
  12, 13, 16,
  13, 14, 16,
  14, 15, 16,
  15, 0, 16
};


void transformPoints(SDL_FPoint* points, int count, float scale, float translateX, float translateY)
{
  for (int i=0; i<count; i++)
  {
    points[i].x = points[i].x * scale + translateX;
    points[i].y = points[i].y * scale + translateY;
  }
}

void transformVerticies(SDL_Vertex* verticies, int count, float scale, float translateX, float translateY)
{
  for (int i=0; i<count; i++)
  {
    verticies[i].position.x = verticies[i].position.x * scale + translateX;
    verticies[i].position.y = verticies[i].position.y * scale + translateY;
  }
}
