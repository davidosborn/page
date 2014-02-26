// NOTE: requires GLSL 1.2r6 for array initializers

uniform sampler2D sampler;
uniform float kernel[] = float[3](1./3., 1./3., 1./3.);

void main()
{
	gl_FragColor = gl_Color * (
		texture2D(sampler, gl_TexCoord[0].st) * kernel[0] +
		texture2D(sampler, gl_TexCoord[0].pq) * kernel[1] +
		texture2D(sampler, gl_TexCoord[1].st) * kernel[2]);
}
