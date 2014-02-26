uniform sampler2D sampler;
uniform float grayness = 1.;
uniform vec3 weight = vec3(1.) / 3.;

void main()
{
	vec4 color = texture2D(sampler, gl_TexCoord[0].st) * gl_Color;
	gl_FragColor = vec4(mix(color.rgb, vec3(dot(color.rgb, weight)), grayness), color.a);
}
