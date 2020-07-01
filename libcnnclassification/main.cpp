#include "cnnClassification.h"

int main(int argc, char** argv) {

  if (argc < 9) {
    std::cerr << "Usage: " << argv[0]
              << " deploy.prototxt network.caffemodel"
              << " mean.binaryproto labels.txt list_of_files.txt base_path"
              << " NormalizationType: MINMAX | MEAN_IMAGE batch_size [nPREDICTIONSS]" << std::endl;
    return 1;
  }

  ::google::InitGoogleLogging(argv[0]);

  char list_file[1204],base_path[1024];
  char file[256];
  char fullpath[1024];
  int label;
  NORMALIZATION_TYPES normalizationType;
  vector < FilenameAndLabel > files;
  int error=0, ok=0, nPREDS=1, top[5];
  string model_file   = argv[1];
  string trained_file = argv[2];
  string mean_file    = argv[3];
  string label_file   = argv[4];
  strcpy(list_file,argv[5]);
  strcpy(base_path,argv[6]);
  cv::Mat img;


  memset(top,0,sizeof(int)*5);

  if(strcmp("MINMAX",argv[7]) == 0)
	normalizationType=MINMAX;
  else if(strcmp("MEAN_IMAGE",argv[7]) == 0)
	normalizationType=MEAN_IMAGE;
  else
  {
        cout << "==>CONFIG ERROR: NormalizationType \"" << argv[7] << "\" unrecognized. Use MINMAX or MEAN_IMAGE <== CONFIG ERROR" << endl;
	return -1;
  }


  int batchSize = atoi(argv[8]);

  if(argc > 9) //Se foi informado o numero de Preds!!, senao o default é 1
     nPREDS=atoi(argv[9]);
  else
     nPREDS = 1;

  Classifier classifier(model_file, trained_file, mean_file, label_file,normalizationType);



  FILE *fp = fopen(list_file,"r");
  if(!fp)
  {
	 fprintf(stderr,"Can't read %s\n",list_file);
	 return 0;
  }


  int errorFlag=0;
  while (!feof(fp))
  {
	  //cout << "Batch: " << batchCount++ << endl;
	  while(!feof(fp) && (int)files.size() < batchSize)
	  {


		  fscanf(fp,"%s %d\n",file,&label);


		  sprintf(fullpath,"%s/%s", base_path, file);
		  img = cv::imread(fullpath, -1);
		  if(!img.data)
          {
            cout << "Can't open " << fullpath << endl;
            continue;
          }

		  img.convertTo(img, CV_32F);
		  classifier.batchImgs.push_back(img);

		  files.push_back(std::make_pair(fullpath,label));
	  }
	//	  std::cout << "---------- Prediction for "
	//				<< file << " ----------" << std::endl;

		CHECK(!img.empty()) << "Unable to decode image " << file;
		std::vector< std::vector <Prediction> >pred = classifier.ClassifyBatch(nPREDS);

		std::ofstream errorFile;
		errorFile.open ("errors.txt", std::ofstream::out | std::ofstream::app);

		for(int i=0;i < (int) pred.size();i++)
		{
			std::vector <Prediction> predictions = pred[i];

			if(predictions.size() > 0)
			{
				//TOP1

				Prediction pred = predictions[0];
								
				if(pred.classID == files[i].second)
					ok++;
				else
				{
					error++;
					//cout << "Error --> ";
					errorFile << files[i].first << " Label:" << std::setw(2) << std::setfill('0') << files[i].second
								<< " Preds:" << std::setw(2) << std::setfill('0') << pred.classID << " ClassName:" << pred.className
															<< " Score:" << std::fixed << std::setprecision(4) << pred.score;
					errorFlag = 1;


				}

				std::cout << files[i].first << " Label:" << std::setw(2) << std::setfill('0') << files[i].second
								<< " Preds:" << std::setw(2) << std::setfill('0') << pred.classID << " ClassName:" << pred.className
															<< " Score:" << std::fixed << std::setprecision(4) << pred.score;


				//TOP K
				for(int k=0; k<(int)predictions.size(); k++)
				{
						Prediction pred = predictions[k];
						
						
						if(pred.classID == files[i].second)
							top[k]++;

						if(errorFlag == 1)
						{
							errorFile << " | [Preds:" << std::setw(2) << std::setfill('0') << pred.classID << " ClassName:"
										<< pred.className << " Score:" << std::fixed << std::setprecision(4) << pred.score << "]";


						}

						
						cout << " | [Preds:" << std::setw(2) << std::setfill('0') << pred.classID << " ClassName:"
									<< pred.className << " Score:" << std::fixed << std::setprecision(4) << pred.score << "]";



				}


				cout << endl;
				if(errorFlag == 1)
					errorFile << endl;

				errorFlag = 0;

			}


		}//for preds


		img.release();

		errorFile.close();

		for(int i=0;i < (int) classifier.batchImgs.size();i++)
			classifier.batchImgs[i].release();

		classifier.batchImgs.clear();
		files.clear();
	}//feof



   int total = ok + error;

   float acc = (float) ok / (float) total;
   cout <<  "Accuracy: " << std::fixed << std::setprecision(4) << acc << " ( " << ok << " / " << total << " [ " << total-ok << " ] ) " << endl;

   for(int k=0; k<(int)nPREDS; k++)
   {

	   if(k > 0)
		top[k] += top[k-1];

	   float acc = (float) top[k] / (float) total;
	    cout <<  "Top " << std::fixed << std::setprecision(4) << k+1 << ": "
					<< acc << " ( " << top[k] << " / " << total << " [ " << total - top[k] << " ] ) " << endl;

   }

   return 0;
}

