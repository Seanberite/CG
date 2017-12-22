#version 150

out vec4 out_Color;
in vec3 ex_Normal;
in vec2 exTexCoord;

void main(void)
{
	float a = sin(exTexCoord.s*30.0)/2.0+0.5;
	float b = sin(exTexCoord.t*30.0)/2.0+0.5;
	const vec3 light = vec3(0, 0, -1);
	float shade;
	shade = clamp(shade, 0, 1);
	shade  = dot(normalize(ex_Normal),light);
	out_Color = vec4(a, b, 1, 1.0);
}
