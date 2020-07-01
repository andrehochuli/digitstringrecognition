#include "../libpreprocess/preProcess.h"

#include "handWNumeralRecognizer_1110Classes.h"
#include "../preProcess/preProcess_v1.h"
#include "cnnClassifier.h"

handWNumeralRecognizer_1110Classes::handWNumeralRecognizer_1110Classes(string _filename)
{
    this->showErrors=0;
    this->filename = _filename;

    preProcess.push_back(new preProcess_v1()); //recognizerInterface will free it

    cnnParams params;

    params.model_file = "./models/hocNet/alexnet_iter_30000.caffemodel";
    params.mean_file = "./models/hocNet/dummy_mean.binaryproto";
    params.label_file = "./models/size_labels.txt";
    params.deploy_file = "./models/hocNet/alexnet_deploy.prototxt";
    params.normalizationType = MINMAX;

    classifiers.push_back(new cnnClassifier(&params));

    params.model_file = "./models/CNN1110_fullprotocol/1110Digits_model.caffemodel";
    params.mean_file = "./models/CNN1110_fullprotocol/1110Digits_mean.binaryproto";
    params.label_file = "./models/CNN1110_fullprotocol/1110Digits_labels.txt";
    params.deploy_file = "./models/CNN1110_fullprotocol/1110Digits_deploy.prototxt";
    params.normalizationType = MEAN_IMAGE;    
    
    classifiers.push_back(new cnnClassifier(&params));

}

handWNumeralRecognizer_1110Classes::~handWNumeralRecognizer_1110Classes()
{

}

int handWNumeralRecognizer_1110Classes::process(int batchSize,  int knownStrlen, FUSION_TYPE fType, float fThreshold)
{
    int ret=0;
    vector <ImageInfo *> imgsProcessed,components; // Ponteiros para as images
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
    
    //knownStrlen = 1;
	knownPred.classID=knownStrlen-1;
	knownPred.className=((char)knownStrlen+48);
	knownPred.score=-9.90; //Score when Length was known
	
    batchSize =1;
    cout << fType << "of" << DIGIT_MAX_FUSION << endl;
    if (fType == DIGIT_MAX_FUSION)
    {
        fType = CNN_1110_MAX_DIGIT;        
    }
    else if (fType == NO_FUSION)
    {
        fType = CNN_1110_TOP1;
    }
    else if (fType == PROD_FUSION)
    {
        fType = CNN_1110_PROD;
    }    
    else if(fType != CNN_1110_DIGIT_ONLY)
    {
        cout << fType << " " << DIGIT_MAX_FUSION;
        cout << "ERROR: FUSION TYPE NOT AVALIABLE TO 1110 CLASSES" << endl;
        return 0;
    }
    
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
        
        //cout << imgs[0]->filename << endl;
        if((int)imgs.size()<=0)
            break;

        for(int i=0; i<(int)preProcess.size();i++)
            preProcess[i]->process(this->imgs,&imgsProcessed);
		
		
        components.clear();        

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

				
				
					
					
				components.push_back(imgsProcessed[i]->components[j]);
				
			}
            
            printf("ASDLKJASDKLA\n");
            
            stats.Calls1D += components.size();
            
			
            /* Classify isolated components */
            preds.clear();
            if((int)components.size() > 0)
            {
				preds = classifiers[1]->process(components,20);				
				for(int j=0;j<(int)components.size();j++)
					components[j]->DSM[0] = preds[j];
			}						
			
            components.clear();
						

        }// for images
		
        
		this->processResults::fType = fType;
        this->processResults::process(imgsProcessed,&stats,SHOW_ALL,saveErrs, fThreshold);
    
    }//While load images


    float score = (float)stats.nOK/(float)stats.nFiles;

	
    cout << "==> UnClassifieds Components: " << endl;
    cout << "\t ==> 4digits: " << stats.Calls4D << endl;    

    cout << "Classifiers Calls: " << endl;
    cout << "LEM: " << stats.CallsLEM << endl; //Divide pelo top, pois o classificador foi chamado para cada TOP !!
    cout << "Digit Classifier: " << stats.Calls1D << endl;
        
    
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
