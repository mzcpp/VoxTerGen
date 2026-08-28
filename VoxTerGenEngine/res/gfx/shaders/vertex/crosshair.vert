#version 450 core

layout (location = 0) in vec3 pos_;
layout (location = 1) in vec2 tex_coord_;

out vec3 tex_coord;

void main()
{   
    gl_Position = vec4(pos_, 1.0f);
    tex_coord = vec2(tex_coord_.x, tex_coord_.y);
}