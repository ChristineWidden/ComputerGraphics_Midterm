#version 330 core
out vec4 color;
in vec2 vertex_tex;

uniform sampler2D tex;

void main()
{
color = vec4(texture(tex, vertex_tex).rgb, 1);

//color = vec4(1, 0, 0, 1);



}
