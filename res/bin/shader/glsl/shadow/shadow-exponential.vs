// Macro Salvi, Rendering Filtered Shadows with Exponential Shadow Maps, ShaderX6, 2008

varying float depth;

void SetDepth()
{
	depth = gl_Position.z / gl_Position.w;
}
