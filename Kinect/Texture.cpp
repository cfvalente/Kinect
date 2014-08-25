#include "Texture.h"
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>

GLuint TextureLoader(GLubyte* data, int width, int height)
{
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	return textureID;
}

GLuint CVImageToGLTexture(cv::Mat image)
{
	int nrow,ncol;
	nrow = (int) image.rows;
	ncol = (int) image.cols;
	GLubyte* tex = new GLubyte[nrow*ncol*3];
	#pragma omp parallel for schedule(static)
	for(int row = 0; row < nrow; row++)
	{
		for(int col = 0; col < ncol; col++)
		{
			tex[3*row*ncol+3*col] = (GLubyte) image.at<cv::Vec3b>(row, col).val[0];
			tex[3*row*ncol+3*col+1] = (GLubyte) image.at<cv::Vec3b>(row, col).val[1];
			tex[3*row*ncol+3*col+2] = (GLubyte) image.at<cv::Vec3b>(row, col).val[2];
		}
	}
	return TextureLoader(tex, ncol, nrow);
	delete [] tex;
}


void TextureUnloader(GLuint *textureId)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1,textureId);
}
