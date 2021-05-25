varying vec4 color;
varying vec2 texCoord;

uniform vec2 resolution;
uniform sampler2D tex;
uniform sampler2D tex1;
uniform float time;
uniform float zoom;

void main() {
	gl_FragColor = texture2D(tex, q);
}	