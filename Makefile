CC = g++
CXXFLAGS = -I/usr/include -Wno-write-strings
LDLIBS = -lglut -lGLEW -lGL -lGLU -lX11
LDFLAGS = -L/usr/lib -L/usr/lib/nvidia-304-updates

mainOfiles = motionviewer.o GlTransformable.o Camera.o MainUtility.o BvhParser.o Skeleton.o
mainHeaders = MainUtility.h Skeleton.h BvhParser.h FrameTimer.h


all: motionviewer

motionviewer: $(mainOfiles)

motionviewer.o: motionviewer.cpp $(mainHeaders)

GlTransformable.o: GlTransformable.cpp GlTransformable.h

Camera.o: Camera.cpp Camera.h

MainUtility.o: MainUtility.cpp MainUtility.h

BvhParser.o: BvhParser.cpp BvhParser.h 

Skeleton.o: Skeleton.cpp Skeleton.h

clean:
	rm -rf motionviewer *.o
	
zip:
	zip motionviewer.zip *.cpp *.h Makefile