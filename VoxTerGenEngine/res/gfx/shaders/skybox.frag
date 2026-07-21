#version 450 core

uniform samplerCube skybox;

in vec3 tex_coords;

out vec4 fragment_color;

void main()
{
    fragment_color = texture(skybox, tex_coords);
}