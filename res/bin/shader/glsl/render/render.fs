varying vec3 normal;

float GetInvShadow(); // defined in render-shadow{,-none}.fs
vec4 SampleDiffuse(); // defined in sample-diffuse{,-none}.fs
float SampleMask();   // defined in sample-mask{,-none}.fs

void main()
{
	vec3 no = normalize(normal);
	vec4 diffuseColor = SampleDiffuse() * gl_Color;

	// lambertian term
	float lambert = dot(no, gl_LightSource[0].position.xyz);
	float halfLambert = pow(lambert * .5 + .5, 2.);
	vec3 indirectLambert = (lambert *
		(1. - gl_LightModel.ambient.rgb) +
		(1. + gl_LightModel.ambient.rgb)) * .5;

	// rim lighting
	float rim = pow(1. - max(dot(no, vec3(0., 0., 1.)), 0.), 1.);

	// direct/indirect terms
	vec3
		direct   = vec3(halfLambert + rim),
		indirect = indirectLambert;

	gl_FragColor = vec4(
		diffuseColor.rgb * mix(direct * GetInvShadow(), indirect, .6),
		diffuseColor.a * SampleMask());
}
