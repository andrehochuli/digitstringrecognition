#ifndef HANDWNUMERALRECOGNIZER_V1_H
#define HANDWNUMERALRECOGNIZER_V1_H

#include "../interfaces/recognizer.h"
#include "Fusion.h"


class handWNumeralRecognizer_v1: public recognizerInterface
{
    public:
        handWNumeralRecognizer_v1(string _filename);
        int process(int batchSize=16, int knowStrlen=0, FUSION_TYPE fType = NO_FUSION, float fThreshold=0.95);
        virtual ~handWNumeralRecognizer_v1();
    protected:
    private:
        string filename;
        Fusion *fusion;
};

#endif // HANDWNUMERALRECOGNIZER_V1_H
