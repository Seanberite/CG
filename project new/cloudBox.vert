#version 150

in  vec3 inPosition;
in vec2 inTexCoord; 
in vec3 inNormal;
out vec2 exTexCoord;
out vec3 exNormal;

uniform mat4 camSkyBoxMatrix;
uniform mat4 projMatrix;


void main(void)
{

	vec4 transformed = projMatrix*camSkyBoxMatrix*vec4(inPosition, 1.0);
	mat3 normalMatrix1 = mat3(camSkyBoxMatrix);
	exNormal = normalMatrix1*inNormal;

	exTexCoord = inTexCoord; 
	gl_Position = vec4(transformed);
}
