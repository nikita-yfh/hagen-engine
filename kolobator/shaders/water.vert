attribute vec3 gpu_Vertex;
attribute vec2 gpu_TexCoord;
attribute vec4 gpu_Color;
uniform mat4 gpu_ModelViewProjectionMatrix;
uniform float time;

varying vec4 color;
varying vec2 texCoord;
#define M_PI 3.1415926535897932384626433832795
#define FPI M_PI/4.0

void main(void)
{
	color = gpu_Color;
	texCoord = vec2(gpu_TexCoord);
	//gl_Position = gpu_ModelViewProjectionMatrix * vec4(gpu_Vertex, 1.0)+vec2;

    float yOff = 0.0;
	float xOff = 0.0;
	//yPos = -1.0;

	//yPos = (gpu_Vertex.y + yOff);
	yOff = 0.5*sin(gpu_Vertex.x*12.0+time*FPI);
	if((gpu_Vertex.x != 1.0 && gpu_Vertex.x != -1.0))
		xOff = 0.025*cos(gpu_Vertex.x*1.0+time*FPI);

    gl_Position = gpu_ModelViewProjectionMatrix * vec4((gpu_Vertex.x - xOff),(gpu_Vertex.y + yOff), gpu_Vertex.z, 1);	
}