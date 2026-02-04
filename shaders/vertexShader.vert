#version 330 core

layout (location = 0) in vec3 iposition;
layout (location = 1) in vec4 icolor;

uniform mat4 projection;
uniform vec3 position;
uniform vec2 scale;
uniform vec4 src;

out vec4 vertexColor;
out vec2 uv;
out vec2 st;

void main ()
{
    gl_Position = projection * vec4(iposition.x*scale.x+position.x, iposition.y*scale.y+position.y, position.z, 1.0);
    vertexColor = icolor;
    uv = vec2((iposition.x)*src.z+src.x, (iposition.y)*src.w+src.y);
    st = iposition.xy;
}