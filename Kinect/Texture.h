#ifndef TEXTURE
#define TEXTURE

#include "Texture.h"
#include <glew/glew.h>
#include <GLFW/glfw3.h>


GLuint TextureLoader(GLubyte* data, int width, int height);
void TextureUnloader(GLuint *textureId);


#endif