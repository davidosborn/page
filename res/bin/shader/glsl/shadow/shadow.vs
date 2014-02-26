void SetDepth(); // defined in shadow-*.vs

void main()
{
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.st, gl_MultiTexCoord1.st);
	SetDepth();
}
