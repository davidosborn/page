// Macro Salvi, Rendering Filtered Shadows with Exponential Shadow Maps, ShaderX6, 2008

varying float depth;

void SetColor()
{
	gl_FragColor = depth;
}
