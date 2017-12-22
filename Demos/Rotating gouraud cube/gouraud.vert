#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out float shade;

// NY
uniform mat4 myMatrix;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	
	vec3 normal = mat3(myMatrix) * inNormal; // Rotate normal (simple normal matrix, uniform scaling only)
	shade = dot(normalize(normal), light);

	gl_Position = myMatrix * vec4(inPosition, 1.0);
}
