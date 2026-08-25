#version 450 core

layout (location = 0) in vec3 pos_;
layout (location = 1) in uint normal_;
layout (location = 2) in vec2 uv_;
layout (location = 3) in uint material_;

layout (std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 camera_pos;
};

uniform mat4 model;

void main()
{   
    gl_Position = projection * view * model * vec4(pos_, 1.0f);
}