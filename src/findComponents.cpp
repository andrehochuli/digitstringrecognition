#include "findComponents.h"

#include "../libpreprocess/preProcess.h"

findComponents::findComponents()
{
    //ctor
}

findComponents::~findComponents()
{
    //dtor
}

/* Uso isto, para ordernar os componentes, baseado pela extremidade do bbox.
Dessa forma, os digitos ficaram na ordem da string,
Evitando que um componente sobre-posto com outro, tenha sido detectado em outra posicao da string */
struct sort_components
{
    inline bool operator() (const Component& im1, const Component& im2)
    {
		//Centro de massa no eixo x
        return im1.bbox.x+(im1.bbox.width/2) < im2.bbox.x+(im2.bbox.width)/2;
    }
};

/* Extrai o componente que contem uma determinada cor */
IplImage * findComponents::extractComponentBlob(IplImage *input, CvRect ROI, unsigned char countourColor,int borderSize)
{

    IplImage *im;
    unsigned char grayLevel;

    im = cvCreateImage(cvSize(ROI.width+borderSize*2,ROI.height+borderSize*2),IPL_DEPTH_8U,1);

    cvSet(im,cvScalarAll(255));


    for(int j=0; j<ROI.height;j++)
        for(int i=0; i<ROI.width;i++)
        {
            grayLevel = CV_IMAGE_ELEM(input, unsigned char, j+ROI.y, i+ROI.x);

            if(grayLevel == countourColor)
                 CV_IMAGE_ELEM(im, unsigned char, j+borderSize, i+borderSize) = 0;

        }

    return im;
}

vector <CvPoint> findComponents::findClosestPoints(vector <CvPoint> pointsA, vector <CvPoint> pointsB )
{
    vector <CvPoint> closestPoints;
    CvPoint A,B;
    float best=9999.99,dist;

    for(int i=0;i<(int)pointsA.size();i++)
    {
        A = pointsA[i];
        for(int j=0;j<(int)pointsB.size();j++)
        {

            B = pointsB[j];
            dist = sqrt(pow(A.x-B.x,2) + pow(A.y-B.y,2));

            if(dist < best)
            {
                best = dist;
                closestPoints.clear();
                closestPoints.push_back(A);
                closestPoints.push_back(B);
            }
        }
    }

    return closestPoints;
}

vector <CvPoint> findComponents::findcountourPoints(IplImage *input, CvPoint offset)
{
    vector <CvPoint> points;

    IplImage *cannyOut;
    unsigned char grayLevel;

    cannyOut = cvCloneImage(input);
    cvZero(cannyOut);

    cvCanny( input, cannyOut, 127, 255 , 3);

    for(int i=0;i<cannyOut->width;i++)
		for(int j=0;j<cannyOut->height;j++)
		{
			grayLevel = CV_IMAGE_ELEM(cannyOut, unsigned char, j, i);
			//grayLevel = img->imageData[img->widthStep * j + i];
			if(grayLevel == 255)
                points.push_back(cvPoint(i+offset.x,j+offset.y));

        }

    cvReleaseImage(&cannyOut);


    return points;
}

