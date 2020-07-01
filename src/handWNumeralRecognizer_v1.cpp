#include "../libpreprocess/preProcess.h"

#include "handWNumeralRecognizer_v1.h"
#include "../preProcess/preProcess_v1.h"
#include "cnnClassifier.h"

handWNumeralRecognizer_v1::handWNumeralRecognizer_v1(string _filename)
{
    this->showErrors=0;
    this->filename = _filename;

    preProcess.push_back(new preProcess_v1()); //recognizerInterface will free it

    cnnParams params;

//#define NORM
#ifndef NORM

    params.model_file = "./models/hocNet/alexnet_iter_30000.caffemodel";
    params.mean_file = "./models/hocNet/dummy_mean.binaryproto";
    params.label_file = "./models/size_labels.txt";
    params.deploy_file = "./models/hocNet/alexnet_deploy.prototxt";
    params.normalizationType = MINMAX;

    /*params.model_file = "./models/size_model.caffemodel";
    params.mean_file = "./models/size_mean.binaryproto";
    params.label_file = "./models/size_labels.txt";
    params.deploy_file = "./models/size_deploy.prototxt";
    params.normalizationType = MINMAX;*/

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models/1digit_model.caffemodel";
    params.mean_file = "./models/1digit_mean.binaryproto";
    params.label_file = "./models/1digit_labels.txt";
    params.deploy_file = "./models/1digit_deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;

    /*params.model_file = "./models/single/imagenet_train_iter_24000.caffemodel";
    params.mean_file = "./models/single/dummy_mean.binaryproto";
    params.label_file = "./models/single/labels.txt";
    params.deploy_file = "./models/single/deploy.prototxt";*/

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models/2digit_model.caffemodel";
    params.mean_file = "./models/2digit_mean.binaryproto";
    params.label_file = "./models/2digit_labels.txt";
    params.deploy_file = "./models/2digit_deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models/3digit_model.caffemodel";
    params.mean_file = "./models/3digit_mean.binaryproto";
    params.label_file = "./models/3digit_labels.txt";
    params.deploy_file = "./models/3digit_deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;

    classifiers.push_back(new cnnClassifier(&params));
#else
	
    params.model_file = "./models_norm/size/model.caffemodel";
    params.mean_file = "./models_norm/size/mean.binaryproto";
    params.label_file = "./models_norm/size/labels.txt";
    params.deploy_file = "./models_norm/size/deploy.prototxt";
    params.normalizationType = MINMAX;

    /*params.model_file = "./models/size_model.caffemodel";
    params.mean_file = "./models/size_mean.binaryproto";
    params.label_file = "./models/size_labels.txt";
    params.deploy_file = "./models/size_deploy.prototxt";
    params.normalizationType = MINMAX;*/

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models_norm/isolated/model.caffemodel";
    params.mean_file = "./models_norm/isolated/mean.binaryproto";
    params.label_file = "./models_norm/isolated/labels.txt";
    params.deploy_file = "./models_norm/isolated/deploy.prototxt";

    params.normalizationType = MINMAX;

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models_norm/2digit/model.caffemodel";
    params.mean_file = "./models_norm/2digit/mean.binaryproto";
    params.label_file = "./models_norm/2digit/labels.txt";
    params.deploy_file = "./models_norm/2digit/deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models_norm/3digit/model.caffemodel";
    params.mean_file = "./models_norm/3digit/mean.binaryproto";
    params.label_file = "./models_norm/3digit/labels.txt";
    params.deploy_file = "./models_norm/3digit/deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;

    classifiers.push_back(new cnnClassifier(&params));

#endif
}

handWNumeralRecognizer_v1::~handWNumeralRecognizer_v1()
{

}

