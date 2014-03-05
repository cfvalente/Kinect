#include "Kinect.h"
#include <Ole2.h>
#include <NuiApi.h>
#include <iostream>

using namespace std;



bool Kinect_init(INuiSensor* &sensor, HANDLE &rgbStream, HANDLE &depthStream)
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
    if(FAILED(sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR)))
	{
		printf("No ready Kinect found.\n");
		return false;
	}
    if(FAILED(sensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_1280x960,    // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &rgbStream)))
	{
		printf("Color data acquisition failed.\n");
		return false;
	}

	if(FAILED(sensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_DEPTH,                     // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,             // Image resolution
        //0,
		NUI_IMAGE_STREAM_FLAG_DISTINCT_OVERFLOW_DEPTH_VALUES,   // Values outside normal Kinect depth range - [0.8m, 4.0m] 
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &depthStream)))
	{
		printf("Depth data acquisition failed.\n");
		return false;
	}

	if(sensor == NULL)
	{
		printf("Kinect error.\n");
		return false;
	}



    return true;
}

void KinectUp()
{
	LONG plAngleDegrees;
	NuiCameraElevationGetAngle(&plAngleDegrees);
	if(plAngleDegrees+2 < NUI_CAMERA_ELEVATION_MAXIMUM)
	{
		NuiCameraElevationSetAngle(plAngleDegrees+2);
	}
}

void KinectDown()
{
	LONG plAngleDegrees;
	NuiCameraElevationGetAngle(&plAngleDegrees);
	if(plAngleDegrees-2 > NUI_CAMERA_ELEVATION_MINIMUM)
	{
		NuiCameraElevationSetAngle(plAngleDegrees-2);
	}
}




void getKinectColorData(GLubyte* destColor, INuiSensor* &sensor, HANDLE &rgbStream, int width, int height) {
    NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;
	INuiFrameTexture* texture;


	if (FAILED(sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame))) return;
	texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
    {
        const BYTE* pBufferRun = (const BYTE*) LockedRect.pBits;
        const BYTE* pBufferEnd = pBufferRun + (width*height)*4;

        while (pBufferRun < pBufferEnd) 
		{
            *destColor++ = *pBufferRun++;
		}

	}
	texture->UnlockRect(0);

    sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);


	//SaveBitmapToFile(static_cast<BYTE *>(LockedRect.pBits), width, height, 32, L"C:/Users/Caio/Desktop/img.bmp");
}


void getKinectDepthData(GLubyte* destDepth, INuiSensor* &sensor, HANDLE &depthStream, int width, int height) {
	HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;

    // Attempt to get the depth frame
    hr = sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame);
    if (FAILED(hr))
    {
        return;
    }

    BOOL nearMode;
    INuiFrameTexture* pTexture;

    // Get the depth image pixel texture
    hr = sensor->NuiImageFrameGetDepthImagePixelFrameTexture(
        depthStream, &imageFrame, &nearMode, &pTexture);
    if (FAILED(hr))
    {
        goto ReleaseFrame;
    }

    NUI_LOCKED_RECT LockedRect;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        // Get the min and max reliable depth for the current frame
        int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
        int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

        BYTE * rgbrun = destDepth;
        const NUI_DEPTH_IMAGE_PIXEL * pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(LockedRect.pBits);

        // end pixel is start + width*height - 1
        const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (width * height);

        while ( pBufferRun < pBufferEnd )
        {
            // discard the portion of the depth that contains only the player index
            USHORT depth = pBufferRun->depth;

            // To convert to a byte, we're discarding the most-significant
            // rather than least-significant bits.
            // We're preserving detail, although the intensity will "wrap."
            // Values outside the reliable depth range are mapped to 0 (black).

            // Note: Using conditionals in this loop could degrade performance.
            // Consider using a lookup table instead when writing production code.
            BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);

            // Write out blue byte
            *(rgbrun++) = intensity;

            // Write out green byte
            *(rgbrun++) = intensity;

            // Write out red byte
            *(rgbrun++) = intensity;

            // We're outputting BGR, the last byte in the 32 bits is unused so skip it
            // If we were outputting BGRA, we would write alpha here.
			*(rgbrun++) = 1.0;
            //++rgbrun;

            // Increment our index into the Kinect's depth buffer
            ++pBufferRun;
        }

    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    pTexture->Release();

ReleaseFrame:
    // Release the frame
    sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}


/*
void getKinectDepthData(GLubyte* destDepth, INuiSensor* &sensor, HANDLE &depthStream, int width, int height) {
    NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;
	INuiFrameTexture* texture;
	BOOL nearMode = FALSE;
	HRESULT hr;

	if (sensor->NuiImageStreamGetNextFrame(depthStream, 200, &imageFrame) < 0) 
	{
			return;
	}
	hr = sensor->NuiImageFrameGetDepthImagePixelFrameTexture(
        depthStream, &imageFrame, &nearMode, &texture);
	if(FAILED(hr))
	{
		texture->UnlockRect(0);
		sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
		return;
	}
	texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0)
    {
        //const BYTE* pBufferRun = (const BYTE*) LockedRect.pBits;
        //const BYTE* pBufferEnd = pBufferRun + (width*height);
		const NUI_DEPTH_IMAGE_PIXEL * pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(LockedRect.pBits);
		const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (width*height);
        int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
        int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

		int i = 0;

        while (pBufferRun < pBufferEnd) 
		{*/
            //USHORT depth = NuiDepthPixelToDepth(*pBufferRun++);
			/*if(pBufferRun->depth == NULL) 
			{
				texture->UnlockRect(0);
				sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
				return;
			}*//*
			i++;
			USHORT depth = pBufferRun->depth;



			//BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);
			BYTE intensity = (BYTE) depth;
            // Draw a grayscale image of the depth:
            // B,G,R are all set to depth%256, alpha set to 1.
            *destDepth++ = intensity;
			*destDepth++ = intensity;
			*destDepth++ = intensity;
            *destDepth++ = 0xff;

			++pBufferRun;
		}

	}
	texture->UnlockRect(0);

    sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);


	//SaveBitmapToFile(static_cast<BYTE *>(LockedRect.pBits), width, height, 32, L"C:/Users/Caio/Desktop/img.bmp");
}*/

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