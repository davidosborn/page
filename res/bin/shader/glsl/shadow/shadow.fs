uniform sampler2D diffuseSampler, maskSampler;

void SetColor(); // defined in shadow-*.fs

void main()
{
	float alpha = gl_Color.a *
		texture2D(diffuseSampler, gl_TexCoord[0].st).a *
		texture2D(maskSampler, gl_TexCoord[0].pq).a;
	if (alpha < .5) discard;
	SetColor();
}
