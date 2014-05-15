#ifndef TEXTURE
#define TEXTURE

#include "Texture.h"
#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"


GLuint TextureLoader(GLubyte* data, int width, int height);
void TextureUnloader(GLuint *textureId);


#endif