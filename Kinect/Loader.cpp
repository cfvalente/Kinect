#include <iostream>
#include <string>
#include <stdio.h>
#include "Loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#ifdef LOADER_DEBBUG
# define print(x) std::cout << x
#else
# define print(x) do {} while (0)
#endif



struct model load_position_uvColor()
{
	struct model model_data;
	model_data.num_vertices = 4;
	model_data.num_faces = 2;
	model_data.position_data = new float [18];
	model_data.uv_data = new float [12];
	model_data.face_data = new unsigned int [6];
	model_data.position_data[0] = -5.0f;
	model_data.position_data[1] = 5.0f;
	model_data.position_data[2] = 0.0f;
	model_data.position_data[3] = 5.0f;
	model_data.position_data[4] = -5.0f;
	model_data.position_data[5] = 0.0f;
	model_data.position_data[6] = -5.0f;
	model_data.position_data[7] = -5.0f;
	model_data.position_data[8] = 0.0f;
	model_data.position_data[9] = 5.0f;
	model_data.position_data[10] = 5.0f;
	model_data.position_data[11] = 0.0f;
	model_data.uv_data[0] = 1.0f;
	model_data.uv_data[1] = 0.0f;
	model_data.uv_data[2] = 0.0f;
	model_data.uv_data[3] = 1.0f;
	model_data.uv_data[4] = 1.0f;
	model_data.uv_data[5] = 1.0f;
	model_data.uv_data[6] = 0.0f;
	model_data.uv_data[7] = 0.0f;
	model_data.face_data[0] = 0;
	model_data.face_data[1] = 1;
	model_data.face_data[2] = 2;
	model_data.face_data[3] = 0;
	model_data.face_data[4] = 3;
	model_data.face_data[5] = 1;
	return model_data;
}

struct model load_position_uvDepth()
{
	struct model model_data;
	model_data.num_vertices = 4;
	model_data.num_faces = 2;
	model_data.position_data = new float [18];
	model_data.uv_data = new float [12];
	model_data.face_data = new unsigned int [6];
	model_data.position_data[0] = 5.0f;
	model_data.position_data[1] = 5.0f;
	model_data.position_data[2] = 0.0f;
	model_data.position_data[3] = 15.0f;
	model_data.position_data[4] = -5.0f;
	model_data.position_data[5] = 0.0f;
	model_data.position_data[6] = 5.0f;
	model_data.position_data[7] = -5.0f;
	model_data.position_data[8] = 0.0f;
	model_data.position_data[9] = 15.0f;
	model_data.position_data[10] = 5.0f;
	model_data.position_data[11] = 0.0f;
	model_data.uv_data[0] = 1.0f;
	model_data.uv_data[1] = 0.0f;
	model_data.uv_data[2] = 0.0f;
	model_data.uv_data[3] = 1.0f;
	model_data.uv_data[4] = 1.0f;
	model_data.uv_data[5] = 1.0f;
	model_data.uv_data[6] = 0.0f;
	model_data.uv_data[7] = 0.0f;
	model_data.face_data[0] = 0;
	model_data.face_data[1] = 1;
	model_data.face_data[2] = 2;
	model_data.face_data[3] = 0;
	model_data.face_data[4] = 3;
	model_data.face_data[5] = 1;
	return model_data;
}


struct model load_modelColor()
{
	struct model model_data;


	GLuint vboHandles[2];
	glGenBuffers(1,&vboHandles[0]);
	GLuint positionBufferHandle = vboHandles[0];
	glGenBuffers(1,&vboHandles[1]);
	GLuint UVBufferHandle = vboHandles[1];

	model_data = load_position_uvColor();

	glGenBuffers(1, &model_data.elementBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,3*model_data.num_vertices*sizeof(float),model_data.position_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,UVBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,2*model_data.num_vertices*sizeof(float),model_data.uv_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_data.elementBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3* model_data.num_faces * sizeof(GL_UNSIGNED_INT), model_data.face_data, GL_STATIC_DRAW);

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


struct model load_modelDepth()
{
	struct model model_data;


	GLuint vboHandles[2];
	glGenBuffers(1,&vboHandles[0]);
	GLuint positionBufferHandle = vboHandles[0];
	glGenBuffers(1,&vboHandles[1]);
	GLuint UVBufferHandle = vboHandles[1];

	model_data = load_position_uvDepth();

	glGenBuffers(1, &model_data.elementBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,3*model_data.num_vertices*sizeof(float),model_data.position_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,UVBufferHandle);
	glBufferData(GL_ARRAY_BUFFER,2*model_data.num_vertices*sizeof(float),model_data.uv_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_data.elementBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3* model_data.num_faces * sizeof(GL_UNSIGNED_INT), model_data.face_data, GL_STATIC_DRAW);

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