int handWNumeralRecognizer_v1::process(int batchSize,  int knownStrlen, FUSION_TYPE fType, float fThreshold)
{
    int ret=0;
    vector <ImageInfo *> imgsProcessed,isolated,pairs,trios,four,others; // Ponteiros para as images
	std::vector< std::vector <Prediction> > preds;
	Prediction knownPred,tempPred;
    statsInfo stats;    
    SAVE_TYPE saveErrs;    
    std::vector <Prediction> top4;
    imgsProcessed.clear();
    
    stats.clear();

    if((int)this->saveDir.size() > 0)
        saveErrs = SAVE_ALL;
    else
        saveErrs = SAVE_NONE;
		
	knownPred.classID=knownStrlen-1;
	knownPred.className=((char)knownStrlen+48);
	knownPred.score=-9.90; //Score when Length was known
	
    while(ret >= 0)
    {
        /* Remove old files */
        for(int i=0;i<(int)imgsProcessed.size();i++)
            delete imgsProcessed[i];

        for(int i=0;i<(int)this->imgs.size();i++)
            delete imgs[i];

        imgsProcessed.clear();
        imgs.clear();

        ret = loadImages(filename,batchSize);

        if((int)imgs.size()<=0)
            break;

        for(int i=0; i<(int)preProcess.size();i++)
            preProcess[i]->process(this->imgs,&imgsProcessed);
		
		
        isolated.clear();
        pairs.clear();
        trios.clear();
        four.clear();
        others.clear();

		/* Classifies each image and his components*/
		for(int i=0;i<(int)imgsProcessed.size();i++)
		{
			/* Se nao tem components*/
			if((int)imgsProcessed[i]->components.size()<=0)
				continue;

			//LEM Prediction
			preds.clear();
			if(knownStrlen == 0)	//Unknown		
			{
				preds = classifiers[0]->process(imgsProcessed[i]->components,5); //5 = TOP5					
				stats.CallsLEM += (int)imgsProcessed[i]->components.size();
			}
			
			for(int j=0;j<(int)imgsProcessed[i]->components.size();j++)
			{	
				/* Se conhecemos o tamanho*/
				if(knownStrlen > 0)
					imgsProcessed[i]->components[j]->LEM.push_back(knownPred);				
				else
					imgsProcessed[i]->components[j]->LEM = preds[j];

				int maxTOP=0;
				if(fType == NO_FUSION)
					maxTOP=1;
				else
					maxTOP=2;

				for(int iTop = 0 ; iTop < maxTOP; iTop++)
				{
					Prediction LEM = imgsProcessed[i]->components[j]->LEM[iTop];
					switch(LEM.classID)
					{
						case 0: //Isolado
							isolated.push_back(imgsProcessed[i]->components[j]);
							break;

						case 1: //Par
							pairs.push_back(imgsProcessed[i]->components[j]);
							break;

						case 2: //Trio
							trios.push_back(imgsProcessed[i]->components[j]);
							break;

						case 3: //Four
							if(iTop == 1)
								top4.push_back(LEM);
								
							four.push_back(imgsProcessed[i]->components[j]);
							break;

						default:
							others.push_back(imgsProcessed[i]->components[j]);
							break;
					}
				}
				
				
			}
            
            stats.Calls1D += isolated.size();
            stats.Calls2D += pairs.size();
            stats.Calls3D += trios.size();
            stats.Calls4D += four.size();
			
            /* Classify isolated components */
            preds.clear();
            if((int)isolated.size() > 0)
            {
				preds = classifiers[1]->process(isolated,5);				
				for(int j=0;j<(int)isolated.size();j++)
					isolated[j]->DSM[0] = preds[j];
			}
			
            /* Classify pairs/double components */
            preds.clear();
            if((int)pairs.size() > 0)
            {
				preds = classifiers[2]->process(pairs,5);							
            	for(int j=0;j<(int)pairs.size();j++)
					pairs[j]->DSM[1] = preds[j];
			}
			
            /* Classify trios components */
            preds.clear();
            if((int)trios.size() > 0)
            {
				preds = classifiers[3]->process(trios,5);
            	for(int j=0;j<(int)trios.size();j++)
					trios[j]->DSM[2] = preds[j];
			}
			
			if((int)four.size() > 0)
            {
				tempPred.classID = 3;
				tempPred.className = "-length4-";
				tempPred.score = 0.0;
            	for(int j=0;j<(int)four.size();j++)
					four[j]->DSM[3].push_back(tempPred);
			}
						
			isolated.clear();
			pairs.clear();
			trios.clear();
			four.clear();			

        }// for images
		
		this->processResults::fType = fType;
        this->processResults::process(imgsProcessed,&stats,SHOW_ALL,saveErrs, fThreshold);
    
    }//While load images


    float score = (float)stats.nOK/(float)stats.nFiles;

	
    cout << "==> UnClassifieds Components: " << endl;
    cout << "\t ==> 4digits: " << stats.Calls4D << endl;
    cout << "\t ==> Others: " << (int) others.size() << endl;

    cout << "Classifiers Calls: " << endl;
    cout << "LEM: " << stats.CallsLEM << endl; //Divide pelo top, pois o classificador foi chamado para cada TOP !!
    cout << "1-DIGIT: " << stats.Calls1D << endl;
    cout << "2-DIGIT: " << stats.Calls2D << endl;
    cout << "3-DIGIT: " << stats.Calls3D << endl;
    cout << "4-DIGIT: " << stats.Calls4D << endl;    
    
    int totalCalls = (stats.CallsLEM + stats.Calls1D + stats.Calls2D + stats.Calls3D);
    cout << "Total Calls: " << totalCalls << endl;
    
    cout << "Processed: " << stats.nFiles << " Images" << endl;
	
	cout << "Fusion Threshold: " << fThreshold << endl;
	
    cout << "Conf: " << std::fixed << std::setprecision(4) << score << " nError: " << stats.sizeErrors+stats.recErrors
            << " [Size: " << stats.sizeErrors << " ] [Rec: " << stats.recErrors << " ]" << endl;
    
    cerr << "Conf: " << std::fixed << std::setprecision(4) << score << " nError: " << stats.sizeErrors+stats.recErrors
            << " [Size: " << stats.sizeErrors << " ] [Rec: " << stats.recErrors << " ]" << endl;
    
    /*cout << "Top2_4 " << (int)top4.size() << endl;
    
    for(int i=0;i<(int)top4.size();i++)
		cout << top4[i].score << endl;*/
	
    return 0;

}
