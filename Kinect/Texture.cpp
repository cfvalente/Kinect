#include "Texture.h"
#include <glew/glew.h>
#include <GLFW/glfw3.h>


GLuint TextureLoader(GLubyte* data, int width, int height)
{
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	return textureID;
}


void TextureUnloader(GLuint *textureId)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1,textureId);
}
