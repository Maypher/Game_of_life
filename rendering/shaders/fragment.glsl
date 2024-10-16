#version 460 core

out vec4 FragColor;  

in float cell_alive;

void main()
{
	vec4 color = vec4(1.0f);
	
	if (cell_alive == 0) {
		color = vec4(0.0f);
	}

    FragColor = color;
} 
