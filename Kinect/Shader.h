#ifndef SHADER
#define SHADER

#include "Include/glew/glew.h"


void printShader(int shaderIndex);

/// <summary>
/// Loads vertex and fragment shaders.
/// </summary>
/// <returns>Returns true if shaders were loaded.</returns>
bool useShader(GLuint &programHandle, int shaderIndex);

int compileShader(GLuint &programHandle, char *vshader_name, char *fshader_name);

#endif