uniform sampler2D maskSampler;

float SampleMask()
{
	return texture2D(maskSampler, gl_TexCoord[1].st).a;
}
