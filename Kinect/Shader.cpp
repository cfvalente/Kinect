#include <iostream>
#include <vector>

#include "Shader.h"
#include "File.h"
#include "Info.h"

using namespace std;

struct shader 
{
	GLuint fshader;
	GLuint vshader;
	GLchar *fshader_name;
	GLchar *vshader_name;
};

vector<struct shader> shaders;

void shaderInit()
{
}

void printShader(int shaderIndex)
{
	if(shaderIndex >= shaders.size())
	{
		cout << "Error Shader: Out of bound index\n";
		return;
	}
	cout << "Vertex Shader name: " << shaders[shaderIndex].vshader_name << "\n";
	cout << "Fragment Shader name: " << shaders[shaderIndex].fshader_name << "\n";
}


int compileShader(GLuint &programHandle, char *vshader_name, char *fshader_name)
{
	GLint size[2];
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	programHandle = glCreateProgram();
	if(vshader == 0) cout << "Error Shader: Vertex Shader\n";
	if(fshader == 0) cout << "Error Shader: Fragment Shader\n";
	const GLchar *vshadercode = read_file(vshader_name);
	const GLchar *fshadercode = read_file(fshader_name);
	const GLchar *vcodeArray[] = {vshadercode};
	const GLchar *fcodeArray[] = {fshadercode};
	size[0] = (int) strlen(vshadercode);
	size[1] = (int) strlen(fshadercode);
	glShaderSource(vshader,1,vcodeArray,NULL);
	glShaderSource(fshader,1,fcodeArray,NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);

	if(compilationStatus(vshader,"Vertex Shader") != false && compilationStatus(fshader, "Fragment Shader") != false)
	{
		shader res;
		res.fshader = fshader;
		res.vshader = vshader;
		res.fshader_name = fshader_name;
		res.vshader_name = vshader_name;
		shaders.push_back(res);
	}
	glAttachShader(programHandle,shaders[shaders.size()-1].vshader);
	glAttachShader(programHandle,shaders[shaders.size()-1].fshader);

	glLinkProgram(programHandle);
	if(linkStatus(programHandle))
	{
		return true;
	}
	return false;
}
