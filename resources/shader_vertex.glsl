#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_pos;
out vec3 vertex_normal;

void main()
{
	
	vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;

	vec4 tpos =  M * vec4(vertPos, 1.0);	
	vertex_pos = vertPos.xyz;
	gl_Position = P * V * tpos;

}
