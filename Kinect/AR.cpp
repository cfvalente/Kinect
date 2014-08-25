#include <opencv2/core/core.hpp>
#include <aruco\aruco.h>
#include <opencv2/highgui/highgui.hpp>

#include <glew/glew.h>

#include <omp.h>


static aruco::CameraParameters CamParam;


bool AR_MarkerDetectorInit()
{
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



bool AR_MarkerDetector(vector<aruco::Marker> &Markers, GLubyte dataColor[], int widthColor, int heightColor, float MarkerSize)
{
	//GLubyte *imageData;
	aruco::MarkerDetector MDetector;
	//imageData = new GLubyte[widthColor*heightColor*3];

	/*#pragma omp parallel for schedule(static)
		for(int i=0; i<widthColor*heightColor; i++)
		{
			imageData[3*i]=dataColor[4*i];
			imageData[3*i+1]=dataColor[4*i+1];
			imageData[3*i+2]=dataColor[4*i+2];
		}*/
	cv::Mat Tex(heightColor, widthColor, CV_8UC3, dataColor);

	CamParam.resize(Tex.size());
	try
	{
		MDetector.detect(Tex,Markers,CamParam,MarkerSize);
	}
	catch(exception e)
	{
		return 0;
	}
	//delete [] imageData;
	return 1;
}

bool AR_MarkerDetectorFromImage(vector<aruco::Marker> &Markers, cv::Mat Tex, float MarkerSize)
{
	aruco::MarkerDetector MDetector;
	CamParam.resize(Tex.size());
	try
	{
		MDetector.detect(Tex,Markers,CamParam,MarkerSize);
	}
	catch(exception e)
	{
		return 0;
	}
	return 1;
}