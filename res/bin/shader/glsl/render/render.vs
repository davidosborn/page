varying vec3 normal;

void SetShadowCoord(); // defined render-shadow{,-none}.vs

void main()
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	gl_TexCoord[0].st = (gl_TextureMatrix[0] * gl_MultiTexCoord0).st;
	gl_TexCoord[1].st = (gl_TextureMatrix[1] * gl_MultiTexCoord1).st;
	gl_TexCoord[2].st = (gl_TextureMatrix[2] * gl_MultiTexCoord2).st;
	gl_TexCoord[3].st = (gl_TextureMatrix[3] * gl_MultiTexCoord3).st;
	gl_TexCoord[4].st = (gl_TextureMatrix[4] * gl_MultiTexCoord4).st;
	gl_TexCoord[5].st = (gl_TextureMatrix[5] * gl_MultiTexCoord5).st;
	gl_TexCoord[6].st = (gl_TextureMatrix[6] * gl_MultiTexCoord6).st;
	gl_TexCoord[7].st = (gl_TextureMatrix[7] * gl_MultiTexCoord7).st;
	normal = gl_NormalMatrix * gl_Normal.xyz;
	SetShadowCoord();
}
