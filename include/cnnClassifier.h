#ifndef CNNCLASSIFIER_H
#define CNNCLASSIFIER_H

#include "../interfaces/classifierInterface.h"


typedef struct cnnParams
{
  NORMALIZATION_TYPES normalizationType;
  string deploy_file;
  string model_file;
  string mean_file;
  string label_file;

}cnnParams;

class cnnClassifier: public classifierInterface
{
    public:
        cnnClassifier(cnnParams * _params);
        std::vector< std::vector <Prediction> >process(vector <ImageInfo *> imgs, int Npreds=1);
        virtual ~cnnClassifier();
    protected:
    private:
        cnnParams params;
        Classifier *classifier;
};

#endif // CNNCLASSIFIER_H
