#version 450 core

in VS_OUT
{
    flat uint normal;
    vec2 uv;
    flat uint material;
} fs_in;

out vec4 fragment_color;

void main()
{
    fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}