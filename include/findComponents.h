#ifndef FINDCOMPONENTS_H
#define FINDCOMPONENTS_H

#include "common.h"

class Component
{
    public:
    ImageInfo *imgInfo;
    CvRect bbox;
    unsigned char floodColor;
};

class findComponents
{
    public:
        findComponents();
        virtual ~findComponents();
        IplImage * extractComponentBlob(IplImage *input, CvRect ROI, unsigned char countourColor, int borderSize);
    protected:
        int find(ImageInfo *input,int flagMerge=0);
    private:
        int mergeComponents(vector <Component> *tempComponents, ImageInfo *input,int pos);
        int brokenDigitLESOliveira(vector <Component> *tempComponents, ImageInfo *input);
        IplImage * groupImages(ImageInfo *input, Component *imgLeft, CvPoint ptLeft, Component *imgRight, CvPoint ptRight);
        vector <CvPoint> findcountourPoints(IplImage* img, CvPoint offset);
        vector <CvPoint> findClosestPoints(vector <CvPoint> points1, vector <CvPoint> points2 );

};

#endif // FINDCOMPONENTS_H
