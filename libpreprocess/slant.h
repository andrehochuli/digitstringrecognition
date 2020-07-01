#ifndef _slant_h_
#define _slant_h_

#include "globals.h"

// Number of chain codes
#define NCOD 8

// This value must be bigger than the number of components in the string
#define MAXCC 99

//connected components with lenght less than MINLENGHT
// are not considered during the slant estimation
#define MINLENGHT 10

float cv_slant_estbycc(IplImage *im1, int *yorigin, int flagInvertColor=1);
IplImage * cv_slant_correction(IplImage *im1, float angle, int yorigin, int contourVal=0);



#endif


