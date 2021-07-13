precision mediump float;
varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D light;

void main() {
	gl_FragColor = texture2D(tex, texCoord) * texture2D(light, texCoord) * 3.0;
}