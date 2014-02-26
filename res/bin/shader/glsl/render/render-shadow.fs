varying vec4 shadowCoord;

const float boundarySize = .0625;

float GetInvShadowImpl(); // defined in render-shadow-*.fs

float FadeShadowBoundary()
{
	vec4 coord = vec4(shadowCoord.xy, 1. - shadowCoord.xy);
	vec4 mu = clamp(coord / boundarySize, 0., 1.);
	vec2 mu2 = min(mu.xy, mu.zw);
	return min(mu2.x, mu2.y);
}

float GetInvShadow()
{
	return GetInvShadowImpl() * FadeShadowBoundary();
}
