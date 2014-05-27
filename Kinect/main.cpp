#include <iostream>
#include <string>

#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Controls.h"
#include "Renderer.h"
#include "ModelLoader.h"
#include "CameraLoader.h"

#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>

#include "Kinect.h"
#include "Texture.h"

#include <opencv2/core/core.hpp>
#include <aruco\aruco.h>
#include <opencv2/highgui/highgui.hpp>

#define widthColor 1280
#define heightColor 960
#define widthDepth 640
#define heightDepth 480

#define widthColorf 1280.0f
#define heightColorf 960.0f
#define widthDepthf 640.0f
#define heightDepthf 480.0f

#define orthoZNear 0.1f
#define orthoZFar 4050.0f

using namespace std;
using namespace glm;

GLuint programHandle;

mat4 ModelView;
mat4 Model;
mat4 View;
mat4 OrthoProjection;
mat4 Projection;
mat3 Normal;

vec3 position = vec3(0.0f,0.0f,10.0f);
vec3 direction = vec3(0.0f, 0.0f,-1.0f);
vec3 up = vec3(0.0f,1.0f,0.0f);

int renderingMode;

struct camera_model model_dataColor;
//struct camera_model model_dataDepth;

GLFWwindow* window;

char *texvshader_name = "Shader/texvshader.glsl";
char *texfshader_name = "Shader/texfshader.glsl";

char *modvshader_name = "Shader/modvshader.glsl";
char *modfshader_name = "Shader/modfshader.glsl";


// OpenGL Variables
GLuint textureIdColor;              // ID of the texture to contain Kinect RGB Data
GLubyte dataColor[widthColor*heightColor*4];  // BGRA array containing the texture data

GLuint textureIdDepth;              // ID of the texture to contain Kinect Depth Data
GLubyte dataDepth[widthDepth*heightDepth*4];  // BGRA array containing the texture data

// Kinect variables
HANDLE rgbStream;              // The identifier of the Kinect's RGB Camera
HANDLE depthStream;              // The identifier of the Kinect's Depth Camera
INuiSensor* sensor;            // The kinect sensor

// ArUco variables
aruco::CameraParameters CamParam;
vector<aruco::Marker> Markers;

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

	Projection = glm::perspective(45.0f, widthColorf / heightColorf, 0.1f, 500.0f);
	OrthoProjection = glm::ortho(0.0f,widthColorf,0.0f,heightColorf, orthoZNear, orthoZFar);

	Model = mat4(1.0);

	View = glm::lookAt(position,position+direction,up);

	model_dataColor = load_modelColor(widthColorf,heightColorf);
	//model_dataDepth = load_modelDepth(widthDepthf,heightDepthf);

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window,mouse_position_callback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}

void movement()
{   
	//View = lookAt(position,position+direction,up);
}

bool AR_MarkerDetectorInit()
{
	//cv::namedWindow("in",1);
	try
	{
		CamParam.readFromXMLFile("Camera/camera.yml"); 
		return 1;
	}
	catch(exception e)
	{
		return 0;
	}
}



bool AR_MarkerDetector()
{
	GLubyte *imageData;
	aruco::MarkerDetector MDetector;
	float MarkerSize = 0.127f;
	imageData = new GLubyte[widthColor*heightColor*3];
	for(int i=0; i<widthColor*heightColor; i++)
	{
		imageData[3*i]=dataColor[4*i];
		imageData[3*i+1]=dataColor[4*i+1];
		imageData[3*i+2]=dataColor[4*i+2];
	}
	cv::Mat Tex(heightColor, widthColor, CV_8UC3, imageData);

	CamParam.resize(Tex.size());
	try
	{
		MDetector.detect(Tex,Markers,CamParam,MarkerSize);
	}
	catch(exception e)
	{
		return 0;
	}
	double mvp[16];
	cv::Point2f point;
	for (unsigned int i=0;i<Markers.size();i++) 
	{
            cout<<Markers[i]<<endl;
            //Markers[i].draw(Tex,cv::Scalar(0,0,255),2);
			Markers[i].glGetModelViewMatrix(mvp);
			point = Markers[i].getCenter();
	}
	//cv::imshow("in",Tex);
	delete [] imageData;
	return 1;
}


int main(int argc, char *argv[])
{
	GL_init(argc,argv);
	if(!Kinect_init(sensor, rgbStream, depthStream)) return 1;
	if(!AR_MarkerDetectorInit()) return 1;
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		movement();

		if(!useShader(programHandle,compileShader(programHandle,texvshader_name,texfshader_name))) return 1;
		getKinectColorData(dataColor, sensor, rgbStream, widthColor, heightColor);
		textureIdColor = TextureLoader(dataColor, widthColor, heightColor);
		rendererColor(programHandle, Model, View, OrthoProjection, renderingMode, model_dataColor, window);

		if(!useShader(programHandle,compileShader(programHandle,modvshader_name,modfshader_name))) return 1;
		renderer(programHandle, Model, View, Projection, mat3(View*Model), renderingMode, model_data, window);

		//getKinectDepthData(dataDepth, sensor, depthStream, widthDepth, heightDepth);
		//textureIdDepth = TextureLoader(dataDepth, widthDepth, heightDepth);
		//rendererDepth(programHandle, Model, View, Projection, renderingMode, model_dataDepth, window);

		glfwSwapBuffers(window);

		AR_MarkerDetector();

		TextureUnloader(&textureIdColor);
		//TextureUnloader(&textureIdDepth);
	}
	glfwTerminate();
	return 0;
}