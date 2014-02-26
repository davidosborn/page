// William Donnelly and Andrew Lauritzen, Variance Shadow Maps, 2006

varying float depth;

void SetDepth()
{
	depth = gl_Position.z / gl_Position.w;
}
