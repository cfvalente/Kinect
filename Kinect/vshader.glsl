#version 420

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexUV;

uniform mat4 MVP;

out vec2 UV;

void main() 
{
	gl_Position = MVP*vec4(VertexPosition[0],VertexPosition[1],VertexPosition[2],1.0);
	UV = VertexUV;
}
