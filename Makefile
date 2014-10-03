CC = g++
CXXFLAGS = -I/usr/include -Wno-write-strings
LDLIBS = -lglut -lGLEW -lGL -lGLU -lX11
LDFLAGS = -L/usr/lib -L/usr/lib/nvidia-304-updates

mainOfiles = model.o GlTransformable.o Camera.o MainUtility.o
mainHeaders = model.h Camera.h MainUtility.h


all: modelviewer modelviewerVBO

modelviewer: modelviewer.o $(mainOfiles)

modelviewer.o: modelviewer.cpp $(mainHeaders)

modelviewerVBO: modelviewerVBO.o $(mainOfiles)

modelviewerVBO.o: modelviewerVBO.cpp $(mainHeaders)

modelviewerVAO: modelviewerVAO.o $(mainOfiles)

modelviewerVAO.o: modelviewerVAO.cpp $(mainHeaders)


model.o: model.cpp model.h

GlTransformable.o: GlTransformable.cpp GlTransformable.h

Camera.o: Camera.cpp Camera.h

MainUtility.o: MainUtility.cpp MainUtility.h

clean:
	rm -rf modelviewer modelviewerVBO *.o