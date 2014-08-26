#include <iostream>
#include <string>

#include <omp.h>

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
#include <opencv2/highgui/highgui.hpp>

#include <aruco\aruco.h>
#include "AR.h"

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

#define MarkerSize 0.127f

using namespace std;
using namespace glm;
using namespace cv;

GLuint texprogramHandle;
GLuint modprogramHandle;

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
GLuint textureIdColor;							// ID of the texture to contain Kinect RGB Data
GLubyte dataColor[widthColor*heightColor*3];	// BGRA array containing the texture data

GLuint textureIdDepth;							// ID of the texture to contain Kinect Depth Data
GLubyte dataDepth[widthDepth*heightDepth*4];	// BGRA array containing the texture data

// Kinect variables
HANDLE rgbStream;								// The identifier of the Kinect's RGB Camera
HANDLE depthStream;		   						// The identifier of the Kinect's Depth Camera
INuiSensor* sensor;								// The kinect sensor

// ArUco variables
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

void monta_matriz_de_projecao()
{
	float znear = 0.01; // Near clipping distance
	float zfar = 1000; // Far clipping distance
	float f_x = 1.0591254265877012e+003; // Focal length in x axis
	float f_y = 1.0594512153069588e+003; // Focal length in y axis (usually
	float c_x = 6.4051354189348990e+002; // Camera primary point x
	float c_y = 4.8137915848944107e+002; // Camera primary point y
	/*Projection.value[0] = - 2.0 * f_x;
	Projection.operator[1] = 0.0;
	Projection.operator[2] = 0.0;
	Projection.operator[3] = 0.0;
	Projection.operator[4] = 0.0;
	Projection.operator[5] = 2.0 * f_y;
	Projection.operator[6] = 0.0;
	Projection.operator[7] = 0.0;
	Projection.operator[8] = 2.0 * c_x / widthColor - 1.0;
	Projection.operator[9] = 2.0 * c_y / heightColor - 1.0;
	Projection.operator[10] = -( zfar+znear ) / ( zfar - znear );
	Projection.operator[11] = -1.0;
	Projection.operator[12] = 0.0;
	Projection.operator[13] = 0.0;
	Projection.operator[14] = -2.0 * zfar * znear / ( zfar - znear );
	Projection.operator[15] = 0.0;*/
	Projection = glm::mat4(- 2.0 * f_x,0.0,0.0,0.0,0.0,2.0 * f_y,0.0,0.0,2.0 * c_x / widthColor - 1.0,2.0 * c_y / heightColor - 1.0,-( zfar+znear ) / ( zfar - znear ),-1.0,0.0,0.0,-2.0 * zfar * znear / ( zfar - znear ),0.0);
	Projection = glm::transpose(Projection);

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

	//Projection = glm::perspective(100.0f, widthColorf / heightColorf, 0.1f, 4000.0f);
	monta_matriz_de_projecao();
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





int main(int argc, char *argv[])
{
	GL_init(argc,argv);
	//if(!Kinect_init(sensor, rgbStream, depthStream)) return 1;
	if(!AR_MarkerDetectorInit()) return 1;
	struct model model_data = load_model("Model/teapot.obj");
	if(!compileShader(texprogramHandle,texvshader_name,texfshader_name)) return 1;
	if(!compileShader(modprogramHandle,modvshader_name,modfshader_name)) return 1;

	Mat image;
	image = imread("Markers/_DSC0068.jpg", CV_LOAD_IMAGE_COLOR);
	AR_MarkerDetectorFromImage(Markers, image, MarkerSize);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		movement();


		glUseProgram(texprogramHandle);
		//getKinectColorData(dataColor, sensor, rgbStream, widthColor, heightColor);
		//textureIdColor = TextureLoader(dataColor, heightColor, widthColor);
		textureIdColor = CVImageToGLTexture(image);
		rendererColor(texprogramHandle, Model, View, OrthoProjection, renderingMode, model_dataColor, window);

		glUseProgram(modprogramHandle);
		//AR_MarkerDetector(Markers, dataColor, widthColor, heightColor, MarkerSize);
		for (unsigned int i=0;i<Markers.size();i++) 
		{
			double aux[16];
			//cout<<Markers[i]<<endl;
			Markers[i].glGetModelViewMatrix(aux);
			mat4 mv((float) aux[0],(float) aux[1],(float) aux[2],(float) aux[3],
				(float) aux[4],(float) aux[5],(float) aux[6],(float) aux[7],
				(float) aux[8],(float) aux[9],(float) aux[10],(float) aux[11],
				(float) aux[12],(float) aux[13],(float) aux[14],(float) aux[15]);
			for(int p = 0; p < 4; p++) printf("MVP-R%d -- %f %f %f %f\n",p,aux[4*p],aux[4*p+1],aux[4*p+2],aux[4*p+3]); 
			renderer(modprogramHandle, Projection*mv*scale(glm::rotate(Model,-30.0f,glm::vec3(1.0,0.0,0.0)),vec3(0.005,0.005,0.005)), model_data, window);
		}

		//getKinectDepthData(dataDepth, sensor, depthStream, widthDepth, heightDepth);
		//textureIdDepth = TextureLoader(dataDepth, widthDepth, heightDepth);
		//rendererDepth(programHandle, Model, View, Projection, renderingMode, model_dataDepth, window);

		glfwSwapBuffers(window);
		TextureUnloader(&textureIdColor);
		//TextureUnloader(&textureIdDepth);
	}
	glfwTerminate();
	return 0;
}