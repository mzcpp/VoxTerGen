#version 450 core

layout (location = 0) in vec2 pos_;
layout (location = 1) in vec2 tex_coord_;

uniform mat4 projection;
uniform mat4 model;

out vec2 tex_coords;

void main()
{
    gl_Position = projection * model * vec4(pos_, 0.0, 1.0);
    tex_coords = tex_coord_;
}