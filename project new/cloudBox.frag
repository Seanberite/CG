#version 150

out vec4 outColor;
in vec2 exTexCoord;
in vec3 exNormal;
uniform sampler2D cloudTex;


// Används för att vrida ljuskällan lika mycket som världen men med en annan vinkel än molnen
uniform mat4 cameraMatrix;

void main(void)
{

	vec3 light = normalize(mat3(cameraMatrix) * vec3(0.8, 0.3, 1.0));
	vec4 p = texture(cloudTex, exTexCoord);
	float shade;
	float pa = pow(p.a, 2);
	shade  = pow(dot(normalize(exNormal),light)/1.3, 3);
	shade = clamp(shade, 0, 1);
	outColor = vec4(pa*shade*p.r, pa*0.7*shade*p.g, pa*0.7*shade*p.b, p.a);

}
