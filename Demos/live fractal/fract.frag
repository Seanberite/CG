#version 150

out vec4 outColor;
in vec2 texCoord;

uniform float juliaRe;
uniform float juliaIm;

void main(void)
{
	float re = texCoord.x;
	float im = texCoord.y;
	
	float startre = re;
	float startim = im;
	const float kOuterBound = 10.0;
	int i;
	
	for (i = 0; i < 50; i++)
	{
		float re2 = re*re - im*im;
		float im2 = re*im + im*re;
		
		re2 += startre;
		im2 += startim;
//		re2 += juliaRe;
//		im2 += juliaIm;
		
		re = re2;
		im = im2;
		
		if (re2 > kOuterBound) break;
	}
	
	outColor = vec4(
			sin(i * 0.5),
			cos(i * 0.5),
			sin(2*i * 0.2 + 1.0),
			1.0);
}
