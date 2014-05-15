#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>

#include "Info.h"
#include "Renderer.h"
#include "Shader.h"

using namespace glm;



void rendererColor(GLuint programHandle, mat4 Model, mat4 View, mat4 Projection, int renderingMode, model model_data, GLFWwindow* window)
{
	mat4 MVP, ModelView;

	ModelView = View * Model;
	MVP = Projection * ModelView;
	GLuint location;

	uniformToShader4mat(programHandle,"MVP", MVP);

	glBindVertexArray(model_data.vaoHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model_data.elementBufferHandle);
	//uniformToShader3f(programHandle,"Kd", vec3(model_data.material_kd[model_data.mesh_material[m]][0],model_data.material_kd[model_data.mesh_material[m]][1],model_data.material_kd[model_data.mesh_material[m]][2]));
	//uniformToShader1f(programHandle,"Shininess", model_data.material_shininess[model_data.mesh_material[m]]);

	//glDrawArrays(GL_TRIANGLES, 0, 2*3);
	glDrawElements(GL_TRIANGLES, 3 * model_data.num_faces * sizeof(GL_UNSIGNED_INT), GL_UNSIGNED_INT, (void*)0);

}

void rendererDepth(GLuint programHandle, mat4 Model, mat4 View, mat4 Projection, int renderingMode, model model_data, GLFWwindow* window)
{
	mat4 MVP, ModelView;
	

	ModelView = View * Model;
	MVP = Projection * ModelView;
	GLuint location;

	uniformToShader4mat(programHandle,"MVP", MVP);

	glBindVertexArray(model_data.vaoHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,model_data.elementBufferHandle);
	//uniformToShader3f(programHandle,"Kd", vec3(model_data.material_kd[model_data.mesh_material[m]][0],model_data.material_kd[model_data.mesh_material[m]][1],model_data.material_kd[model_data.mesh_material[m]][2]));
	//uniformToShader1f(programHandle,"Shininess", model_data.material_shininess[model_data.mesh_material[m]]);

	//glDrawArrays(GL_TRIANGLES, 0, 2*3);
	glDrawElements(GL_TRIANGLES, 3 * model_data.num_faces * sizeof(GL_UNSIGNED_INT), GL_UNSIGNED_INT, (void*)0);

}





void uniformToShader4mat(GLuint programHandle,char *var, glm::mat4 value)
{
	GLuint location = glGetUniformLocation(programHandle,var);
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void uniformToShader3mat(GLuint programHandle,char *var, glm::mat3 value)
{
	GLuint location = glGetUniformLocation(programHandle,var);
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void uniformToShader4f(GLuint programHandle,char *var, glm::vec4 value)
{
	GLuint location = glGetUniformLocation(programHandle,var);
	glUniform4f(location,value[0],value[1],value[2],value[3]);
}

void uniformToShader3f(GLuint programHandle,char *var, glm::vec3 value)
{
	GLuint location = glGetUniformLocation(programHandle,var);
	glUniform3f(location,value[0],value[1],value[2]);
}

void uniformToShader1f(GLuint programHandle,char *var, float value)
{
	GLuint location = glGetUniformLocation(programHandle,var);
	glUniform1f(location,value);
}