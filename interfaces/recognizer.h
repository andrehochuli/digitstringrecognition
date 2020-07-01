#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <iostream>
#include <vector>
#include "preProcessInterface.h"
#include "classifierInterface.h"
#include "verificationInterface.h"
#include "imageLoader.h"
#include "processResults.h"

using namespace cv;
using namespace std;



class recognizerInterface:public imageLoader, public processResults
{
    public:
        recognizerInterface();
        virtual int process(int batchSize=16, int knowStrlen=0, FUSION_TYPE ftype = NO_FUSION, float fThreshold = 0.95) = 0;
        virtual ~recognizerInterface();
        int showErrors;


    protected:
        vector <preProcessInterface *> preProcess;
        vector <classifierInterface *> classifiers;
        vector <verificationInterface *> verification;
    private:
};

#endif // RECOGNIZER_H
