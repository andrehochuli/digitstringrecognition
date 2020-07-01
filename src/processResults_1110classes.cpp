#include "processResults_1110classes.h"
#include <cv.h>

processResults_1110::processResults_1110()
{
    fType = NO_FUSION;    
}

processResults_1110::~processResults_1110()
{
	
    
}

float processResults_1110::fusion(FusionInfo *fInfo,statsInfo *stats)
{	
	int DSM_ID,LEMsize;
	Prediction pBest,pBestLen,pDig,pLen,p1,p2;	
	vector <Prediction> TopLEM;	
	float predScore=1.0;
	
	fInfo->DSM.clear();
	fInfo->LEM.clear();
	
    cout << "AQUI AQUI" << endl;
		
	for(int j=0;j<(int)fInfo->main->components.size();j++)
    {	
		TopLEM = fInfo->main->components[j]->LEM;
		
		LEMsize = (int)TopLEM.size();
		if(LEMsize<=0)
            continue;
		
		pBest.score = -9.999;			
		
		if(fType == SUM_FUSION || fType == PROD_FUSION)	
		{
			for(int t=0;t<LEMsize;t++)
			{
				pLen = TopLEM[t];	
				DSM_ID = pLen.classID;
				if(DSM_ID >= 3) //4+
				{
					cout << "Sorry, we dont have " << TopLEM[t].className << "Digit Length classifier [TOP " << t << "]" << endl;			
					break;				
				}
				
				if(fInfo->main->components[j]->DSM[DSM_ID].size() < 0)
				{
					cout << "No preds" << endl;								
					continue;
				}
					
				pDig = fInfo->main->components[j]->DSM[DSM_ID][0];
				
				if(fType == SUM_FUSION)
					pDig.score += pLen.score;
				else if(fType == PROD_FUSION)
					pDig.score *= pLen.score;			
				
				if(pDig.score > pBest.score)
				{
					pBest = pDig;
					pBestLen = TopLEM[t]; 			
				}								
			}
		}										
		else if(fType == DIGIT_MAX_FUSION && LEMsize > 1 && TopLEM[0].score <= fInfo->fusionThreshold) //So faz a fusão se  o top1 do lem for menor 95%
		{		
			DSM_ID = TopLEM[0].classID; //TOP 1 LEM 
			if(DSM_ID >= 3)
			{
				cout << "Sorry, we dont have " << TopLEM[0].className << "Digit Length classifier [TOP1]" << endl;
				stats->Calls4D-=1;			
				p1.score = 0;
			}
			else			
				p1 = fInfo->main->components[j]->DSM[DSM_ID][0]; //TOP 1 DSM
				
			
			DSM_ID = TopLEM[1].classID; //TOP 2 LEM 
			if(DSM_ID >= 3)
			{
				cout << "Sorry, we dont have " << TopLEM[1].className << "Digit Length classifier [TOP2]" << endl;
				stats->Calls4D-=1;
				p2.score = 0;
			}
			else
				p2 = fInfo->main->components[j]->DSM[DSM_ID][0]; //TOP 1 DSM
					
			if(p1.score >= p2.score)
			{
				pBest = p1;
				pBestLen = TopLEM[0]; 								
			}
			else
			{
				pBest = p2;
				pBestLen = TopLEM[1]; 				 		
			}
			
		}
        else
        {
            
            DSM_ID = TopLEM[0].classID; //TOP 1 LEM             
			if(DSM_ID >= 3)
			{
				cout << "Sorry, we dont have " << TopLEM[0].className << "Digit Length classifier [TOP1]" << endl;
				stats->Calls4D-=1;			
				p1.score = 0;
			}
			else			
            {
                for(int i=0; i<(int)fInfo->main->components[j]->DSM[DSM_ID].size();i++)                
                {   
                    //cout << DSM_ID+1 << " " << i << "of" << (int)fInfo->main->components[j]->DSM[DSM_ID].size() << " PRED " << fInfo->main->components[j]->DSM[DSM_ID][i].className << endl;
                    int len=fInfo->main->components[j]->DSM[DSM_ID][i].className.size();
                    if (len == DSM_ID+1)
                    {
                        p1 = fInfo->main->components[j]->DSM[DSM_ID][i];
                        //cout << "BEST " << DSM_ID+1 << " " << i << " PRED " << fInfo->main->components[j]->DSM[DSM_ID][i].className << endl;
                        break;
                    }
                }
            }
				
            //cout <<  TopLEM[0].classID << "(" << TopLEM[0].score << ") " << p1.className << " " <<  p1.score << endl;   
            
			DSM_ID = TopLEM[1].classID; //TOP 2 LEM             
			if(DSM_ID >= 3)
			{
				cout << "Sorry, we dont have " << TopLEM[1].className << "Digit Length classifier [TOP2]" << endl;
				stats->Calls4D-=1;
				p2.score = 0;
			}
			else
            {
                for(int i=0; i<(int)fInfo->main->components[j]->DSM[0].size();i++)                
                {    
                    cout << DSM_ID+1 << " " << i << "of" << (int)fInfo->main->components[j]->DSM[0].size() << " PRED " << fInfo->main->components[j]->DSM[0][i].className << endl;
                    int len=fInfo->main->components[j]->DSM[0][i].className.size();
                    if (len == DSM_ID+1)
                    {
                        p2 = fInfo->main->components[j]->DSM[0][i];
                        //cout << "BEST " << DSM_ID+1 << " " << i << " PRED " << fInfo->main->components[j]->DSM[DSM_ID][i].className << endl;
                        break;
                    }
                }                
				
            }
			
            //cout <<  TopLEM[1].classID << "(" << TopLEM[1].score << ") " << p2.className << " " <<  p2.score << endl;   
            
            if(TopLEM[0].score <= fInfo->fusionThreshold)
            {		
                if(p1.score >= p2.score)
                {
                    pBest = p1;
                    pBestLen = TopLEM[0]; 								
                }
                else
                {
                    pBest = p2;
                    pBestLen = TopLEM[1]; 						
                }
                
            }
            else
            {
                pBest = p1;
                pBestLen = TopLEM[0]; 								
                
            }
            
            /*DSM_ID = TopLEM[0].classID; //TOP 1 LEM 		           
			if(DSM_ID >= 3)
				cout << "Sorry, we dont have " << TopLEM[0].className << "Digit Length classifier [TOP1]" << endl;				
														
			
            pBest = fInfo->main->components[j]->DSM[DSM_ID][0];
            
            for(int i=0; i<(int)fInfo->main->components[j]->DSM[DSM_ID].size();i++)                
            {    
                //cout << DSM_ID+1 << " " << i << "of" << (int)fInfo->main->components[j]->DSM[DSM_ID].size() << " PRED " << fInfo->main->components[j]->DSM[DSM_ID][i].className << endl;
                int len=fInfo->main->components[j]->DSM[DSM_ID][i].className.size();
                if (len == DSM_ID+1)
                {
                    pBest = fInfo->main->components[j]->DSM[DSM_ID][i];
                    //cout << "BEST " << DSM_ID+1 << " " << i << " PRED " << fInfo->main->components[j]->DSM[DSM_ID][i].className << endl;
                    break;
                }
            }               
			
			pBestLen = TopLEM[0]; */
			
			//Desconta as chamadas do top2, pois nao foram utilizadas
			//Então, nao tiveram calls
			if(fType == DIGIT_MAX_FUSION && TopLEM.size() > 1)
			{
				switch(TopLEM[1].classID)
				{
					case 0: 
						stats->Calls1D-=1;
						break;
					case 1: 
						stats->Calls2D-=1;
						break;
					case 2: 
						stats->Calls3D-=1;
						break;
					case 3: 
						stats->Calls4D-=1;
						break;
					
					default:
						cout << "ERROR SWITCH " << TopLEM[0].classID << endl;
						sleep(5);
						break;
				}
			}			
            
        }
        
        
		/*else //TOP1 - NO FUSION
		{
			DSM_ID = TopLEM[0].classID; //TOP 1 LEM 		
			if(DSM_ID >= 3)
				cout << "Sorry, we dont have " << TopLEM[0].className << "Digit Length classifier [TOP1]" << endl;				
														
			
			pBest = fInfo->main->components[j]->DSM[DSM_ID][0];
			pBestLen = TopLEM[0]; 			
			
			//Desconta as chamadas do top2, pois nao foram utilizadas
			//Então, nao tiveram calls
			if(fType == DIGIT_MAX_FUSION && TopLEM.size() > 1)
			{
				switch(TopLEM[1].classID)
				{
					case 0: 
						stats->Calls1D-=1;
						break;
					case 1: 
						stats->Calls2D-=1;
						break;
					case 2: 
						stats->Calls3D-=1;
						break;
					case 3: 
						stats->Calls4D-=1;
						break;
					
					default:
						cout << "ERROR SWITCH " << TopLEM[0].classID << endl;
						sleep(5);
						break;
				}
			}	
			
		}*/
		
		
		
		
		fInfo->DSM.push_back(pBest);
		fInfo->LEM.push_back(pBestLen);
		
		predScore *= pBest.score;
        fInfo->predictChain.append(pBest.className);
        fInfo->predictChainComp.append(pBest.className);
        fInfo->predictChainComp.append("-");

        if(atoi(pBestLen.className.c_str()) == 1)
            fInfo->predictSizeComp.append("1");
        else if(atoi(pBestLen.className.c_str()) == 2)
            fInfo->predictSizeComp.append("2");
        else if(atoi(pBestLen.className.c_str()) == 3)
            fInfo->predictSizeComp.append("3");

        fInfo->predictSizeComp.append("-");

    }
	
    return predScore;	
}

