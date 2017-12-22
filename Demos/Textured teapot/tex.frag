#version 150

out vec4 outColor;
in vec3 exNormal; // Phong
in vec2 texCoord;
uniform sampler2D tex;

void main(void)
{
//	out_Color = vec4(1.0);
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	
	shade = dot(normalize(exNormal), light);

// Shading only:
//	outColor = vec4(shade, shade, shade, 1.0); // inColor;

// Shading + texture
	outColor = shade * texture(tex, texCoord);

// Texture only:
//	outColor = texture(tex, texCoord);
}
