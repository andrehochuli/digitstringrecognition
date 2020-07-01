#include "preProcess_v1.h"
#include "common.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "../libpreprocess/preProcess.h"
#include "findComponents.h"

preProcess_v1::preProcess_v1()
{
    //ctor
}

preProcess_v1::~preProcess_v1()
{
    //dtor
}

int preProcess_v1::process(const vector <ImageInfo *> &inputImgs, vector <ImageInfo *> *processed)
{
    int i,size;
    ImageInfo *imInfo;
    int smoothWin = 3, flagSlant = 0, mergeSmall=1; 
    Component tempInfo;
    CvRect bbox;
    std::ostringstream tempName;
    size = inputImgs.size();
    
    
    for(i=0; i< size; i++)
    {
        imInfo = new ImageInfo(); //Dont free it here, destructor of std::vector will done !
        imInfo->filename = inputImgs[i]->filename;
        imInfo->label = inputImgs[i]->label;
        
        /* PRE PROCESS */        
        imInfo->img = preProcessImage(inputImgs[i]->img,smoothWin,flagSlant,NO_RESIZE,BORDER_SIZE);                
        this->find(imInfo,mergeSmall);               
        /* ------------------ */
        
        /* NO PRE_PROCESS (Validation 1110 - CC only*/
        /*imInfo->img = cvCloneImage(inputImgs[i]->img);
        bbox = cvRect(0,0,inputImgs[i]->img->width,inputImgs[i]->img->height);        
        tempInfo.imgInfo = new ImageInfo();
        
        tempInfo.bbox = bbox;
        tempInfo.imgInfo->img = cvCloneImage(inputImgs[i]->img);
        
        tempName.str("");
        tempName << imInfo->filename.substr(0,imInfo->filename.size()-4) ;
        tempName << "-comp1of1.png" ;
        
        tempInfo.imgInfo->filename = tempName.str();
        imInfo->bboxes.push_back(bbox);
        imInfo->components.push_back(tempInfo.imgInfo);*/
        /*----------------------*/ 
         
            
        processed->push_back(imInfo);
    }

    return 0;
}
