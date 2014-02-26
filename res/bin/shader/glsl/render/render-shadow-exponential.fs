// Macro Salvi, Rendering Filtered Shadows with Exponential Shadow Maps, ShaderX6, 2008

uniform sampler2D shadowSampler;

varying vec4 shadowCoord;

const float k = 16.f;

float GetInvShadowImpl()
{
	float moment = texture2D(shadowSampler, shadowCoord.xy).r;
	return clamp(exp(k * (moment - shadowCoord.z)), 0.f, 1.f);
}
