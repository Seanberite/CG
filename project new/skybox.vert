#version 150

in vec3 inPosition;
in vec2 inTexCoord; 
out vec2 exTexCoord;

uniform mat4 camSkyBoxMatrix;
uniform mat4 projMatrix;

void main(void)
{
	vec4 transformed = projMatrix*camSkyBoxMatrix*vec4(inPosition, 1.0);

	exTexCoord = inTexCoord; 
	gl_Position = vec4(transformed);
}
