#ifndef AR
#define AR

#include <aruco\aruco.h>

#include <glew/glew.h>
#include <opencv2/core/core.hpp>



bool AR_MarkerDetectorInit();

bool AR_MarkerDetector(vector<aruco::Marker> &Markers, GLubyte dataColor[], int widthColor, int heightColor, float MarkerSize);

bool AR_MarkerDetectorFromImage(vector<aruco::Marker> &Markers, cv::Mat Tex, float MarkerSize);

#endif