int processResults_1110::process( ImageInfo *result, statsInfo *stats, SHOW_TYPE flagShow, SAVE_TYPE flagSave, float fThreshold)
{

	int hasError=0;	
	FusionInfo fInfo;	

	stats->nFiles++;
	
	fInfo.main = result;
	
	fInfo.fusionThreshold = fThreshold;
		
	fInfo.predictionScore = fusion(&fInfo,stats);	
    
    fInfo.predictSize = 0;
    for(int i=0;i < (int) fInfo.predictSizeComp.size();i++)
    {
        if (fInfo.predictSizeComp[i] != '-')
        {
            //cout << (int)fInfo.predictSizeComp[i]-48 << endl;                                    
            fInfo.predictSize += (int)fInfo.predictSizeComp[i]-48;
        }
    }
	
    //fInfo.predictSize = (int) fInfo.predictChain.size();

	cout << "[" <<  stats->nFiles << "] "  << result->filename << " " << result->label
			<< " " << "PredChain: " << fInfo.predictChain << " [" << fInfo.predictChainComp
				<< "] PredSize: " << fInfo.predictSize << " [" << fInfo.predictSizeComp
				<< "] Score: " << std::fixed << std::setprecision(3) 
				<< fInfo.predictionScore << " broken:" << result->hasJoin;    

	if((int)result->label.size() != fInfo.predictSize)
	{
		stats->sizeErrors++;
		hasError=1;
		cout << " [ERROR SIZE] ";
		fInfo.result = "ERROR_SIZE";
	}
	else if(fInfo.predictChain.compare(result->label) != 0)
	{
		stats->recErrors++;
		hasError=1;
		cout << " [ERROR REC] ";
		fInfo.result = "ERROR_REC";
	}
	else
	{
		stats->nOK++;
		cout << " [OK] ";
		fInfo.result = "OK";
	}

	cout << endl;

	if(flagShow==SHOW_ALL || (hasError == 1 && flagShow == SHOW_ERRORS_ONLY))
	{
		/* Components */
		for(int top=0; top<2; top++)
			for(int j=0;j<(int)result->components.size();j++)
			{
				if((int)result->components[j]->LEM.size()<=top)
					break;

				Prediction predSize = result->components[j]->LEM[top];

				cout << "  PredLen [Top" << top+1 << "]: " << predSize.className << " " << std::fixed
							<< std::setprecision(3) << predSize.score
					<< " | Digs:";

				/* Print all predictions */
				int DSM_ID = predSize.classID;
				for(int p=0; p<(int)result->components[j]->DSM[DSM_ID].size();p++)
				{
					if(p>2) //TOP3 only
						break;
					Prediction pred = result->components[j]->DSM[DSM_ID][p];
					cout << " | [TOP" << p+1 << "] " << setfill(' ') << setw(3) << pred.className << " "
								<< std::fixed << std::setprecision(3) << pred.score;
				}

				cout << endl;               
			}
	}

	/* Save master img */
	if(flagSave==SAVE_ALL || (hasError == 1 && flagSave == SAVE_ERRORS_ONLY))
	{
		saveResult(&fInfo);
	}

	cout << endl;

    return 0;
}

