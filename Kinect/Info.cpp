#include <iostream>

#include "Info.h"



void printData()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION,&major);
	glGetIntegerv(GL_MINOR_VERSION,&minor);
	printf("GL Vendor     : %s\n",vendor);
	printf("GL Renderer   : %s\n",renderer);
	printf("GL Version    : %s\n",version);
	printf("GL Version int: %d %d\n",major,minor);
	printf("GLSL Version  : %s\n",glslVersion);
}


bool compilationStatus(GLuint shader,char *shader_name)
{
	GLint res,loglen;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&res);
	if(res == GL_FALSE) 
	{
			printf("%s: Compilation error\n", shader_name);
			glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&loglen);
			if(loglen > 0)
			{
				char *log = (char *)malloc(sizeof(char)*loglen);
				GLsizei written;
				glGetShaderInfoLog(shader,loglen,&written,log);
				printf("Shader log:\n%s\n",log);
				free(log);
			}
			return false;

	}
	return true;
}


bool linkStatus(GLuint programHandle)
{
	GLint status,loglen;
	glGetProgramiv(programHandle,GL_LINK_STATUS,&status);
	if(status == GL_FALSE)
	{
		printf("Erro ao linkar\n");
		glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH,&loglen);
		if(loglen > 0)
		{
			char *log = (char *)malloc(loglen);
			GLsizei written;
			glGetProgramInfoLog(programHandle,loglen,&written,log);
			printf("Program Log:\n%s\n",log);
			free(log);
			return false;
		}
	}
	return true;
}


void printMatrix(glm::mat4 M)
{
	for(int i = 0;i < 4; i++)
	{
		std::cout << "M[" << i << "]  " << M[0][i] << "  " << M[1][i] << "  " << M[2][i] << "  " << M[3][i] << "\n";
	}
}