#version 330 core

out vec4 FragColor;
in vec4 vertexColor;
in vec2 uv;

uniform sampler2D atlas;

void main()
{
    vec4 color = texture(atlas, uv).xyzw;
    if (color.w == 0.0f)
        discard;
    FragColor = color;
}