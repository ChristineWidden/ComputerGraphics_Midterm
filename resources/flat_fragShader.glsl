#version 330 core
out vec4 color;
in vec3 vertex_pos;
in vec3 pos;

uniform vec3 campos;
uniform vec4 setColor;


void main()
{

	float red = clamp(setColor.r - pos.z * 0.5 + 1.2, setColor.r * 0.5, setColor.r);
	float green = clamp(setColor.g - pos.z * 0.5 + 1.2, setColor.g * 0.5, setColor.g);
	float blue = clamp(setColor.b - pos.z * 0.5 + 1.2, setColor.b * 0.5, setColor.b);

	color = vec4(red, green, blue, 1);

	//color = vec4(setColor.rgb - pos.z * 0.3 + 0.5, 1);

}




