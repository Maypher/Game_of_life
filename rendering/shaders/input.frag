#version 460 core

out vec4 FragColor;  

in float cell_id;

void main()
{
	vec4 color = vec4(cell_id);

    FragColor = color;
}
