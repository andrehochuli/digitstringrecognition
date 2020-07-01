#ifndef CLASSIFIERINTERFACE_H
#define CLASSIFIERINTERFACE_H

#include "common.h"

class classifierInterface
{
    public:
        classifierInterface();
        virtual std::vector< std::vector <Prediction> > process(vector <ImageInfo *> imgs, int NPreds=1) = 0;
        virtual ~classifierInterface();
    protected:
    private:
};

#endif // CLASSIFIERINTERFACE_H
