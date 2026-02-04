#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 uv;
in vec2 st;

uniform sampler2D atlas;

void main()
{
  //vec4 color = texture(atlas, uv).xyzw;
  //if (color.w == 0.0f)
  //  discard;
  float dist = distance(st, vec2(0.5f, 0.5f));
  float radius = 0.5f;
  
  //float edgeDist = abs(dist - radius);
  //float pixelDist = edgeDist / fwidth(dist);
  
  if (dist >= radius)
    discard;
  
  FragColor = vertexColor;
}