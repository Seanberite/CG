#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 exNormal; // Phong

uniform mat4 mdlMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat3 normalMatrix = mat3(mdlMatrix); // Cheap normal matrix
	exNormal = normalMatrix * inNormal; // Phong

	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
