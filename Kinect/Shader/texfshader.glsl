#version 420

in vec2 UV;

uniform sampler2D myTextureSampler;

out vec4 FragColor;




void main ()  
{     
	FragColor = texture( myTextureSampler, UV ).rgba;
}  