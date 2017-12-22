#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
in vec3 exSurfPos;
in vec3 height;
in vec3 exGouraudColorVec;

uniform sampler2D objectTexture;
uniform vec3 lightSourcesDirPosArr[10];
uniform vec3 lightSourcesColorArr[10];
uniform int isDirectional[10];
uniform float specularExponent[10];

uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;

void main(void)
{
	float ambientAmount = 0.1;
	float diffuseAmount;
	float specularAmount;
  float dist;
  float blackAmount;


	vec3 ambientVec = vec3(ambientAmount);
	vec3 diffuseVec = vec3(0.0);
	vec3 specularVec = vec3(0.0);
	vec4 totalLightVec = vec4(0.0);
  vec4 remover;

  dist = length(exSurfPos);
  blackAmount = (dist-30);
  blackAmount = max(0, blackAmount)/(60-30);
  blackAmount = clamp(blackAmount, 0.0, 1.0);


	for(int i = 0; i<1; i++){
		if(isDirectional[i]==1){
				vec3 lightDir = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[i]);
				diffuseAmount = max(0,dot(lightDir, normalize(exNormal)))*0;
				diffuseVec = diffuseVec + (diffuseAmount*lightSourcesColorArr[i]);

				vec3 surfToCam =  normalize(- exSurfPos);
				specularAmount = pow(max(0,dot(normalize(2*normalize(exNormal)*dot(lightDir, normalize(exNormal))- lightDir), surfToCam)), specularExponent[i])/80;
				specularVec = specularVec + lightSourcesColorArr[i]*specularAmount;
			}

		}
		totalLightVec = vec4(diffuseVec+ambientVec+specularVec + exGouraudColorVec, 1.0);
		vec4 p = texture(objectTexture, texCoord);
    outColor = vec4(totalLightVec.x*p.r, totalLightVec.y*p.g, totalLightVec.z*p.b, p.a);
/*
    vec3 lightDir = normalize(mat3(cameraMatrix) * lightSourcesDirPosArr[0]);
    float fogAmount = 1.0 - exp( -dist*outColor.z );
    float sunAmount = max( dot( -normalize(exSurfPos), lightDir ), 0.0 );
    vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
                               vec3(1.0,0.9,0.7), // yellowish
                               pow(sunAmount,8.0) );
    vec3 janne = mix(vec3(outColor), fogColor, fogAmount);

    outColor = vec4(janne, 1.0);
*/
    remover = blackAmount * outColor;

    outColor = outColor-remover;





}
