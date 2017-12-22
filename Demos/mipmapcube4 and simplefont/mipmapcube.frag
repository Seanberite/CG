#version 150

// Color from vertex shader

in vec2 texcoord;
out vec4 outColor;
uniform sampler2D tex;
uniform int texRepeat;

void main(void)
{
	outColor = texture(tex, texcoord*texRepeat) + vec4(texcoord.x, texcoord.y, 1.0, 1.0); // New st-colorized style
//	outColor = texture(tex, texcoord*texRepeat); // Old black/white style
}
