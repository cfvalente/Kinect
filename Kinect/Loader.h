#ifndef LOADER
#define LOADER

#include "Include/glew/glew.h"

struct model
{
	GLuint vaoHandle;
	GLuint elementBufferHandle;
	unsigned int num_vertices;
	unsigned int num_faces;
	float *position_data;
	float *uv_data;
	unsigned int *face_data;
};


struct model load_modelColor();

struct model load_modelDepth();

#endif
