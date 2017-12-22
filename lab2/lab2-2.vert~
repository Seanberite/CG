#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord; 
out vec2 exTexCoord;
out vec3 ex_Normal;
uniform mat4 myMatrix;
uniform mat4 myMatrix2;

void main(void)
{

vec4 transformed = myMatrix*vec4(in_Position, 1.0);
vec4 transformed_Normal = myMatrix*vec4(in_Normal, 1.0);
	

	ex_Normal = vec3(transformed_Normal);
	exTexCoord = inTexCoord; 
	gl_Position = vec4(transformed);
}
