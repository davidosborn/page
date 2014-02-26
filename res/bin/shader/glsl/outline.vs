// Döllner and Nienhaus, Blueprint Rendering and Sketchy Drawings, GPU Gems 2, 2005

uniform ivec2 texSize;

const float lineWidth = .75;

void main()
{
	gl_Position = ftransform();
	vec2 sampleSize = lineWidth / vec2(texSize);
	gl_TexCoord[0] = vec4(
		gl_MultiTexCoord0.s - sampleSize.x, gl_MultiTexCoord0.t - sampleSize.y,
		gl_MultiTexCoord0.s + sampleSize.x, gl_MultiTexCoord0.t - sampleSize.y);
	gl_TexCoord[1] = vec4(
		gl_MultiTexCoord0.s + sampleSize.x, gl_MultiTexCoord0.t + sampleSize.y,
		gl_MultiTexCoord0.s - sampleSize.x, gl_MultiTexCoord0.t + sampleSize.y);
}
