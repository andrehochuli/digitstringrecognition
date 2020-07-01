#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "preProcess.h"


int main(int argc, char **argv)
{
	IplImage *img, *imgProcessed;
	char imgPath[MAX_STRING],imgFilename[MAX_STRING],lineString[MAX_STRING*3];
	char *filein,*basePath,*savePath;
	int imgsCount;
	FILE *fpin;

	if(argc < 4)
	{
		fprintf(stderr,"Usage: %s base_path save_path list_of_images\n", argv[0]);
		return -1;
	}

	basePath = argv[1];
	savePath = argv[2];
	filein = argv[3];


	fpin = fopen(filein,"r");
	if(!fpin)
	{
		fprintf(stderr,"Erro ao abrir %s\n",filein);
		return -1;
	}

	imgsCount = 0;

	while (!feof(fpin))
	{
		memset(lineString,0,sizeof(char)*MAX_STRING*3);


		if(fgets(lineString,sizeof(char)*MAX_STRING*3,fpin) == NULL)
		{
			fprintf(stderr,"Fim do Arquivo\n");
			fclose(fpin);
			return -1;
		}

		sscanf(lineString,"%s\n",imgFilename);

		sprintf(imgPath,"%s/%s",basePath,imgFilename);

		fprintf(stdout,"Processing %s [ %02d ]\n",imgPath,imgsCount+1);
		img = NULL;
		img = cvLoadImage(imgPath,0);
		if(!img)
		{
			fprintf(stderr, "Erro ao abrir %s (%s)\n",imgPath,imgFilename);
                        fprintf(stdout, "Erro ao abrir %s (%s)\n",imgPath,imgFilename);
			//fclose(fpin);
			//return -1;
			continue;

		}

		#ifdef SHOW_RESULTS_IMGS
		cvShowImage("img",img);
		#endif


		/** Pre-processing = Gaussian Filter, Threshold, Bbox */
        int smoothSize=3;
        int slant=1;
        CvSize normSize = cvSize(0,0);
        int borderSize = 5;
		imgProcessed = preProcessImage(img,smoothSize,slant,normSize,borderSize);

		/**--------------------------------------------------*/


		#ifdef SHOW_RESULTS_IMGS
		cvShowImage("imgProcessed",imgProcessed);
		cvWaitKey(0);
		#endif

		sprintf(imgPath,"%s/%s",savePath,basename(imgFilename));
		printf("Saving %s\n",imgPath);

		cvSaveImage(imgPath,imgProcessed);

		cvReleaseImage(&imgProcessed);
		cvReleaseImage(&img);
		imgsCount++;

	} //while read file

	fclose(fpin);
	printf("Fim do Processo\n");
	return 0;
}

