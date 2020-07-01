#include "imageLoader.h"



imageLoader::imageLoader()
{

}

imageLoader::~imageLoader()
{
    if(!fp.is_open())
        fp.close();

    releaseImages();
}

int imageLoader::releaseImages()
{
    for(int i=0;i<(int)imgs.size();i++)
        delete imgs[i];

    return 0;
}

int imageLoader::loadImages(string filename, int NImgs)
{
    int i=0;
    ImageInfo *load;

    if(fp.eof())
        return -1;

    releaseImages();

    if(!fp.is_open())
    {
        fp.open(filename.c_str());
        if(!fp.is_open())
        {
            cout << "[ERROR] Can't open " << filename << endl;
            return -1;
        }
    }

    while(i<NImgs)
    {
        i++;

        load = new ImageInfo(); //Dont free it here, destructor of std::vector will done !

        fp >> load->filename;
        fp >> load->label;

        if(fp.eof())
        {
            cout << "[ImageLoader] ==> End of File ==> Stopped" << endl;
            delete load;
            break;
        }

        load->img = cvLoadImage(load->filename.c_str(),CV_LOAD_IMAGE_GRAYSCALE);        
        if(!load->img)
        {
            cout << "[ERROR] Can't load " << load->filename << endl;
            delete load;
            continue;
        }

        imgs.push_back(load);
    }



    return 0;
}
