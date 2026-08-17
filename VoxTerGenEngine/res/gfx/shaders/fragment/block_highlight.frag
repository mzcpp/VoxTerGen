#version 450 core

in VS_OUT
{
    flat uint normal;
    vec2 uv;
    flat uint material;
} fs_in;

out vec4 fragment_color;

uniform float distance_threshold;

void main()
{
    const float x_dist_left = fs_in.uv.x;
    const float x_dist_right = 1.0f - fs_in.uv.x;

    const float y_dist_left = fs_in.uv.y;
    const float y_dist_right = 1.0f - fs_in.uv.y;

    const float min_distance = min(min(min(x_dist_left, x_dist_right), y_dist_left), y_dist_right);
    
    if (min_distance >= distance_threshold)
    {
        discard;
    }

    fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}