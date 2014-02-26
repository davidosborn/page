// William Donnelly and Andrew Lauritzen, Variance Shadow Maps, 2006

varying float depth;

const float distribution = 32.;

void SetColor()
{
	vec2 moments = vec2(depth, depth * depth);
	gl_FragColor.ba = fract(moments * distribution);
	gl_FragColor.rg = moments - gl_FragColor.ba / distribution;
}
