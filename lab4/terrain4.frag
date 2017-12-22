#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
in vec3 exSurfPos;
uniform sampler2D texUnit0, texUnit1;


uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

void main(void)
{
	vec3 light = mat3(cameraMatrix) * vec3(0.58, 0.58, 0.58);
	float ambient = 0.3;
	float shade;
	shade  = dot(normalize(exNormal),light);
	shade = clamp(shade, 0, 1);
	float slope = dot(normalize(exNormal), normalize(mat3(cameraMatrix) * vec3(0, 1, 0)));
	//outColor = (shade + ambient) * slope *texture(texUnit1, texCoord) + (shade +ambient) * (1-slope) * texture(texUnit0, texCoord);
	outColor = (shade + ambient) * (exSurfPos.y) *texture(texUnit1, texCoord) + (shade +ambient) * (2 - exSurfPos.y) * texture(texUnit0, texCoord);

}
