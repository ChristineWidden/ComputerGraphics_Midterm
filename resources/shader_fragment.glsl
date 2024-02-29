

#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;

uniform vec3 campos;
uniform vec4 setColor;

void main()
{
vec3 theColor = setColor.rgb;
//theColor = vec3(1, 0, 0);


vec3 ambient = vec3(1, 1, 1) * 0.4;

vec3 n = normalize(vertex_normal);
vec3 lp=vec3(-120,100,-100);
vec3 ld = normalize(vertex_pos - lp);
float diffuse = dot(n,ld);

vec3 col = theColor*diffuse*0.9;

vec3 cd = normalize(vertex_pos - campos);
vec3 h = normalize(cd+ld);
float spec = dot(n,h);
spec = clamp(spec,0,1);
spec = pow(spec,15);
//col += vec3(1,1,1)*spec*3;

col += vec3(1,1,1)*spec*2 * theColor;
color = vec4(col, setColor.a);



}




