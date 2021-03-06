#ifndef INFO
#define INFO

#include <glew/glew.h>
#include <glm/glm.hpp>


///// <summary>
/// Prints GPU info
/// </summary>
void printData();


///// <summary>
/// Prints shader compilation status and returns true for successful and false for failure
/// <para> GLuint shader </para>
/// </summary>
bool compilationStatus(GLuint, char *shader_name);

///// <summary>
/// Prints shader link status and returns true for successful and false for failure
/// <para> GLuint shader </para>
/// </summary>
bool linkStatus(GLuint);

///// <summary>
/// Prints the elements of the transpose matrix
/// <para> glm::mat4 </para>
/// </summary>
void printMatrix(glm::mat4);

#endif