#version 330 core

layout (location = 0) in vec3 iposition;
layout (location = 1) in vec4 icolor;

uniform mat4 projection;
uniform mat4 transform;
uniform vec4 src;

out vec4 vertexColor;
out vec2 uv;
out vec2 st;

void main ()
{
    gl_Position = projection * transform * vec4(iposition, 0.0);
    vertexColor = icolor;
    uv = vec2((iposition.x)*src.z+src.x, (iposition.y)*src.w+src.y);
    st = iposition.xy;
}