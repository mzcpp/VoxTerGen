#version 450 core

layout (location = 0) in vec3 pos_;

layout (std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 camera_pos;
};

out vec3 tex_coords;

void main()
{
    tex_coords = pos_;

    mat4 viewNoTranslation = mat4(mat3(view));

    vec4 position = projection * viewNoTranslation * vec4(pos_, 1.0f);
    
    gl_Position = position.xyww;
}