#version 150

in  vec3 in_Position;
in  vec2 in_Texcoord;
uniform mat4 modelviewMatrix;
uniform mat4 projMatrix;
out  vec2 texcoord;

void main(void)
{
	gl_Position = projMatrix * modelviewMatrix * vec4(in_Position, 1.0);
	texcoord = in_Texcoord;
}
