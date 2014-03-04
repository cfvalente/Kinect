#ifndef LOADER
#define LOADER

#include "Include/glew/glew.h"

struct model
{
	GLuint vaoHandle;
	GLuint elementBufferHandle;
	unsigned int num_vertices;
	unsigned int num_faces;
};


struct model load_model();

#endif
