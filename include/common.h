#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <imageInfo.h>

#define FOREGROUND_GRAYSCALE 0

#define BORDER_SIZE 5

#define NO_RESIZE cvSize(0,0)

#define RESIZE_32x32 cvSize(32,32)

#define RESIZE_64x64 cvSize(64,64)

#define RESIZE_128x128 cvSize(128,128)

using namespace std;
using namespace cv;

#endif // COMMON_H_INCLUDED
