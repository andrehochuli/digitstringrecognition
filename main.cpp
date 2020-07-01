#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "interfaces/preProcessInterface.h"
#include "interfaces/recognizer.h"
#include "preProcess/preProcess_v1.h"
#include "handWNumeralRecognizer_v1.h"
#include "handWNumeralRecognizer_1110Classes.h"


#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if(argc < 3)
    {
        cout << "Usage: " << argv[0] << " list_of_labeled_files method=[DS|1110]" << endl;
        return 0;
    }

    
    float fThreshold = 0.95;    
    FUSION_TYPE fType = NO_FUSION;
    recognizerInterface *recognizer = NULL;
    
    if(strcmp(argv[2],"DS") ==0)
    {
        recognizer = new handWNumeralRecognizer_v1(argv[1]);
        fType = DIGIT_MAX_FUSION;
    }
    else if(strcmp(argv[2],"1110") ==0)
    {
			recognizer = new handWNumeralRecognizer_1110Classes(argv[1]);			
	}	
    
    recognizer->process(1,0,fType,fThreshold);     
    	
    delete recognizer;

	
    return 0;
}
