#version 450 core

in VS_OUT
{
    in vec3 normal;
    in vec2 uv;
} fs_in;

out vec4 fragment_color;

uniform sampler2D texture1;

void main()
{
    fragment_color = texture(texture1, fs_in.uv);
}