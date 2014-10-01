FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/X11/include -I/opt/local/include -L/opt/X11/lib -L/opt/local/lib -lGLEW -lGLUT -Wno-write-strings

CC = g++
CXXFLAGS = -I/opt/X11/include -I/opt/local/include -Wno-write-strings
LDLIBS = -lGLEW -lGLUT
LDFLAGS = -L/opt/X11/lib -L/opt/local/lib -framework Cocoa -framework OpenGL -framework GLUT

# regular variables used
executable = circle

$(executable): $(executable).o model.o

$(executable).o: $(executable).cpp model.h

model.o: model.cpp model.h