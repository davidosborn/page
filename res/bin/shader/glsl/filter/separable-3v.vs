uniform int texSize;

void main()
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	float texelSize = 1. / float(texSize);
	gl_TexCoord[0] = gl_MultiTexCoord0.stst;
	gl_TexCoord[1] = gl_MultiTexCoord0.stst;
	gl_TexCoord[0].t -= texelSize;
	gl_TexCoord[1].t += texelSize;
}
