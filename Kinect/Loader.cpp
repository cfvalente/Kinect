#include <iostream>
#include <string>
#include <stdio.h>
#include "Loader.h"
#include "Include/assimp/Importer.hpp"
#include "Include/assimp/scene.h"
#include "Include/assimp/postprocess.h"



#ifdef LOADER_DEBBUG
# define print(x) std::cout << x
#else
# define print(x) do {} while (0)
#endif


float position_data[18];
float uv_data[12];
unsigned int face_data[6];

void load_position_uv(struct model &model_data)
{
	model_data.num_vertices = 4;
	model_data.num_faces = 2;
	position_data[0] = -5.0f;
	position_data[1] = 5.0f;
	position_data[2] = 0.0f;
	position_data[3] = 5.0f;
	position_data[4] = -5.0f;
	position_data[5] = 0.0f;
	position_data[6] = -5.0f;
	position_data[7] = -5.0f;
	position_data[8] = 0.0f;
	position_data[9] = 5.0f;
	position_data[10] = 5.0f;
	position_data[11] = 0.0f;
	uv_data[0] = 1.0f;
	uv_data[1] = 0.0f;
	uv_data[2] = 0.0f;
	uv_data[3] = 1.0f;
	uv_data[4] = 1.0f;
	uv_data[5] = 1.0f;
	uv_data[6] = 0.0f;
	uv_data[7] = 0.0f;
	face_data[0] = 0;
	face_data[1] = 1;
	face_data[2] = 2;
	face_data[3] = 0;
	face_data[4] = 3;
	face_data[5] = 1;
}


struct model load_model()
{
	struct model model_data;


	GLuint vboHandles[2];
	glGenBuffers(1,&vboHandles[0]);
	GLuint positionBufferHandle = vboHandles[0];
	glGenBuffers(1,&vboHandles[1]);
	GLuint UVBufferHandle = vboHandles[1];

	load_position_uv(model_data);

	glGenBuffers(1, &model_data.elementBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,3*model_data.num_vertices*sizeof(float),position_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,UVBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,2*model_data.num_vertices*sizeof(float),uv_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_data.elementBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3* model_data.num_faces * sizeof(GL_UNSIGNED_INT), face_data, GL_STATIC_DRAW);

	glGenVertexArrays(1,&model_data.vaoHandle);
	glBindVertexArray(model_data.vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER,UVBufferHandle);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(GLubyte *)NULL);

	return model_data;
}