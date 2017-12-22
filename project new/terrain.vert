#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;
out vec3 height;
out vec3 exSurfPos;
out vec3 exGouraudColorVec;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

uniform vec3 lightSourcesDirPosArr[10];
uniform vec3 lightSourcesColorArr[10];
uniform int isDirectional[10];


void main(void)
{
	float diffuseAmount;

	vec3 diffuseVec = vec3(0.0);
	exGouraudColorVec = vec3(0.0);

	mat3 normalMatrix1 = mat3(cameraMatrix) * mat3(mdlMatrix);
	exNormal = normalMatrix1*inNormal;
	texCoord = inTexCoord;
	vec4 temp = mdlMatrix*vec4(inPosition, 1.0);
	height = temp.xyz / temp.w;
	temp = cameraMatrix * mdlMatrix*vec4(inPosition, 1.0);
	exSurfPos = temp.xyz / temp.w;


	for(int i = 0; i<10; i++){
		if(isDirectional[i]==0){  // for fireflies

			vec4 temp = cameraMatrix*vec4(lightSourcesDirPosArr[i], 1.0);
			vec3 lightSourcePos = temp.xyz / temp.w;
			vec3 surfToSource = normalize(lightSourcePos - exSurfPos);

			diffuseAmount = pow(max(0,dot(exNormal,surfToSource)), 8)/length(exSurfPos)*5;
			diffuseVec = diffuseVec + (lightSourcesColorArr[i] * diffuseAmount);

		}
	}
	exGouraudColorVec = diffuseVec;
	gl_Position = projMatrix * cameraMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
