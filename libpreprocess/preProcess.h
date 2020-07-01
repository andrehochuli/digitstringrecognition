#ifndef _PRE_PROCESS_H
#define _PRE_PROCESS_H

#include "globals.h"

/** Finds a global bbox around black pixels*/
CvRect findBBOX(IplImage *img,int borderSize);

/** Pre-processing = Gaussian Filter, Threshold(127), Bbox */
/* Parameters with 0  value, disable respective process, */
/* except in borderSize, that sets 0 border. To preserve original border, set negative one(-1) */
/* Ex smoothWinSize=0 and normSize=(0,0), do not smooth and do not normalize */
IplImage *preProcessImage(IplImage *img, int smoothWinSize=3, int slant=1, CvSize normSize=cvSize(64,64), int borderSize=3);
#endif
