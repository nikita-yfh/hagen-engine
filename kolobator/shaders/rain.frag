#version 120
varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform sampler2D tex1;
uniform float time;
uniform float zoom;

void main() {
	vec2 q = texCoord;
    vec4 col=texture2D(tex, q);
	
	q = gl_FragCoord.xy/resolution.xy;
	float dis = 1.;
	for (int i = 0; i < 12; i++)
	{
		float f = pow(dis, .45)+.25;

		vec2 st =  f * (q * vec2(1.5, .05)+vec2(-time*.001+q.y*.5, time*.0012));
		f = (texture2D(tex1, st * .5, -99.0).x + texture2D(tex1, st*.284, -99.0).y);
		f = clamp(pow(abs(f)*.5, 29.0) * 140.0, 0.00, q.y*.4+.05);

		vec4 bri = vec4(.5);
		col += bri*f;
		dis += 3.5;
	}
	col = pow(col, vec4(1.1));
	gl_FragColor = col;
}