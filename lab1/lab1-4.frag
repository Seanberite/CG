#version 150

out vec4 out_Color;
in vec3 exColor;

void main(void)
{
	out_Color = vec4(exColor, 1.0);
}
