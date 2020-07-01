CPP=g++-5

OPENCV_INCLUDE=/home/aghochuli/INSTALL/opencv-2.4.11/INSTALL/include/
OPENCV_LIB=/home/aghochuli/INSTALL/opencv-2.4.11/INSTALL/lib

CFLAGS=-Wall -Wno-unused-result  -std=c++11
INCLUDES= -I /usr/local/include/ -I /usr/local/cuda/include/
INCLUDES+= -I$(OPENCV_INCLUDE) -I$(OPENCV_INCLUDE)/opencv -I $(CAFFE_ROOT)/include/ -I $(CAFFE_ROOT)/build/src/

LIBS= libcnnclassification.a -lcaffe -L$(OPENCV_LIB) -lopencv_core -lopencv_highgui -lopencv_imgproc -lgflags -lglog -lopencv_core 
LIBS+=-L/usr/local/cuda/lib64 -lcudart -lcurand -lcublas  -lcblas 
LIBS+=-lprotobuf -lhdf5_serial -lhdf5_serial_hl -lboost_system -lboost_thread
LIBS+= -L../caffe/build/lib/ -lcaffe 


all: libcnnclassification.a classification

rebuild: clean all

classification: main.cpp libcnnclassification.a
	$(CPP) $(CFLAGS) $(INCLUDES) main.cpp -o classification $(LIBS)

cnnClassification.o: cnnClassification.cpp cnnClassification.h 
	$(CPP) $(CFLAGS) $(INCLUDES) -c cnnClassification.cpp -o cnnClassification.o 

libcnnclassification.a: cnnClassification.o 
	ar rcs libcnnclassification.a *.o

clean:
	rm classification *.o *.a
