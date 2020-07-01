#ifndef handWNumeralRecognizer_1110Classes_H
#define handWNumeralRecognizer_1110Classes_H

#include "../interfaces/recognizer.h"
#include "Fusion.h"


class handWNumeralRecognizer_1110Classes: public recognizerInterface
{
    public:
        handWNumeralRecognizer_1110Classes(string _filename);
        int process(int batchSize=16, int knowStrlen=0, FUSION_TYPE fType = NO_FUSION, float fThreshold=0.95);
        virtual ~handWNumeralRecognizer_1110Classes();
    protected:
    private:
        string filename;
        Fusion *fusion;
};

#endif // handWNumeralRecognizer_1110Classes_H
