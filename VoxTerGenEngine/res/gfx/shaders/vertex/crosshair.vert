#version 450 core

layout (location = 0) in vec3 pos_;
layout (location = 1) in vec2 tex_coord_;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(pos_, 1.0);
    tex_coords = tex_coord_;
}