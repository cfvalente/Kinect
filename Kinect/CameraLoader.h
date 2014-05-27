#ifndef CAMLOADER
#define CAMLOADER

#include <glew/glew.h>

struct camera_model
{
	GLuint vaoHandle;
	GLuint elementBufferHandle;
	unsigned int num_vertices;
	unsigned int num_faces;
	float *position_data;
	float *uv_data;
	unsigned int *face_data;
};


struct camera_model load_modelColor(GLfloat width, GLfloat height);

struct camera_model load_modelDepth(GLfloat width, GLfloat height);

#endif
