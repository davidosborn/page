uniform sampler2D diffuseSampler;

vec4 SampleDiffuse()
{
	return texture2D(diffuseSampler, gl_TexCoord[0].st);
}