IplImage * findComponents::groupImages(ImageInfo *input, Component *first, CvPoint firstPoint, Component *second, CvPoint secondPoint)
{
    IplImage *result;
    CvPoint offset=cvPoint(0,0);
    CvSize imgSize = cvGetSize(input->img);

    /* Crio um pouco maior, pois ao deslizar os componentes, a imagem pode mudar de tamanho */
    imgSize.width *= 1.25;
    imgSize.height *= 1.25;

    result = cvCreateImage(imgSize,input->img->depth,input->img->nChannels);
    if(!result)
    {
        cout << "Didnt create result img" << endl;
        return NULL;
    }

    cvSet(result,cvScalarAll(255));

    offset.x = first->bbox.x - BORDER_SIZE;
    offset.y = first->bbox.y - BORDER_SIZE;
    for(int x=BORDER_SIZE; x<first->imgInfo->img->width; x++)
        for(int y=BORDER_SIZE; y<first->imgInfo->img->height; y++)
        {
            if(CV_IMAGE_ELEM(first->imgInfo->img, unsigned char, y, x) == 0)
                ((uchar *)(result->imageData + ((y+offset.y)*result->widthStep)))[x+offset.x] = 0;
        }

    offset.x = second->bbox.x + (firstPoint.x-secondPoint.x);
    offset.y = second->bbox.y + (firstPoint.y-secondPoint.y);

     for(int x=BORDER_SIZE; x<second->imgInfo->img->width; x++)
        for(int y=BORDER_SIZE; y<second->imgInfo->img->height; y++)
        {
            if(CV_IMAGE_ELEM(second->imgInfo->img, unsigned char, y, x) == 0)
                ((uchar *)(result->imageData + ((y-BORDER_SIZE+offset.y)*result->widthStep)))[x-BORDER_SIZE+offset.x] = 0;
        }

    /*offset.x = firstPoint.x - secondPoint.x;
    offset.y = firstPoint.y - secondPoint.y;

    for(int x=second->bbox.x; x<second->bbox.x+second->bbox.width; x++)
        for(int y=second->bbox.y; y<second->bbox.y+second->bbox.height; y++)
        {
            if(CV_IMAGE_ELEM(input->img, unsigned char, y, x) == 0)
                ((uchar *)(result->imageData + ((y+offset.y)*result->widthStep)))[x+offset.x] = 0;
        }*/



    return result;
}

int findComponents::mergeComponents(vector <Component> *tempComponents, ImageInfo *input, int pos)
{
    int compSize = (int)tempComponents->size();
    Component *left=NULL,*right=NULL,*comp=NULL;
    vector <CvPoint> contourLeft,contourRight, contourComp;
    vector <CvPoint> closestToLeft,closestToRight;
    double distToLeft=9999.99, distToRight=9999.99;
    IplImage *grouped=NULL;

    comp=&(*tempComponents)[pos];
    contourComp = findcountourPoints(comp->imgInfo->img,cvPoint(comp->bbox.x-BORDER_SIZE,comp->bbox.y-BORDER_SIZE));

    //Tem left se nao for o primeiro
    if(pos != 0)
    {
        left=&(*tempComponents)[pos-1];
        contourLeft = findcountourPoints(left->imgInfo->img,cvPoint(left->bbox.x-BORDER_SIZE,left->bbox.y-BORDER_SIZE));
        closestToLeft = findClosestPoints(contourLeft,contourComp);
        distToLeft = sqrt(pow(closestToLeft[0].x-closestToLeft[1].x,2) + pow(closestToLeft[0].y-closestToLeft[1].y,2));
    }

    //Tem rigth se nao for o ultimo
    if(pos != compSize-1)
    {
        right=&(*tempComponents)[pos+1];
        contourRight = findcountourPoints(right->imgInfo->img,cvPoint(right->bbox.x-BORDER_SIZE,right->bbox.y-BORDER_SIZE));
        closestToRight = findClosestPoints(contourComp,contourRight);
        distToRight = sqrt(pow(closestToRight[0].x-closestToRight[1].x,2) + pow(closestToRight[0].y-closestToRight[1].y,2));
    }


    /* Seleciona o componente mais proximo*/
    if(distToRight < distToLeft)
    {
        grouped = groupImages(input, comp,closestToRight[0],right,closestToRight[1]);
        input->hasJoin = 1;
        right->imgInfo->hasJoin = 1;
        cvReleaseImage(&right->imgInfo->img);
        right->imgInfo->img = preProcessImage(grouped,0,0,cvSize(0,0),BORDER_SIZE);
    }
    else
    {
       grouped = groupImages(input, left,closestToLeft[0],comp,closestToLeft[1]);
       input->hasJoin = 1;
       left->imgInfo->hasJoin = 1;
       cvReleaseImage(&left->imgInfo->img);
       left->imgInfo->img = preProcessImage(grouped,0,0,cvSize(0,0),BORDER_SIZE);

    }

    tempComponents->erase(tempComponents->begin() + pos);
    input->bboxes.erase(input->bboxes.begin() + pos);


    if(grouped !=NULL )
        cvReleaseImage(&grouped);

    return 0;

}

