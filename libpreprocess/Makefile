CC=gcc
CPP=g++-5 
CFLAGS=-Wall -fPIC

OPENCV_INCLUDE=/home/aghochuli/INSTALL/opencv-2.4.11/INSTALL/include/
OPENCV_LIB=/home/aghochuli/INSTALL/opencv-2.4.11/INSTALL/lib


INCLUDES=-I$(OPENCV_INCLUDE) -I$(OPENCV_INCLUDE)/opencv 

LIBS=-L$(OPENCV_LIB) -lopencv_core -lopencv_highgui -lopencv_imgproc

all: libpreprocess.a processBaseDemo

rebuild: clean all

processBaseDemo: processBase.cpp globals.h libpreprocess.a
	$(CPP) $(CFLAGS) $(INCLUDES) processBase.cpp  preProcess.o slant.o -o processBaseDemo $(LIBS)

preProcess.o: preProcess.cpp preProcess.h globals.h
	$(CPP) $(CFLAGS) $(INCLUDES) -c preProcess.cpp -o preProcess.o

slant.o: slant.cpp slant.h globals.h
	$(CPP) $(CFLAGS) $(INCLUDES) -c slant.cpp -o slant.o

libpreprocess.a: slant.o preProcess.o
	ar rcs libpreprocess.a *.o

clean:
	rm processBaseDemo *.o *.a
