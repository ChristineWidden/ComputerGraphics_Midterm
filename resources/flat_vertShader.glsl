#version 330 core
layout(location = 0) in vec3 vertPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_pos;
out vec3 pos;

void main()
{

	vec4 tpos =  M * vec4(vertPos, 1.0);	
	vertex_pos = vertPos.xyz;
	gl_Position = P * V * tpos;
	pos = gl_Position.xyz;

}
