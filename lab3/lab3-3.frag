#version 150

out vec4 out_Color;
in vec3 ex_Normal;
in vec2 exTexCoord;
uniform sampler2D texUnit1;


void main(void)
{
	
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	float ambient;
	ambient = 1.0;
	shade  = dot(normalize(ex_Normal),light);
	out_Color = shade*texture(texUnit1, exTexCoord);
}
