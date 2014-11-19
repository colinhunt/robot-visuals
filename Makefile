CC = g++
CXXFLAGS = -I/usr/include -Wno-write-strings
LDLIBS = -lglut -lGLEW -lGL -lGLU -lX11
LDFLAGS = -L/usr/lib -L/usr/lib/nvidia-304-updates

mainOfiles = model.o GlTransformable.o Camera.o MainUtility.o
mainHeaders = model.h Camera.h MainUtility.h


mainOfiles = model.o GlTransformable.o Camera.o TGALoader.o
mainHeaders = model.h Camera.h TGALoader.h


all: solidviewer attachviewer personviewer

solidviewer: solidviewer.o $(mainOfiles)

solidviewer.o: solidviewer.cpp $(mainHeaders)

attachviewer: attachviewer.o $(mainOfiles) BvhParser.o Skeleton.o Articulator.o

attachviewer.o: attachviewer.cpp $(mainHeaders) BvhParser.h Skeleton.h Articulator.h

personviewer: personviewer.o $(mainOfiles) BvhParser.o Skeleton.o Articulator.o

personviewer.o: personviewer.cpp $(mainHeaders) BvhParser.h Skeleton.h Articulator.h FrameTimer.h

model.o: model.cpp model.h

GlTransformable.o: GlTransformable.cpp GlTransformable.h

Camera.o: Camera.cpp Camera.h

TGALoader.o: TGALoader.cpp TGALoader.h

BvhParser.o: BvhParser.cpp BvhParser.h

Skeleton.o: Skeleton.cpp Skeleton.h

Articulator.o: Articulator.cpp Articulator.h

clean:
	rm -rf *viewer *.o

zip:
	zip sapViewers.zip *.cpp *.h Makefile README.md