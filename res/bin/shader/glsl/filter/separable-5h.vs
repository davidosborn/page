uniform int texSize;

void main()
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	float texelSize = 1. / float(texSize);
	gl_TexCoord[0] = gl_MultiTexCoord0.stst;
	gl_TexCoord[1] = gl_MultiTexCoord0.stst;
	gl_TexCoord[2] = gl_MultiTexCoord0.stst;
	gl_TexCoord[0].s -= texelSize * 2.;
	gl_TexCoord[0].p -= texelSize;
	gl_TexCoord[1].p += texelSize;
	gl_TexCoord[2].s += texelSize * 2.;
}
