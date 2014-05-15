#include <iostream>
#include <string>

#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"

#include "Include/glm/glm.hpp"
#include "Include/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Controls.h"
#include "Renderer.h"
#include "Loader.h"

#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

#include "Kinect.h"
#include "Texture.h"



#define widthColor 1280
#define heightColor 960
#define widthDepth 640
#define heightDepth 480

using namespace std;
using namespace glm;

GLuint programHandle;

mat4 ModelView;
mat4 Model;
mat4 View;
mat4 Projection;
mat3 Normal;

vec3 position = vec3(0.0f,0.0f,40.0f);
vec3 direction = vec3(0.0f, 0.0f,-1.0f);
vec3 up = vec3(0.0f,1.0f,0.0f);

int renderingMode;

struct model model_dataColor;
struct model model_dataDepth;

GLFWwindow* window;

char *vshader_name = "vshader.glsl";
char *fshader_name = "fshader.glsl";


// OpenGL Variables
GLuint textureIdColor;              // ID of the texture to contain Kinect RGB Data
GLubyte dataColor[widthColor*heightColor*4];  // BGRA array containing the texture data

GLuint textureIdDepth;              // ID of the texture to contain Kinect Depth Data
GLubyte dataDepth[widthDepth*heightDepth*4];  // BGRA array containing the texture data

// Kinect variables
HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera
HANDLE depthStream;              // The identifier of the Kinect's Depth Camera
INuiSensor* sensor;            // The kinect sensor


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool a = 0;
	if (keyboard(key, scancode, action, mods, renderingMode, position, direction, up) == CLOSE_WINDOW)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if(a) { cout << "A"; a = !a; }
		else { cout << "B"; a = !a; }
	}
}

void mouse_position_callback(GLFWwindow* window, double x, double y)
{
	int size_x,size_y;
	glfwGetWindowSize(window,&size_x,&size_y);
	glfwGetCursorPos(window,&x, &y);
	glfwSetCursorPos(window,(size_x/2.0),(size_y/2.0));
	mouse_movement(x,y,size_x,size_y,direction,up);
}

/* Inicializacao de variaveis e objetos */
void GL_init(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(widthColor, heightColor, "Kinect", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE; 
	glewInit();
	glViewport(0,0,widthColor,heightColor);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f);

	renderingMode = rendering::all;

	Projection = glm::perspective(45.0f, 1280.0f / 1024.0f, 0.1f, 500.0f);

	Model = mat4(1.0);

	View = glm::lookAt(position,position+direction,up);

	model_dataColor = load_modelColor();
	model_dataDepth = load_modelDepth();

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window,mouse_position_callback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}

void movement()
{   
	View = lookAt(position,position+direction,up);
}



int main(int argc, char *argv[])
{
	GL_init(argc,argv);
	if(!Kinect_init(sensor, rgbStream, depthStream)) return 1;
	if(!useShader(programHandle,compileShader(programHandle,vshader_name,fshader_name))) return 1;
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		movement();
		getKinectColorData(dataColor, sensor, rgbStream, widthColor, heightColor);
		textureIdColor = TextureLoader(dataColor, widthColor, heightColor);
		rendererColor(programHandle, Model, View, Projection, renderingMode, model_dataColor, window);

		getKinectDepthData(dataDepth, sensor, depthStream, widthDepth, heightDepth);
		textureIdDepth = TextureLoader(dataDepth, widthDepth, heightDepth);
		rendererDepth(programHandle, Model, View, Projection, renderingMode, model_dataDepth, window);

		glfwSwapBuffers(window);

		TextureUnloader(&textureIdColor);
		TextureUnloader(&textureIdDepth);
	}
	glfwTerminate();
	return 0;
}