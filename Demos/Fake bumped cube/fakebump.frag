#version 150

// Simplified Phong: No materials, only one, hard coded light source
// and no ambient

in vec2 texcoord;
uniform sampler2D tex;

out vec4 outColor;
in vec3 exNormal; // Phong
in vec3 exSurface; // Phong (specular)

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates!
	float diffuse, specular, shade;

	// Fake Phong
	const float step = 1.0/64.0; // Should match size of texture
	float bt = texture(tex, texcoord).r - texture(tex, texcoord + vec2(0.0, step)).r;
	float bs = texture(tex, texcoord).r - texture(tex, texcoord + vec2(step, 0.0)).r;
	// fake it
	vec3 ps, pt;
	ps = 2.0 * cross(exNormal, vec3(1.0, 0.0, 0.0));
	pt = 2.0 * cross(exNormal, ps);
	vec3 n = exNormal + ps * bs + pt * bt;

	
	// Diffuse
//	diffuse = dot(normalize(exNormal), light);
	diffuse = dot(normalize(n), light);
	diffuse = max(0.0, diffuse); // No negative light

	
	// Specular
	vec3 r = reflect(-light, normalize(n));
//	vec3 r = reflect(-light, normalize(exNormal));
	vec3 v = normalize(-exSurface); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 150.0);
	specular = max(specular, 0.0);
	shade = 0.7*diffuse + 1.0*specular;
	outColor = vec4(shade, shade, shade, 1.0);

//	outColor = shade * texture(tex, texcoord);
}
