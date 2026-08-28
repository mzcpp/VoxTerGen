#version 450 core

uniform sampler2D crosshair_texture;

in vec3 tex_coords;

out vec4 fragment_color;

void main()
{
    fragment_color = texture(crosshair_texture, tex_coords);
}