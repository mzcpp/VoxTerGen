#version 450 core

layout (location = 0) in vec3 pos_;
layout (location = 1) in vec3 normal_;
layout (location = 2) in vec2 uv_;
layout (location = 3) in uint material_;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 normal;
    vec2 uv;
    flat uint material;
} vs_out;

void main()
{
    vs_out.normal = normal_;
    vs_out.uv = uv_;
    vs_out.material = material_;

    gl_Position = projection * view * model * vec4(pos_, 1.0f);
}