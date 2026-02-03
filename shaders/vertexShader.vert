#version 330 core

layout (location = 0) in vec3 iposition ;
layout (location = 1) in vec4 icolor;

uniform mat4 projection;
uniform vec3 position;
uniform vec2 scale;

out vec4 vertexColor;

void main ()
{
    gl_Position = vec4(iposition.x*scale.x+position.x, iposition.y*scale.y+position.y, position.z, 1.0)*projection;
    vertexColor = icolor;
}