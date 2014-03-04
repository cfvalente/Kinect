#include "Kinect.h"
#include <Ole2.h>
#include <NuiApi.h>
#include <iostream>

using namespace std;

bool Kinect_init(INuiSensor* &sensor, HANDLE &rgbStream)
{
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1)
	{
		cout << "Kinect not found.";
		return false;
	}
    if (NuiCreateSensorByIndex(0, &sensor) < 0)
	{
		cout << "Could not initialize Kinect.";
		return false;
	}

    // Initialize sensor
    sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
    sensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_1280x960,    // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &rgbStream);
    return sensor;
}



void getKinectData(GLubyte* dest,INuiSensor* &sensor, HANDLE &rgbStream, int width, int height) {
    NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;
    if (sensor->NuiImageStreamGetNextFrame(rgbStream, 200, &imageFrame) < 0) return;
    INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
    {
        const BYTE* curr = (const BYTE*) LockedRect.pBits;
        const BYTE* dataEnd = curr + (width*height)*4;

        while (curr < dataEnd) {
            *dest++ = *curr++;
        }
    }
	texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);


	SaveBitmapToFile(static_cast<BYTE *>(LockedRect.pBits), width, height, 32, L"C:/Users/Caio/Desktop/img.bmp");
}



bool SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCWSTR lpszFilePath)
{
    DWORD dwByteCount = lWidth * lHeight * (wBitsPerPixel / 8);

    BITMAPINFOHEADER bmpInfoHeader = {0};

    bmpInfoHeader.biSize        = sizeof(BITMAPINFOHEADER);  // Size of the header
    bmpInfoHeader.biBitCount    = wBitsPerPixel;             // Bit count
    bmpInfoHeader.biCompression = BI_RGB;                    // Standard RGB, no compression
    bmpInfoHeader.biWidth       = lWidth;                    // Width in pixels
    bmpInfoHeader.biHeight      = -lHeight;                  // Height in pixels, negative indicates it's stored right-side-up
    bmpInfoHeader.biPlanes      = 1;                         // Default
    bmpInfoHeader.biSizeImage   = dwByteCount;               // Image size in bytes

    BITMAPFILEHEADER bfh = {0};

    bfh.bfType    = 0x4D42;                                           // 'M''B', indicates bitmap
    bfh.bfOffBits = bmpInfoHeader.biSize + sizeof(BITMAPFILEHEADER);  // Offset to the start of pixel data
    bfh.bfSize    = bfh.bfOffBits + bmpInfoHeader.biSizeImage;        // Size of image + headers

    // Create the file on disk to write to
    HANDLE hFile = CreateFileW(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Return if error opening file
    if (NULL == hFile) 
    {
        return E_ACCESSDENIED;
    }

    DWORD dwBytesWritten = 0;
    
    // Write the bitmap file header
    if ( !WriteFile(hFile, &bfh, sizeof(bfh), &dwBytesWritten, NULL) )
    {
        CloseHandle(hFile);
        return E_FAIL;
    }
    
    // Write the bitmap info header
    if ( !WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwBytesWritten, NULL) )
    {
        CloseHandle(hFile);
        return E_FAIL;
    }
    
    // Write the RGB Data
    if ( !WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwBytesWritten, NULL) )
    {
        CloseHandle(hFile);
        return E_FAIL;
    }    

    // Close the file
    CloseHandle(hFile);
    return S_OK;
}