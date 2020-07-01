#include "preProcess.h"
#include "slant.h"

/** Finds a global bbox around black pixels, leaving some border*/
CvRect findBBOX(IplImage *img)
{
	int i,j;
	int top=img->height,bottom=0;
	int left=img->width,right=0;
	unsigned char grayLevel;

	for(i=0;i<img->width;i++)
		for(j=0;j<img->height;j++)
		{
			grayLevel = CV_IMAGE_ELEM(img, unsigned char, j, i);
			//grayLevel = img->imageData[img->widthStep * j + i];
			if(grayLevel == WHITE)
				continue; //back to start (next iteration)

			if(i < left)
				left = i;

			if(i > right)
				right = i;

			if(j < top)
				top = j;

			if(j > bottom)
				bottom = j;

		}

		return cvRect(left,top,abs(right-left),abs(bottom-top));
}

/** Pre-processing = Gaussian Filter, Threshold, slant , CropBbox and Normalize*/
IplImage *preProcessImage(IplImage *img, int smoothWinSize, int slant, CvSize normSize, int borderSize)
{
	CvRect bbox;
	CvSize size;
	IplImage *clone, *slantImg,*norm,*imgBorder;
    float angle;
    int yorigin=-1;

	clone = cvCloneImage(img);

	/* Eliminate some noise */
	if(smoothWinSize != 0)
        cvSmooth(clone,clone,CV_MEDIAN,smoothWinSize);

	cvThreshold(clone,clone,127,WHITE,CV_THRESH_BINARY);

	if(slant == 1)
	{
        angle = cv_slant_estbycc(clone,&yorigin,1);
        slantImg = cv_slant_correction(clone,angle,yorigin,0);
		bbox = findBBOX(slantImg); //Find bbox , because slantImg have some extra borders

		cvReleaseImage(&clone);
		clone = cvCreateImage(cvSize(bbox.width,bbox.height),slantImg->depth, slantImg->nChannels);
		cvSet(clone,cvScalarAll(WHITE));

		cvSetImageROI(slantImg,bbox);
		cvCopy(slantImg, clone);

		cvResetImageROI(slantImg);
		cvReleaseImage(&slantImg);
	}

    /* Size Normalization */
    if(borderSize < 0 && normSize.width > 0 && normSize.height > 0 )
    {
        norm = cvCreateImage(normSize,clone->depth, clone->nChannels);
        cvResize(clone,norm);

        cvReleaseImage(&clone);
        clone = cvCloneImage(norm);
        cvReleaseImage(&norm);
    }

    if(borderSize >= 0)
    {
        bbox = findBBOX(clone); //Find bbox , because sla

        /* Se tem que normalizar */
        if(normSize.width > 0 && normSize.height > 0 )
        {
            size.width = normSize.width;
            size.height = normSize.height;
        }
        else // Apenas adiciona bordas
        {
            size.width = bbox.width + borderSize*2;
            size.height = bbox.height + borderSize*2;
        }
        
        
        
        
            imgBorder = cvCreateImage(size,clone->depth, clone->nChannels);
            cvSet(imgBorder,cvScalarAll(WHITE));

            cvSetImageROI(clone,bbox);

            cvSetImageROI(imgBorder,cvRect(borderSize,borderSize,imgBorder->width-borderSize*2,imgBorder->height-borderSize*2));

            cvResize(clone,imgBorder);

            cvResetImageROI(imgBorder);

            cvResetImageROI(clone);

            cvReleaseImage(&clone);
            clone = cvCloneImage(imgBorder);
            cvReleaseImage(&imgBorder);
        
	}


	return clone;
}
