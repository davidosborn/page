// William Donnelly and Andrew Lauritzen, Variance Shadow Maps, 2006

uniform sampler2D shadowSampler;

varying vec4 shadowCoord;

const float
	distribution = 32.,
	threshold    = .25;

float GetInvShadowImpl()
{
	vec4 sample = texture2D(shadowSampler, shadowCoord.xy);
	vec2 moments = sample.rg + sample.ba / distribution;
	float
		factor = shadowCoord.z <= moments.x,
		variance = min(max(moments.y - moments.x * moments.x, 0.) + .00001, 1.),
		mean = moments.x - shadowCoord.z,
		pmax = variance / (variance + mean * mean);
	return max(factor, smoothstep(threshold, 1., pmax));
}
