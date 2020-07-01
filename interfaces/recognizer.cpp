#include "recognizer.h"

recognizerInterface::recognizerInterface()
{
    //ctor
}



recognizerInterface::~recognizerInterface()
{
   for(int i=0; i<(int)preProcess.size();i++)
        delete preProcess[i];

    preProcess.clear();

    for(int i=0; i<(int)classifiers.size();i++)
        delete classifiers[i];

    classifiers.clear();

    for(int i=0; i<(int)verification.size();i++)
        delete verification[i];

    verification.clear();

}
