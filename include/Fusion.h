#ifndef FUSION_H
#define FUSION_H

#include "imageInfo.h"

class hypothesis
{
    public:
    Prediction pred;
    vector <hypothesis> hyp;

    hypothesis()
    {
        hyp.clear();
        pred.score = 1;
        pred.className.clear();
        pred.classID = -1;
    }

};

class Fusion
{
    public:
        int maxTop;
        Fusion();
        void generateHyp(ImageInfo *img, int pos, hypothesis *node);
        void viterbi(ImageInfo *img, int begin=0);
        Prediction process(ImageInfo *img);
        virtual ~Fusion();
    protected:
    private:
        Prediction winner;
        hypothesis hyp_tree;
};

#endif // FUSION_H
