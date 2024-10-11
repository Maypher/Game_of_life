#version 460 core
layout (location = 0) in vec3 aPos;

uniform uvec2 game_dimensions;

void main()
{
	vec2 scaled_pos = aPos.xy / vec2(game_dimensions.xy) * 2.0;
    gl_Position = vec4(scaled_pos.xy, aPos.z, 1.0);
}
