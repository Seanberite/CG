#version 150

in  vec3 in_Position;
in vec3 inColor;
out vec3 exColor;
uniform mat4 myMatrix;

void main(void)
{
vec4 transformed = myMatrix*vec4(in_Position, 1.0);
	exColor = inColor;
	gl_Position = vec4(transformed);
}
