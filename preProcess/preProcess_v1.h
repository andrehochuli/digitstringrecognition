#ifndef PREPROCESS_V1_H
#define PREPROCESS_V1_H

#include "../interfaces/preProcessInterface.h"
#include "common.h"



class preProcess_v1: public preProcessInterface
{
    public:
        preProcess_v1();
        int process(const vector <ImageInfo *> &inputImgs, vector <ImageInfo *> *processed);
        virtual ~preProcess_v1();
    protected:

    private:

};

#endif // PREPROCESS_V1_H
