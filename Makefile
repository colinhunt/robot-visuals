FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/X11/include -I/opt/local/include -L/opt/X11/lib -L/opt/local/lib -lGLEW -lGLUT -Wno-write-strings

CC = g++
CXXFLAGS = -I/opt/X11/include -I/opt/local/include -Wno-write-strings
LDLIBS = -lGLEW -lGLUT
LDFLAGS = -L/opt/X11/lib -L/opt/local/lib -framework Cocoa -framework OpenGL -framework GLUT

mainOfiles = model.o GlTransformable.o Camera.o
mainHeaders = model.h Camera.h


all: modelviewer modelviewerVBO

modelviewer: modelviewer.o $(mainOfiles)

modelviewer.o: modelviewer.cpp $(mainHeaders)

modelviewerVBO: modelviewerVBO.o $(mainOfiles)

modelviewerVBO.o: modelviewerVBO.cpp $(mainHeaders)

model.o: model.cpp model.h

GlTransformable.o: GlTransformable.cpp GlTransformable.h

Camera.o: Camera.cpp Camera.h

clean:
	rm -rf modelviewer modelviewerVBO *.o