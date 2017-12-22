#version 150

in  vec3 in_Position;
in vec2 inTexCoord; 
out vec2 exTexCoord;
uniform mat4 camSkyBoxMatrix;
uniform mat4 projectionMatrix;


void main(void)
{

	vec4 transformed = projectionMatrix*camSkyBoxMatrix*vec4(in_Position, 1.0);
	

	exTexCoord = inTexCoord; 
	gl_Position = vec4(transformed);
}
