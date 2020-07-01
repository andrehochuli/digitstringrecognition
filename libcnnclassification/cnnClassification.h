#ifndef _CNN_CLASSIFICATION_H
#define _CNN_LASSIFICATION_H

#include <boost/bimap.hpp>
#include "caffe/caffe.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

enum NORMALIZATION_TYPES
{
	MINMAX=1,
	MEAN_IMAGE
};


using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using namespace std;

typedef struct Prediction
{
   int classID;
   string className;
   float score;
}Prediction;

/* Pair (filename, label (class id 0->N) */
typedef std::pair<string, int> FilenameAndLabel;

class Classifier {
 public:
  Classifier(const string& model_file,
             const string& trained_file,
             const string& mean_file,
             const string& label_file,
             NORMALIZATION_TYPES _normalizationType);


  std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);

  std::vector< vector<Prediction> > ClassifyBatch(int N = 5);

  int getNumberOfClasses()
  {
    return (int) labels_.size();
  }

  vector<cv::Mat> batchImgs;



 private:
  void SetMean(const string& mean_file);

  std::vector<float> Predict(const cv::Mat& img);

  std::vector< float >  PredictBatch();

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void WrapBatchInputLayer(std::vector<std::vector<cv::Mat> > *input_batch);

  void Preprocess(const cv::Mat& img,
                std::vector<cv::Mat>* input_channels);

  void PreprocessBatch(std::vector< std::vector<cv::Mat> >* input_batch);


 private:
  boost::shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
  NORMALIZATION_TYPES normalizationType;
};



#endif
