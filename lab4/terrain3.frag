#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
uniform sampler2D tex;


uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

void main(void)
{
	vec3 light = mat3(cameraMatrix) * vec3(0.58, 0.58, 0.58);
	float shade;
	shade  = dot(normalize(exNormal),light);
	shade = clamp(shade, 0, 1);
	outColor = shade * texture(tex, texCoord);
}
