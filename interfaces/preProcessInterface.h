#ifndef PREPROCESSINTERFACE_H
#define PREPROCESSINTERFACE_H


#include "common.h"
#include "findComponents.h"


using namespace cv;
using namespace std;

class preProcessInterface: public findComponents
{
    public:
        preProcessInterface();
        virtual int process(const vector <ImageInfo *> &inputImgs, vector <ImageInfo *> *processed) = 0;
        virtual ~preProcessInterface();
    protected:

    private:



};

#endif // PREPROCESSINTERFACE_H
