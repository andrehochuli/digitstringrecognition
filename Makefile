#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++ 
WINDRES = windres

INC = 
CFLAGS = -Wall -fexceptions
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG = $(INC) `pkg-config --cflags opencv` -I./include/ -I./include/interfaces -Iinclude -I$(CAFFE_ROOT)/include -I$(CAFFE_ROOT)/build/include 
INC_DEBUG += -IpreProcess -I/usr/local/include/ -I /usr/local/cuda/include/ 

CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR) `pkg-config --libs opencv` -L/home/aghochuli/Projects/caffe/build/lib/
LIB_DEBUG = $(LIB) libpreprocess/libpreprocess.a
LDFLAGS_DEBUG = $(LDFLAGS) libcnnclassification/libcnnclassification.a  -L$(CAFFE_ROOT)/build/lib/ -lcaffe `pkg-config --libs opencv`  -lgflags -lglog -lcudart -lcurand -lcublas  -lcblas -lprotobuf -lhdf5_serial -lhdf5_serial_hl -lboost_system -lboost_thread
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/digitStringRecognition

INC_RELEASE = $(INC) -Iinclude `pkg-config --cflags opencv` -Iinclude/interfaces -I./include/ -I./include/interfaces -I$(CAFFE_ROOT)/include -I$(CAFFE_ROOT)/build/include
INC_RELEASE += -I$(CAFFE_ROOT)/include -I$(CAFFE_ROOT)/build/src -IpreProcess -I /usr/local/include/ -I /usr/local/cuda/include/ 

CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR) `pkg-config --libs opencv` -L/home/aghochuli/Projects/caffe/build/lib/
LIB_RELEASE = $(LIB)libpreprocess/libpreprocess.a
LDFLAGS_RELEASE = $(LDFLAGS) -s libcnnclassification/libcnnclassification.a  -L$(CAFFE_ROOT)/build/lib/ -lcaffe `pkg-config --libs opencv` -L/usr/local/cuda/lib64 -lgflags -lglog  -lcudart -lcurand -lcublas  -lcblas -lprotobuf -lhdf5_serial -lhdf5_serial_hl -lboost_system -lboost_thread -lboost_filesystem
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/digitStringRecognition

OBJ_DEBUG = $(OBJDIR_DEBUG)/interfaces/verificationInterface.o $(OBJDIR_DEBUG)/src/processResults.o $(OBJDIR_DEBUG)/src/imageLoader.o $(OBJDIR_DEBUG)/src/imageInfo.o $(OBJDIR_DEBUG)/src/handWNumeralRecognizer_v1.o $(OBJDIR_DEBUG)/src/handWNumeralRecognizer_1110Classes.o $(OBJDIR_DEBUG)/src/findComponents.o $(OBJDIR_DEBUG)/src/cnnClassifier.o $(OBJDIR_DEBUG)/src/Fusion.o $(OBJDIR_DEBUG)/preProcess/preProcess_v1.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/interfaces/recognizer.o $(OBJDIR_DEBUG)/interfaces/preProcessInterface.o $(OBJDIR_DEBUG)/interfaces/classifierInterface.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/interfaces/verificationInterface.o $(OBJDIR_RELEASE)/src/processResults.o $(OBJDIR_RELEASE)/src/imageLoader.o $(OBJDIR_RELEASE)/src/imageInfo.o $(OBJDIR_RELEASE)/src/handWNumeralRecognizer_v1.o $(OBJDIR_RELEASE)/src/handWNumeralRecognizer_1110Classes.o $(OBJDIR_RELEASE)/src/findComponents.o $(OBJDIR_RELEASE)/src/cnnClassifier.o $(OBJDIR_RELEASE)/src/Fusion.o $(OBJDIR_RELEASE)/preProcess/preProcess_v1.o $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/interfaces/recognizer.o $(OBJDIR_RELEASE)/interfaces/preProcessInterface.o $(OBJDIR_RELEASE)/interfaces/classifierInterface.o

all: release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/interfaces || mkdir -p $(OBJDIR_DEBUG)/interfaces
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src
	test -d $(OBJDIR_DEBUG)/preProcess || mkdir -p $(OBJDIR_DEBUG)/preProcess
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/interfaces/verificationInterface.o: interfaces/verificationInterface.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c interfaces/verificationInterface.cpp -o $(OBJDIR_DEBUG)/interfaces/verificationInterface.o

$(OBJDIR_DEBUG)/src/processResults.o: src/processResults.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/processResults.cpp -o $(OBJDIR_DEBUG)/src/processResults.o

$(OBJDIR_DEBUG)/src/imageLoader.o: src/imageLoader.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/imageLoader.cpp -o $(OBJDIR_DEBUG)/src/imageLoader.o

$(OBJDIR_DEBUG)/src/imageInfo.o: src/imageInfo.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/imageInfo.cpp -o $(OBJDIR_DEBUG)/src/imageInfo.o

$(OBJDIR_DEBUG)/src/handWNumeralRecognizer_v1.o: src/handWNumeralRecognizer_v1.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handWNumeralRecognizer_v1.cpp -o $(OBJDIR_DEBUG)/src/handWNumeralRecognizer_v1.o

