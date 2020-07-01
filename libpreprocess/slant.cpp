#include "slant.h"

float cv_slant_estbycc(IplImage *input, int *yorigin,int flagInvertColor)
{

   int i=0,total=0, numcc=0;
   char chainCode;
   float mReta;
   float somamvReta;
   int vet[MAXCC][NCOD];   /* NCOD = chain codes (see mkfs.h)                                */
   int lenght[MAXCC];      /* MAXCC = maximum number of components in the string (see mkfs.h)*/
   float somapesos;
   float def=3.14159/2;    /* normal slant */
   float mvReta[MAXCC];
   IplImage *im1;

   CvMemStorage* storage = cvCreateMemStorage(0);
   CvChain *chain; // it will be freed by cvReleaseMemStorage

   memset(vet,0,NCOD*MAXCC*sizeof(int));
   memset(lenght,0,MAXCC*sizeof(int));
   memset(mvReta,0,MAXCC*sizeof(float));

   im1 = cvCloneImage(input);

    if(flagInvertColor == 1)
        cvThreshold(im1,im1,127,255,CV_THRESH_BINARY_INV);

   cvFindContours(im1, storage, (CvSeq**)(&chain),
                        sizeof(*chain),CV_RETR_EXTERNAL, CV_CHAIN_CODE);

    cvReleaseImage(&im1);

    mReta=0; *yorigin=-1;
    for(;chain!=NULL;chain=(CvChain*)chain ->h_next)
    {
        //chain=(CvChain*)chain ->h_next;
        if(chain==NULL){break;}

        CvSeqReader reader;
        total = chain->total;

        cvStartReadSeq((CvSeq*)chain,&reader,0);

        if(chain->total > MINLENGHT)
        {

            if(chain->origin.y > *yorigin)
                 *yorigin = chain->origin.y;


            for(i=0;i<total;i++)
            {
                CV_READ_SEQ_ELEM(chainCode, reader);

                if ((int)chainCode != 4 && (int)chainCode !=0)
                {
                    vet[numcc][(int)chainCode]++;
                    lenght[numcc]=total;
                }
            }

            numcc++;
		
	    /* Avoids seg fault */
	    if(numcc >= MAXCC)
            {
		fprintf(stderr,"Warning: Reach MAXCC, but not stop process. Maybe slant will be done wrong.\n");
		break;
	    }

        }
    }

  cvReleaseMemStorage(&storage);

  somamvReta=0; somapesos=0;
  for (i=0; i<numcc; i++) {

    if (((vet[i][5]+vet[i][1]) - (vet[i][7]+vet[i][3]))!=0) {
      mvReta[i]=(vet[i][1]+vet[i][2]+vet[i][3]+vet[i][5]+vet[i][6]+vet[i][7]) / ((vet[i][5]+vet[i][1]) - (vet[i][7]+vet[i][3]));


      mvReta[i]= (float) atan((double) mvReta[i]);

      if (mvReta[i]<0) mvReta[i]=mvReta[i]+def; else mvReta[i]=mvReta[i]-def;
      somamvReta=somamvReta+mvReta[i]*lenght[i];
      somapesos=somapesos+lenght[i];
     }
  }

  if (somapesos!=0)
    mReta=somamvReta/somapesos;




  return mReta;

}

IplImage * cv_slant_correction(IplImage *im1, float angle, int yorigin, int contourVal)
{

   IplImage *im2;
   int x,y,xl,wOffset,hOffset;
   float tanAngle,desv;
	
   /*Creates one 2x higher, because slant maybe projects outlier points*/
   im2=cvCreateImage(cvSize(im1->width*2,im1->height*2),im1->depth,im1->nChannels);

   if(contourVal == 0)
	cvSet(im2,cvScalarAll(255));
   else
    cvSet(im2,cvScalarAll(0));

   tanAngle = tan(angle);
   hOffset = im2->height/6;	
   wOffset = im2->width/6;
   for (y=0; y<im1->height ; y++)
   {

     desv = (y-yorigin)*tanAngle;

     for (x=0; x<im1->width; x++)
     {

        xl=(int) (x - desv);

        if (xl >= 0 && xl < im2->width)
            ((uchar *)(im2->imageData + ((y+hOffset)*im2->widthStep)))[xl+wOffset] = ((uchar *)(im1->imageData + y*im1->widthStep))[x];


    }

   }

   return im2;

}
