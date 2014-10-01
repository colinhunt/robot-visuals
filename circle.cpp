/////////////////////////////////////////////////////////////////////         
// circle.cpp
//
// This program draws a line loop with vertices equally apart on 
// a fixed circle. The larger the number of vertices the better
// the loop approximates the circle.
//
// Interaction:
// Press +/- to increase/decrease the number of vertices of the loop. 
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <Eigen/Geometry>
#include "model.h"

#ifdef __APPLE__

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>

#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#define PI 3.14159265358979324


using namespace std;
using namespace Eigen;

// Globals.
static bool ortho = false;
static Model* myModel;
GLuint displayListID;

struct Vb {
    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
} vb = {-1.0, 1.0, -1.0, 1.0, 1, 100};

void placeCamera();

void translateAndDraw(double x, double y, double z);

// Drawing routine.
void drawScene(void) {
    placeCamera();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Clear the background of our window to white
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)

    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glColor3f(0.0, 0.0, 0.0);

    Model::Vector offset = myModel->translationOffset;
    glTranslatef((GLfloat) offset.x, (GLfloat) offset.y, (GLfloat) offset.z);
    Model::Vertex center = myModel->calculateCenter();
    cout << "Center is " << center.x << ' ' << center.y << ' ' << center.x << endl;

    Quaterniond o = myModel->orientation;
    cout << "Quaternion: " << o.w() << "," << o.x() << "," << o.y() << "," << o.z() << endl;
    double halfTheta = acos(min(max(o.w(),-1.0),1.0));
    cout << "halfTheta: " << halfTheta << endl;

    double angle = 2 * halfTheta * (180 / PI);
    
    cout << "Angle: " << angle << endl;
    
    glRotatef(angle, o.x(), o.y(), o.z());
    
    cout << "Drawing started" << endl;

    glCallList(myModel->displayList);

    glFlush();
    glutSwapBuffers();

    cout << "Drawing finished" << endl;
}

void placeCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ortho) {
        cout << "displaying ortho" << endl;
        glOrtho(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    } else {
        cout << "displaying persp" << endl;
        glFrustum(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    }
    glMatrixMode(GL_MODELVIEW);
}

void createDisplayList() {
    // create one display list
    GLuint index = glGenLists(1);

    // compile the display list of drawing the model
    glNewList(index, GL_COMPILE);
    myModel->draw(GL_POLYGON);
    glEndList();
    myModel->setDisplayList(index);
}

// Initialization routine.
void setup(void) {
    cout << "Setup called" << endl;

    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    cout << "Resize called" << endl;
    glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    cout << "Keypress: " << key << endl;
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'v':
            ortho = true;
            drawScene();
            break;
        case 'V':
            ortho = false;
            drawScene();
            break;
        case 'w':
            myModel->saveToFile();
            break;
        case 'n':
            translateAndDraw(0, 0, -0.1);
            break;
        case 'N':
            translateAndDraw(0, 0, 0.1);
            break;
        case 'p':
            myModel->rotateX(-10);
            drawScene();
            break;
        case 'P':
            myModel->rotateX(10);
            drawScene();
            break;
        case 'y':
            myModel->rotateY(-10);
            drawScene();
            break;
        case 'Y':
            myModel->rotateY(10);
            drawScene();
            break;
        case 'r':
            myModel->rotateZ(-10);
            drawScene();
            break;
        case 'R':
            myModel->rotateZ(10);
            drawScene();
            break;
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: {
            translateAndDraw(0.0,0.1,0.0);
            break;
        }
        case GLUT_KEY_DOWN: {
            translateAndDraw(0.0,-0.1,0.0);
            break;
        }
        case GLUT_KEY_RIGHT: {
            translateAndDraw(0.1,0.0,0.0);
            break;
        }
        case GLUT_KEY_LEFT: {
            translateAndDraw(-0.1,0.0,0.0);
            break;
        }
        default:
            break;
    }
}

void translateAndDraw(double x, double y, double z) {
    Model::Vector offset(x,y,z);
    myModel->translateBy(offset);
    drawScene();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
    cout << "Interaction:" << endl;
    cout << "Press 'v' for orthographic projection" << endl;
    cout << "Press 'V' for perspective projection" << endl;
    cout << "Press 'w' to save the model to disk" << endl;
}

// Main routine.
int main(int argc, char **argv) {
    myModel = new Model("man.obj");

    Model::Vertex center = myModel->calculateCenter();
    cout << "Center is " << center.x << ' ' << center.y << ' ' << center.z << endl;

    Model::Vector offset = myModel->translationOffset;
    cout << "Offset is " << offset.x << ' ' << offset.y << ' ' << offset.z << endl;

    // move the model to 0,0,-2
    Model::Vector v(0, 0, -2);
    myModel->translateBy(v);

    center = myModel->calculateCenter();
    cout << "Center is " << center.x << ' ' << center.y << ' ' << center.z << endl;

    offset = myModel->translationOffset;
    cout << "Offset is " << offset.x << ' ' << offset.y << ' ' << offset.z << endl;

    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("circle.cpp");

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Enable the vertex array functionality:
    glEnableClientState(GL_VERTEX_ARRAY);

    //pass the vertex pointer:
    glVertexPointer(3,   //3 components per vertex (x,y,z)
            GL_DOUBLE,
            sizeof(Model::Vertex),
            myModel->vertexArray());

    glClearColor(1.0, 1.0, 1.0, 0.0);

    createDisplayList();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

//    glewExperimental = GL_TRUE;
//    glewInit();

//    setup();

    glutMainLoop();
    return 0;
}