$(OBJDIR_DEBUG)/src/handWNumeralRecognizer_1110Classes.o: src/handWNumeralRecognizer_1110Classes.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handWNumeralRecognizer_1110Classes.cpp -o $(OBJDIR_DEBUG)/src/handWNumeralRecognizer_1110Classes.o

$(OBJDIR_DEBUG)/src/findComponents.o: src/findComponents.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/findComponents.cpp -o $(OBJDIR_DEBUG)/src/findComponents.o

$(OBJDIR_DEBUG)/src/cnnClassifier.o: src/cnnClassifier.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/cnnClassifier.cpp -o $(OBJDIR_DEBUG)/src/cnnClassifier.o

$(OBJDIR_DEBUG)/src/Fusion.o: src/Fusion.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Fusion.cpp -o $(OBJDIR_DEBUG)/src/Fusion.o

$(OBJDIR_DEBUG)/preProcess/preProcess_v1.o: preProcess/preProcess_v1.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c preProcess/preProcess_v1.cpp -o $(OBJDIR_DEBUG)/preProcess/preProcess_v1.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/interfaces/recognizer.o: interfaces/recognizer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c interfaces/recognizer.cpp -o $(OBJDIR_DEBUG)/interfaces/recognizer.o

$(OBJDIR_DEBUG)/interfaces/preProcessInterface.o: interfaces/preProcessInterface.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c interfaces/preProcessInterface.cpp -o $(OBJDIR_DEBUG)/interfaces/preProcessInterface.o

$(OBJDIR_DEBUG)/interfaces/classifierInterface.o: interfaces/classifierInterface.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c interfaces/classifierInterface.cpp -o $(OBJDIR_DEBUG)/interfaces/classifierInterface.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/interfaces
	rm -rf $(OBJDIR_DEBUG)/src
	rm -rf $(OBJDIR_DEBUG)/preProcess
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/interfaces || mkdir -p $(OBJDIR_RELEASE)/interfaces
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src
	test -d $(OBJDIR_RELEASE)/preProcess || mkdir -p $(OBJDIR_RELEASE)/preProcess
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/interfaces/verificationInterface.o: interfaces/verificationInterface.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c interfaces/verificationInterface.cpp -o $(OBJDIR_RELEASE)/interfaces/verificationInterface.o

$(OBJDIR_RELEASE)/src/processResults.o: src/processResults.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/processResults.cpp -o $(OBJDIR_RELEASE)/src/processResults.o

$(OBJDIR_RELEASE)/src/imageLoader.o: src/imageLoader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/imageLoader.cpp -o $(OBJDIR_RELEASE)/src/imageLoader.o

$(OBJDIR_RELEASE)/src/imageInfo.o: src/imageInfo.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/imageInfo.cpp -o $(OBJDIR_RELEASE)/src/imageInfo.o

$(OBJDIR_RELEASE)/src/handWNumeralRecognizer_v1.o: src/handWNumeralRecognizer_v1.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handWNumeralRecognizer_v1.cpp -o $(OBJDIR_RELEASE)/src/handWNumeralRecognizer_v1.o

$(OBJDIR_RELEASE)/src/handWNumeralRecognizer_1110Classes.o: src/handWNumeralRecognizer_1110Classes.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handWNumeralRecognizer_1110Classes.cpp -o $(OBJDIR_RELEASE)/src/handWNumeralRecognizer_1110Classes.o

$(OBJDIR_RELEASE)/src/findComponents.o: src/findComponents.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/findComponents.cpp -o $(OBJDIR_RELEASE)/src/findComponents.o

$(OBJDIR_RELEASE)/src/cnnClassifier.o: src/cnnClassifier.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/cnnClassifier.cpp -o $(OBJDIR_RELEASE)/src/cnnClassifier.o

$(OBJDIR_RELEASE)/src/Fusion.o: src/Fusion.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Fusion.cpp -o $(OBJDIR_RELEASE)/src/Fusion.o

$(OBJDIR_RELEASE)/preProcess/preProcess_v1.o: preProcess/preProcess_v1.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c preProcess/preProcess_v1.cpp -o $(OBJDIR_RELEASE)/preProcess/preProcess_v1.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/interfaces/recognizer.o: interfaces/recognizer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c interfaces/recognizer.cpp -o $(OBJDIR_RELEASE)/interfaces/recognizer.o

$(OBJDIR_RELEASE)/interfaces/preProcessInterface.o: interfaces/preProcessInterface.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c interfaces/preProcessInterface.cpp -o $(OBJDIR_RELEASE)/interfaces/preProcessInterface.o

$(OBJDIR_RELEASE)/interfaces/classifierInterface.o: interfaces/classifierInterface.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c interfaces/classifierInterface.cpp -o $(OBJDIR_RELEASE)/interfaces/classifierInterface.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/interfaces
	rm -rf $(OBJDIR_RELEASE)/src
	rm -rf $(OBJDIR_RELEASE)/preProcess
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

