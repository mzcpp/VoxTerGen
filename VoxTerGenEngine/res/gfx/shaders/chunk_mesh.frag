#version 450 core

in VS_OUT
{
    vec3 normal;
    vec2 uv;
    flat uint material;
} fs_in;

out vec4 fragment_color;

uniform sampler2D atlas_texture;

void main()
{
    fragment_color = texture(atlas_texture, fs_in.uv);
}