#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;


void main(void)
{
	mat3 normalMatrix1 = mat3(cameraMatrix * mdlMatrix);
	exNormal = normalMatrix1*inNormal;
	texCoord = inTexCoord;
	gl_Position = projMatrix * cameraMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
