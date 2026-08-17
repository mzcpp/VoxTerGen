#version 450 core

uniform sampler2D atlas_texture;
uniform uint atlas_columns;
uniform uint atlas_rows;

in VS_OUT
{
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

void main()
{
    fragment_color = texture(atlas_texture, GetAtlasUV(fs_in.material));
}