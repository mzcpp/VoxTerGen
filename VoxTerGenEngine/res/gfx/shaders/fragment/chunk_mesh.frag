#version 450 core

uniform sampler2D atlas_texture;
uniform uint atlas_columns;
uniform uint atlas_rows;

uniform float fog_near;
uniform float fog_far;
uniform float fog_influence;

layout (std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 camera_pos;
};

in VS_OUT
{
	vec3 pos;
    flat uint normal;
    vec2 uv;
    flat uint material;
} fs_in;

out vec4 fragment_color;

vec2 GetAtlasUV(uint material)
{
	 const vec2 xy_delta = { 1.0f / atlas_columns, 1.0f / atlas_rows };
	 vec2 tile_min = { 0.0f, 0.0f };

	 switch (material)
	 {
	 case 0:
	 case 1:
	 	break;
	 case 2:
		tile_min.x = 0.5f;
	 	break;
	 case 3:
	 	tile_min.x = 0.5f;
		tile_min.y = 0.25f;
	 	break;
	 case 4:
		tile_min.y = 0.25f;
	 	break;
	 case 5:
		tile_min.y = 0.75f;
	 	break;
	 case 6:
	 	tile_min.x = 0.5f;
		tile_min.y = 0.5f;
	 	break;
	 case 7:
		tile_min.y = 0.5f;
	 	break;
	 case 8:
		tile_min.x = 0.5f;
		tile_min.y = 0.75f;
	 	break;
	 }

	 return tile_min + fract(fs_in.uv) * xy_delta;
}

float CalculateFogAmount()
{
	const float dx = (fs_in.pos.x - camera_pos.x);
	const float dy = (fs_in.pos.y - camera_pos.y);
	const float dz = (fs_in.pos.z - camera_pos.z);

	const float fragment_distance_squared = dx * dx + dy * dy + dz * dz;

    float fog_amount = smoothstep(fog_near, fog_far, sqrt(fragment_distance_squared)) * fog_influence;

	return fog_amount;
}

void main()
{
	const float fog_amount = CalculateFogAmount();
	const vec4 fog_color = { 0.878, 0.878, 0.878, 1.0 };

    //fragment_color = mix(texture(atlas_texture, GetAtlasUV(fs_in.material)), fog_color, fog_amount);
    fragment_color = texture(atlas_texture, GetAtlasUV(fs_in.material));
}