#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int alive;
layout (location = 2) in int hovered;

uniform uvec2 game_dimensions;

out float cell_alive;

void main()
{
	vec2 tile_size = vec2(2.0/game_dimensions.xy);
	float row = gl_InstanceID / game_dimensions.x;
	float col = gl_InstanceID % game_dimensions.y;

	vec2 offset = vec2(
		-1.0 + col * tile_size.x,
		-1.0 + row * tile_size.y
	);

	cell_alive = float(alive);

    gl_Position = vec4(aPos.xy * tile_size + offset, aPos.z, 1.0);
}
