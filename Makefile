FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/X11/include -I/opt/local/include -L/opt/X11/lib -L/opt/local/lib -lGLEW -lGLUT -Wno-write-strings

CC = g++
CXXFLAGS = -I/opt/X11/include -I/opt/local/include -Wno-write-strings
LDLIBS = -lGLEW -lGLUT
LDFLAGS = -L/opt/X11/lib -L/opt/local/lib -framework Cocoa -framework OpenGL -framework GLUT

modelviewer: modelviewer.o model.o GlTransformable.o Camera.o

modelviewer.o: modelviewer.cpp model.h Camera.h

modelviewerVBO: modelviewerVBO.o model.o GlTransformable.o Camera.o

modelviewerVBO.o: modelviewerVBO.cpp model.h Camera.h

model.o: model.cpp model.h

GlTransformable.o: GlTransformable.cpp GlTransformable.h

Camera.o: Camera.cpp Camera.h

clean:
	rm -rf modelviewer modelviewerVBO *.o