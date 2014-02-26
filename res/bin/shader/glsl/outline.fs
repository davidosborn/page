// Döllner and Nienhaus, Blueprint Rendering and Sketchy Drawings, GPU Gems 2, 2005
// NOTE: requires GLSL 1.2r6 for array initializers

uniform sampler2D normalSampler;

void main()
{
	vec3 normal[] = vec3[4](
		normalize(texture2D(normalSampler, gl_TexCoord[0].st).xyz * 2. - 1.),
		normalize(texture2D(normalSampler, gl_TexCoord[0].pq).xyz * 2. - 1.),
		normalize(texture2D(normalSampler, gl_TexCoord[1].st).xyz * 2. - 1.),
		normalize(texture2D(normalSampler, gl_TexCoord[1].pq).xyz * 2. - 1.));
	float continuity = (
		dot(normal[0], normal[2]) +
		dot(normal[1], normal[3])) * .5;
/*	float depthContinuity =
		(1. - abs(depth[0] - depth[2])) *
		(1. - abs(depth[1] - depth[3]));*/
	gl_FragColor = vec4(vec3(continuity), 1.);
}
