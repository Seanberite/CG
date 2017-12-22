#version 150

out vec4 outColor;
in vec2 exTexCoord;

uniform sampler2D skyTex;

void main(void)
{
	outColor = texture(skyTex, exTexCoord)*1.5;
}
