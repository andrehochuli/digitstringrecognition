#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "common.h"
#include <fstream>
#include <iostream>

class imageLoader
{
    public:
        imageLoader();
        int loadImages(string filename, int NImgs);
        int releaseImages();
        virtual ~imageLoader();
    protected:
         vector <ImageInfo *> imgs;
    private:
        fstream fp;
};

#endif // IMAGELOADER_H
