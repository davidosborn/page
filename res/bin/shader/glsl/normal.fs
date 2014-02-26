uniform sampler2D diffuseSampler, maskSampler;

varying vec3 normal;

void main()
{
	gl_FragColor = vec4(normal * .5 + .5, gl_Color.a *
		texture2D(diffuseSampler, gl_TexCoord[0].st).a *
		texture2D(maskSampler, gl_TexCoord[0].pq).a);
}