int findComponents::find(ImageInfo *input, int flagMerge)
{
    int i,j;
    unsigned char grayLevel,floodColor=10;
    CvConnectedComp comp;

    Component tempInfo;
    std::ostringstream tempName;
    vector <Component> tempComponents;
    IplImage *im1;
/*    tem que adicionar resize e borda aqui
    alem disso tem que compilar todos, nao um a um*/

    im1 = cvCloneImage(input->img);

    /*Rotula componentes por cor*/
    for(j=0;j<im1->height;j++)
        for(i=0;i<im1->width;i++)
        {
            grayLevel = CV_IMAGE_ELEM(im1, unsigned char, j, i);
            if(grayLevel == FOREGROUND_GRAYSCALE)
            {
                floodColor+=10;

                cvFloodFill(im1,cvPoint(i,j),cvScalarAll(floodColor),cvScalarAll(0),cvScalarAll(0),&comp);

                /* Less than 5x5, reject */
                if(comp.rect.width < 5 && comp.rect.width < 5)
                    continue;


                tempInfo.imgInfo = new ImageInfo();
                tempInfo.floodColor = floodColor;
                tempInfo.bbox = comp.rect;

                tempInfo.imgInfo->img = extractComponentBlob(im1,comp.rect,floodColor,BORDER_SIZE);
                input->bboxes.push_back(comp.rect);
                
                tempComponents.push_back(tempInfo);

            }

        }

    //Correcao do BUG de ordencao: Ordeno o vectr, para ficar na ordem dos digitos da string
    if((int)tempComponents.size() > 0)
        sort(tempComponents.begin(),tempComponents.end(),sort_components());


   if(flagMerge == 1)
    this->brokenDigitLESOliveira(&tempComponents,input);

    //insiro os componentes para o input
    for(int i=0;i<(int)tempComponents.size();i++)
    {
        tempName.str("");
        tempName << input->filename.substr(0,input->filename.size()-4) ;
        tempName << "-comp" << (int)i+1 << "of" << (int)tempComponents.size() << ".png" ;
        tempComponents[i].imgInfo->filename = tempName.str();
        input->components.push_back(tempComponents[i].imgInfo);
    }

    cvReleaseImage(&im1);
    return 0;

}

int findComponents::brokenDigitLESOliveira(vector <Component> *tempComponents, ImageInfo *input)
{
    int SI,CCabove,CCbelow,Max,Min,Ratio; //median line
    bool cond1;
    static int ratioThresh=5;
    Component *comp, *afterComp;


    SI = input->img->height/2;

    /*cout << "Sizes: " << (int)tempComponents->size();*/
    for(int i=0;i<(int)tempComponents->size();i++)
    {
        comp = &(*tempComponents)[i];

        if(i<(int)tempComponents->size()-1)
            afterComp = &(*tempComponents)[i+1];
        else
            afterComp = &(*tempComponents)[i];
            
        cond1 = false;
        //if doesnt intercept == true
        if(!(comp->bbox.y < SI && comp->bbox.y + comp->bbox.height > SI))
            cond1 = true;

        CCabove = SI - comp->bbox.y;
        CCbelow = comp->bbox.y + comp->bbox.height - SI;

        if(CCabove >= CCbelow)
        {
            Max = CCabove;
            Min = CCbelow;
        }
        else
        {
            Max = CCbelow;
            Min = CCabove;
        }
	
	if(Min != 0)
	  Ratio = Max/Min;     
        else
	  Ratio = 0;

        //Se nao cruzar, ou cruzar e o ratio > thresh
        if(cond1 == true || (cond1 == false && Ratio > ratioThresh)//@LESOliveira2002
            || comp->bbox.height < afterComp->bbox.height/2)//broken4 @andre
            {
                mergeComponents(tempComponents,input,i);
                i--; //decrementa para nao pular o indice excluido no merge
            }

    }

    return 0;
}
