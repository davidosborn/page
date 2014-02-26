uniform sampler2D sampler;

void main()
{
	gl_FragColor = gl_Color * log((
		exp(texture2D(sampler, gl_TexCoord[0].st)) +
		exp(texture2D(sampler, gl_TexCoord[0].pq)) +
		exp(texture2D(sampler, gl_TexCoord[1].st))) / 3.);
}
