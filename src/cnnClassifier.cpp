#include "cnnClassifier.h"

cnnClassifier::cnnClassifier(cnnParams * _params)
{
    classifier = NULL;

    classifier = new Classifier(_params->deploy_file, _params->model_file, _params->mean_file,
                                                    _params->label_file, _params->normalizationType);
}

cnnClassifier::~cnnClassifier()
{
    if(classifier)
        delete classifier;
}


 std::vector< std::vector <Prediction> >cnnClassifier::process(vector <ImageInfo *> imgs, int NPreds)
{
		
		std::vector< std::vector <Prediction> >predictions;
		predictions.clear();
		 
        if((int)imgs.size() <= 0)
            return predictions;

        int classes = classifier->getNumberOfClasses();
        if(NPreds > classes) //Avoid top list greater than problem classes
        {
            NPreds = classes;
            /*cout << "[Warning] CnnClassifier says: Number of top list predictions is greater than number of problem classes. Ajusting for "
                << NPreds << endl;*/
        }

        classifier->batchImgs.clear();
        for(int i=0;i<(int)imgs.size();i++)
            classifier->batchImgs.push_back(cv::cvarrToMat(imgs[i]->img));

        /* Lista de listas de predicts. Cada posicao nao primeira, referencia uma imagem */
       predictions = classifier->ClassifyBatch(NPreds);

       for(int i=0;i<(int)imgs.size();i++)
            classifier->batchImgs[i].release();

    return predictions;
}

