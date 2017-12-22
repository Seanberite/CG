#version 150

out vec4 out_Color;
in vec3 ex_Normal;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade  = dot(normalize(ex_Normal),light);
	out_Color = vec4(shade, shade, shade, 1.0);
}
