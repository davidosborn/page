uniform sampler2D sampler;

void main()
{
	gl_FragColor = gl_Color * (
		texture2D(sampler, gl_TexCoord[0].st) +
		texture2D(sampler, gl_TexCoord[0].pq) +
		texture2D(sampler, gl_TexCoord[1].st)) / 3.;
}
