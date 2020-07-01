#ifndef PROCESSRESULTS_H
#define PROCESSRESULTS_H

#include "common.h"

enum SAVE_TYPE
{
    SAVE_NONE=0,
    SAVE_ALL,
    SAVE_ERRORS_ONLY
};

enum SHOW_TYPE
{
    SHOW_NONE=0,
    SHOW_ALL,
    SHOW_ERRORS_ONLY
};

enum FUSION_TYPE
{
	NO_FUSION=0,
	SUM_FUSION,
	PROD_FUSION,
	DIGIT_MAX_FUSION,
    CNN_1110_TOP1,
    CNN_1110_MAX_DIGIT,
    CNN_1110_PROD,
    CNN_1110_DIGIT_ONLY //NO_LENGTH
		
};

class statsInfo
{
    public:
    int nFiles;
    int nOK;
    int sizeErrors;
    int recErrors;
    int onlyLength;
    unsigned int CallsLEM,Calls1D,Calls2D,Calls3D,Calls4D;

    void clear()
    {
        nFiles=0;
        nOK=0;
        sizeErrors=0;
        recErrors=0;
        onlyLength=0;
        CallsLEM=0;
        Calls1D=0;
        Calls2D=0;
        Calls3D=0;
        Calls4D=0;
    };

};

typedef struct FusionInfo
{
	ImageInfo *main;
	vector <Prediction> LEM,DSM;
	
	int predictSize;	
	string predictChain, predictChainComp, predictSizeComp;	
	string result;
	float predictionScore,fusionThreshold;
	
} FusionInfo;

class processResults
{
    public:
        processResults();
        int process( vector <ImageInfo*> &imgsProcessed, statsInfo *stats,
                        SHOW_TYPE flagShow=SHOW_ALL, SAVE_TYPE flagSave=SAVE_NONE, float fThreshold = 0.95 );
        int process( ImageInfo *result, statsInfo *stats, SHOW_TYPE flagShow, SAVE_TYPE flagSave, float fThreshold);
        float fusion(FusionInfo *fInfo,statsInfo *stats);
        float fusion_1110Classes(FusionInfo *fInfo,statsInfo *stats);
        void saveResult(FusionInfo *info);
        FUSION_TYPE fType;
        virtual ~processResults();
        string saveDir;
    protected:
    private:
};

#endif // PROCESSRESULTS_H