void processResults_1110::saveResult( FusionInfo *fInfo)
{
	string filename;
	size_t found;	
	int wSpace=50;
	ostringstream infoString;	
	IplImage *saveImg;
	Prediction pred;
	CvSize size;
	CvRect ROI;
	CvFont font;
	CvScalar color;
	int posX,hLine=55,w=0,compSize,maxHeigth;
	
	
	compSize = (int)fInfo->main->components.size();

	if(saveDir.size() <=0)
	{
		cout << endl << "===> SAVE_DIRECTORY NOT SET. Aborting <===" << endl;
		return;
	}
		
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4);
	
	size.width = wSpace;
	maxHeigth =0;
	for(int j=0;j<compSize;j++)	
	{
		size.width += fInfo->main->components[j]->img->width + wSpace;
		if(fInfo->main->components[j]->img->width > maxHeigth)
			maxHeigth = fInfo->main->components[j]->img->width;
	}
	
	if(size.width < fInfo->main->img->width)
		size.width = fInfo->main->img->width + 10;
	
	
	size.height= fInfo->main->img->height+hLine+15 + maxHeigth+70;

	saveImg = cvCreateImage(size,fInfo->main->img->depth,3);
	cvSet(saveImg,cvScalarAll(255));

	found = fInfo->main->filename.find_last_of("/"); 
	filename = fInfo->main->filename.substr(found+1);
	filename=filename.substr(0,filename.size()-4);
	 
	posX = abs(size.width - fInfo->main->img->width)/2;
	ROI = cvRect(posX,35,fInfo->main->img->width,fInfo->main->img->height);	
	
	/*cvShowImage("Img", fInfo->main->img);
	cvWaitKey(0);*/
	
	//Draw Input String
	cvSetImageROI(saveImg,ROI);
	cvCvtColor(fInfo->main->img,saveImg,CV_GRAY2RGB);
	cvResetImageROI(saveImg);
	
	infoString << "Input: "	<< fInfo->main->label;			   
	cvPutText(saveImg, infoString.str().c_str(), cvPoint(5, 15), &font, cvScalar(255));
	
	
	infoString.str("");//clear
	infoString << "Pred: " << fInfo->predictChain
		       << " " << std::fixed << std::setprecision(3)
		       << fInfo->predictionScore;
	
	if(fInfo->main->label.compare(fInfo->predictChain.c_str()) == 0)
		color = CV_RGB(0,0,255);
	else
		color = CV_RGB(255,0,0);
			
	cvPutText(saveImg, infoString.str().c_str(), cvPoint(5, 30), &font, color);
	
	infoString.str("");//clear   
	if(fInfo->main->hasJoin)
		infoString << " brokenPiece: Yes";
	else
		infoString << "brokenPiece: No";	   
	
	
	
	cvPutText(saveImg, infoString.str().c_str(), cvPoint(5, ROI.height+45), &font, cvScalar(255));
			
	cvLine(saveImg,cvPoint(0,ROI.height+hLine),
		cvPoint(saveImg->width,ROI.height+hLine),cvScalarAll(0),1);
	
	w=wSpace;
	int lPos=0; 
	hLine+=15;
	
	for(int j=0;j<compSize;j++)
	{	
		ROI = cvRect(w,fInfo->main->img->height+hLine,
				fInfo->main->components[j]->img->width,
				fInfo->main->components[j]->img->height);
		
		cvSetImageROI(saveImg,ROI);
		
		cvCvtColor(fInfo->main->components[j]->img,saveImg,CV_GRAY2RGB);
	
		cvResetImageROI(saveImg);
		color = CV_RGB(0,0,255);		
		infoString.str("");
		if(fInfo->LEM[j].classID <= 2)
			pred = fInfo->DSM[j];
		else
		{
			pred.score = 0.0;
			pred.className = "none";
		}
		
		infoString << "(D) " << pred.className << " " 
		<< std::fixed << std::setprecision(3) << pred.score;
		
		/* Avalia qual o componente com label errado */
		if(lPos+(int) pred.className.size()-1 < (int)fInfo->main->label.size())
		{
			
			for(int c = 0; c < (int) pred.className.size(); c++,lPos++)
				if(pred.className[c] != fInfo->main->label[lPos])
					color = CV_RGB(255,0,0);
		}
		else
			color = CV_RGB(255,0,0);
			
		cvPutText(saveImg, infoString.str().c_str(), cvPoint(w-15, fInfo->main->img->height+hLine + ROI.height+30), &font, color);
		
		infoString.str("");
		pred = fInfo->LEM[j];
		infoString << "(L) " << pred.className << " " 
		<< std::fixed << std::setprecision(3) << pred.score;
		
		cvPutText(saveImg, infoString.str().c_str(), cvPoint(w-15, fInfo->main->img->height+hLine + ROI.height+15), &font, color);

		cvRectangle(saveImg,cvPoint(ROI.x,ROI.y),cvPoint(ROI.x+ROI.width,ROI.y+ROI.height),color,1);				

		w += fInfo->main->components[j]->img->width + wSpace; 
			
		
	}
	
	infoString.str("");
	infoString << saveDir << "/" 
			<< fInfo->result  << "-#" << fInfo->main->label 
			<< "-Pred#" << fInfo->predictChain << "-#"
			<< std::fixed << std::setprecision(3) << fInfo->predictionScore << "-broken"
			<< fInfo->main->hasJoin << "-" << filename << ".png";
	
	
	 cvSaveImage(infoString.str().c_str(),saveImg);
	 
	 /*cvShowImage("SAVE",saveImg);
	 cvWaitKey(0);*/
	 
	 cvReleaseImage(&saveImg);
}

int processResults_1110::process( vector <ImageInfo *> &imgsProcessed, statsInfo *stats, 
							SHOW_TYPE flagShow, SAVE_TYPE flagSave, float fThreshold)
{
        int size;
        ImageInfo *result;

        size = (int)imgsProcessed.size();

        for(int i=0;i<size;i++)
        {

            result = imgsProcessed[i];
            this->process(result,stats,flagShow,flagSave,fThreshold);
        }

    return 0;
}
