#ifndef AR
#define AR

#include <aruco\aruco.h>

#include <glew/glew.h>


bool AR_MarkerDetectorInit();

bool AR_MarkerDetector(vector<aruco::Marker> &Markers, GLubyte dataColor[], int widthColor, int heightColor, float MarkerSize);

#endif