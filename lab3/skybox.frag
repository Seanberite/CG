#version 150

out vec4 out_Color;
in vec2 exTexCoord;
uniform sampler2D texUnit0;

void main(void)
{
	out_Color = texture(texUnit0, exTexCoord);
}
