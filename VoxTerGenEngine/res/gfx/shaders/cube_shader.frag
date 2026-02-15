#version 450 core

uniform sampler2D texture_1;
uniform int u_face_id;

in VS_OUT
{
    in vec3 normal;
    in vec2 uv;
    flat in uint color_mask;
} fs_in;

out vec4 fragment_color;

void main()
{
    vec3 color = vec3(0.0f);
    fragment_color = vec4(color, 1.0);
}