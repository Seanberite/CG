#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord; 
out vec2 exTexCoord;
out vec3 ex_Normal;
uniform mat4 mdlMatrix;
uniform mat4 projectionMatrix;


void main(void)
{
mat3 normalMatrix = mat3(mdlMatrix);
vec4 transformed = projectionMatrix*mdlMatrix*vec4(in_Position, 1.0);
vec4 transformed_Normal = vec4(in_Normal, 1.0);
	

	ex_Normal = normalMatrix*vec3(transformed_Normal);
	exTexCoord = inTexCoord; 
	gl_Position = vec4(transformed);
}
