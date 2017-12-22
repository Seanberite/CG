#version 150

in  vec3 in_Position;
in vec3 in_Normal;
out vec3 ex_Normal;
uniform mat4 myMatrix;
uniform mat4 myMatrix2;

void main(void)
{

vec4 transformed = myMatrix*vec4(in_Position, 1.0);
	
	ex_Normal = vec3(in_Normal);
	gl_Position = vec4(transformed);
}
