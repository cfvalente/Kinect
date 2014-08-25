#ifndef TEXTURE
#define TEXTURE

#include "Texture.h"
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


GLuint TextureLoader(GLubyte* data, int width, int height);
void TextureUnloader(GLuint *textureId);
GLuint CVImageToGLTexture(cv::Mat image);


#endif