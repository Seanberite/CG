#version 150

in  vec3 in_Position;

void main(void)
{
vec4 transformed = vec4(in_Position, 1.0);

	gl_Position = vec4(transformed);
}
