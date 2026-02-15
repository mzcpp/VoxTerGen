#version 450 core

layout (location = 0) in vec3 pos_;
layout (location = 1) in vec3 normal_;
layout (location = 2) in vec2 tex_coord_;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    out vec3 normal;
    out vec2 uv;
    flat out uint color_mask;
} vs_out;

void main()
{
    vs_out.normal = normal_;
    vs_out.uv = tex_coord_;

    // *model????
    gl_Position = projection * view  * vec4(pos_, 1.0f);
}