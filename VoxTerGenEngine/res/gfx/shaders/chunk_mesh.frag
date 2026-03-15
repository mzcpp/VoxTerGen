#version 450 core

in VS_OUT
{
    in vec3 normal;
    in vec2 uv;
} fs_in;

out vec4 fragment_color;

void main()
{
    vec3 color = vec3(0.0f);
    fragment_color = vec4(color, 1.0);
}