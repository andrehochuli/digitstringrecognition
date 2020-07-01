#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include "common.h"
#include "../libcnnclassification/cnnClassification.h"

using namespace std;
using namespace cv;


#define MAX_TOP 5
/* Prediction Tuple : classID, className , confidence/score */
/* Ex: Prediction Length 00, isolated or 01, 0.9287 */
/* Ex: Prediction Pair 23, 23, 0.9876 */

class ImageInfo
{
    public:
        IplImage *img;
        int hasJoin;
        vector <CvRect> bboxes;
        string filename, label;
        vector <Prediction> LEM,DSM[4]; //1,2,3 DSM classifier
        vector <ImageInfo *> components;
		int nSegLines,nSegs;
	
        string getPredictChain();
        float getPredictScore();
        string getPredictChainByComp();
        string getPredictSizeByComp();
        int getPredictSize();

        ImageInfo();
        ~ImageInfo();
        ImageInfo& operator=(const ImageInfo& src);


};
#endif // IMAGEINFO_H
