#version 450 core

in VS_OUT
{
    vec3 normal;
    vec2 uv;
    flat uint material;
} fs_in;

out vec4 fragment_color;

void main()
{
    const float x_dist_left = fs_in.uv.x;
    const float x_dist_right = 1.0f - fs_in.uv.x;

    const float y_dist_left = fs_in.uv.y;
    const float y_dist_right = 1.0f - fs_in.uv.y;

    const float threshold = 0.0025f;

    if (!(x_dist_left < threshold || x_dist_right < threshold || 
        y_dist_left < threshold || y_dist_right < threshold))
    {
        discard;
    }

    fragment_color = vec4(0.0f);
}