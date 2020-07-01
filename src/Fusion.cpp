#include "Fusion.h"
#include "common.h"

Fusion::Fusion()
{
    maxTop = 2;
    winner.score = -1;
}

Fusion::~Fusion()
{
    //dtor
}

void Fusion::generateHyp(ImageInfo *img, int pos, hypothesis *node)
{
   /* if(pos >= (int)img->components.size())
    {
        if(node->pred.score >= winner.score)
        {
            winner.score = node->pred.score;
            winner.className = node->pred.className;
            winner.classID = node->pred.classID;

        }

        cout <<  node->pred.className << " " <<  node->pred.score << endl;
        return;
    }

    hypothesis hyp;
     for(int top=0;top<this->maxTop;top++)
    {
        hyp.pred.className.clear();
        hyp.pred.score = 1;
        hyp.pred = img->components[pos]->topPreds[top][0];


        hyp.pred.className.insert(0,node->pred.className);

        hyp.pred.score *= node->pred.score;

        node->hyp.push_back(hyp);


    }

    pos++;

    for(int i=0;i<(int)node->hyp.size();i++)
        generateHyp(img,pos,&node->hyp[i]);*/

}

void Fusion::viterbi(ImageInfo *img, int begin)
{

    if(begin >= (int)img->components.size())
        return;

    viterbi(img,begin+1);

    for(int i=begin;i<(int)img->components.size();i++)
    {


    }




    return;
}


Prediction Fusion::process(ImageInfo *img)
{
    //generateHyp(img,0,&hyp_tree);
	cout << "NAO TEM NADA IMPLEMENTADO AQUI" << endl;


    return winner;
}
