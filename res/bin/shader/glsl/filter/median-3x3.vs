uniform ivec2 texSize;

void main()
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	vec2 texelSize = 1. / vec2(texSize);
	for (int i = 0; i < 5; ++i)
		gl_TexCoord[i] = gl_MultiTexCoord0.stst;
	gl_TexCoord[0].stq -= texelSize.xyy;
	gl_TexCoord[1].s   += texelSize.x;
	gl_TexCoord[1].tp  -= texelSize.yx;
	gl_TexCoord[2].p   += texelSize.y;
	gl_TexCoord[3].s   -= texelSize.x;
	gl_TexCoord[3].tq  += texelSize.yy;
	gl_TexCoord[4].st  += texelSize.xy;
}
