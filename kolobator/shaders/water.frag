#version 130
precision mediump float;
varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D tex1;
uniform sampler2D ren;
uniform float time;

void main() {
    vec2 displacement=texture2D (tex1, texCoord/1000.0).xy;
 
    float t= texCoord.y + displacement.y *0.005+ (sin (texCoord.x/10.0 +time/100.0) * 0.0005);
     
    gl_FragColor = color * texture2D (tex, vec2 (texCoord.x, t));
}	