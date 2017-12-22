#version 150

out vec4 out_Color;
in vec3 ex_Normal;
in vec2 exTexCoord;
in vec3 exSurfPos;
uniform sampler2D texUnit1,texUnit2;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform int isDirectional[4];
uniform float specularExponent[4];
uniform mat4 camMatrix;

void main(void)
{	

	float ambient = 0.1;
	float diffuseAmount;
	float specularAmount;
	 
	
	vec4 totalLight = vec4(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	vec3 ambientLight = vec3(ambient);
	
	
	for(int i=0; i<4; ++i) {
		if (isDirectional[i]==0){
			vec4 temp = camMatrix*vec4(lightSourcesDirPosArr[i], 1.0);
			vec3 lightSourcePos = temp.xyz / temp.w;
			vec3 surfToSource = normalize(lightSourcePos - exSurfPos);

			vec3 reflectVec = normalize(2*ex_Normal*dot(surfToSource, ex_Normal)- surfToSource);

			vec3 surfToCam =  normalize(- exSurfPos);
			specularAmount = pow(max(0,dot(reflectVec,surfToCam)), specularExponent[i]);
			
			specular = specular + lightSourcesColorArr[i]*specularAmount;
			

			diffuseAmount = max(0,dot(ex_Normal,surfToSource));
			diffuse = diffuse + lightSourcesColorArr[i] * diffuseAmount;
		}
		else{
			vec3 lightDir= normalize(mat3(camMatrix)*lightSourcesDirPosArr[i]);
			diffuseAmount = max(0,dot(lightDir, ex_Normal));
			diffuse = diffuse + lightSourcesColorArr[i]*diffuseAmount;

			vec3 surfToCam =  normalize(- exSurfPos);
			specularAmount = pow(max(0,dot(normalize(2*ex_Normal*dot(lightDir, ex_Normal)- lightDir), surfToCam)), specularExponent[i]);		
			specular = specular + lightSourcesColorArr[i]*specularAmount;
			
		}


	}

	totalLight = vec4(specular+diffuse+ambient, 1.0);
	
	vec4 dirttex= texture(texUnit2, exTexCoord);
	vec4 maskros= texture(texUnit1, exTexCoord);
	out_Color = totalLight * maskros * dirttex;
}


