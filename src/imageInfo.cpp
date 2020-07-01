#include "imageInfo.h"

ImageInfo::ImageInfo()
{
    img = NULL;    
    components.clear();
    hasJoin = 0;    
}


ImageInfo::~ImageInfo()
{

    if(img)
      cvReleaseImage(&img);

    img = NULL;
    

    for(int i=0;i<(int)components.size();i++)
    {
        if(components[i])
            delete components[i];

        components[i] = NULL;
    }
	
    nSegLines=0;
    nSegs=0;


}


ImageInfo& ImageInfo::operator=(const ImageInfo& src)
{

    this->components = src.components;
    this->filename = src.filename;
    this->hasJoin = src.hasJoin;
    this->img = cvCloneImage(src.img);
    this->label = src.label;    
    this->LEM = src.LEM;
    this->DSM[0] = src.DSM[0];
    this->DSM[1] = src.DSM[1];
    this->DSM[2] = src.DSM[2];
    this->nSegLines = src.nSegLines;
    this->nSegs = src.nSegs;
    
    return *this;
}



string ImageInfo::getPredictChain()
{
    string chain;

    chain.clear();
    for(int i=0;i<(int)this->components.size();i++)
    {
		if(components[i]->LEM.size() <=0)
            break;
            
		int DSM_ID = components[i]->LEM[0].classID;
		
		if(components[i]->DSM[DSM_ID].size() <=0)
            break;
		
        chain.append(components[i]->DSM[DSM_ID][0].className);
    }

    if((int)chain.size() <=0)
        chain = "N/A";

    return chain;


}

string ImageInfo::getPredictChainByComp()
{
    string chain;

    chain.clear();
    for(int i=0;i<(int)this->components.size();i++)
    {
        if(components[i]->LEM.size() <=0)
            break;
		
		int DSM_ID = components[i]->LEM[0].classID;
		
		if((int)components[i]->DSM[DSM_ID].size()<=0)
            break;
		
        chain.append(components[i]->DSM[DSM_ID][0].className);
        chain.append("-");
    }

    if((int)chain.size() > 0)
        chain.erase( chain.end()-1 );
    else
        chain = "non-predictChain";


    return chain;
}

string ImageInfo::getPredictSizeByComp()
{
    string chain;

    chain.clear();
    for(int i=0;i<(int)this->components.size();i++)
    {
        if((int)components[i]->LEM.size()<=0)
            break;
		
		
        chain.append(components[i]->LEM[0].className);
        chain.append("-");
    }

    if((int)chain.size() > 0)
        chain.erase( chain.end()-1 );
    else
        chain = "non-predictSize";

    return chain;
}

int ImageInfo::getPredictSize()
{
    int size;

    size = 0;

    for(int i=0;i<(int)this->components.size();i++)
    {
        if((int)components[i]->LEM.size()<=0)
            break;

        size += components[i]->LEM[0].classID+1;
    }

    return size;
}

float ImageInfo::getPredictScore()
{
    float score;

    score = 1.0;

    for(int i=0;i<(int)this->components.size();i++)
    {
		if((int)components[i]->LEM.size()<=0)
            break;
                
		int DSM_ID = components[i]->LEM[0].classID;
		
		if((int)components[i]->DSM[DSM_ID].size()<=0)
            break;
            
        score *= components[i]->DSM[DSM_ID][0].score;
    }

    return score;
}
