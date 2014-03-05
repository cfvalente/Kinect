#ifndef KINECT
#define KINECT

#include "Include/glew/glew.h"
#include "Include/GLFW/glfw3.h"
#include "windows.h"
#include <Ole2.h>
#include <NuiApi.h>
#include <iostream>


bool Kinect_init(INuiSensor* &sensor, HANDLE &rgbStream, HANDLE &depthStream);

void getKinectColorData(GLubyte* destColor, INuiSensor* &sensor, HANDLE &rgbStream, int width, int height);

void getKinectDepthData(GLubyte* destDepth,INuiSensor* &sensor, HANDLE &depthStream, int width, int height);

/// <summary>
/// Save passed in image data to disk as a bitmap
/// </summary>
/// <param name="pBitmapBits">image data to save</param>
/// <param name="lWidth">width (in pixels) of input image data</param>
/// <param name="lHeight">height (in pixels) of input image data</param>
/// <param name="wBitsPerPixel">bits per pixel of image data</param>
/// <param name="lpszFilePath">full file path to output bitmap to</param>
/// <returns>indicates success or failure</returns>
bool SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCWSTR lpszFilePath);


#endif