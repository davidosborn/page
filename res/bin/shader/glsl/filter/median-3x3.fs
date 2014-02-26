// Morgan McGuire, A Fast, Small-Radius GPU Median Filter, ShaderX6, 2008

uniform sampler2D sampler;

void minmax(inout vec4 a, inout vec4 b)
{
	vec4 t = a;
	a = min(t, b);
	b = max(t, b);
}

#define m2(a, b)             minmax(a, b)
#define m3(a, b, c)          m2(b, c); m2(a, c); m2(a, b)
#define m4(a, b, c, d)       m2(a, b); m2(c, d); m2(a, c); m2(b, d)
#define m5(a, b, c, d, e)    m2(a, b); m2(c, d); m2(a, c); m2(a, e); m2(d, e); m2(b, e)
#define m6(a, b, c, d, e, f) m2(a, d); m2(b, e); m2(c, f); m2(a, b); m2(a, c); m2(e, f); m2(d, f)

void main()
{
	vec4 r[] = vec4[](
		texture2D(sampler, gl_TexCoord[0].st),
		texture2D(sampler, gl_TexCoord[0].pq),
		texture2D(sampler, gl_TexCoord[1].st),
		texture2D(sampler, gl_TexCoord[1].pq),
		texture2D(sampler, gl_TexCoord[2].st),
		texture2D(sampler, gl_TexCoord[2].pq),
		texture2D(sampler, gl_TexCoord[3].st),
		texture2D(sampler, gl_TexCoord[3].pq),
		texture2D(sampler, gl_TexCoord[4].st));

	m6(r[0], r[1], r[2], r[3], r[4], r[5]);
	m5(r[1], r[2], r[3], r[4], r[6]);
	m4(r[2], r[3], r[4], r[7]);
	m3(r[3], r[4], r[8]);

	gl_FragColor = gl_Color * r[4];
}
