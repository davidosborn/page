uniform mat4 shadowMatrix;

varying vec3 shadowCoord;

void SetShadowCoord()
{
	vec4 shadowClipCoord = shadowMatrix * gl_ModelViewMatrix * gl_Vertex;
	shadowCoord = shadowClipCoord.xyz / shadowClipCoord.w;
	shadowCoord.xy = shadowCoord.xy * .5 + .5;
}
