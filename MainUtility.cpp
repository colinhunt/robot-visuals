/*
 *  modelViewer.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "MainUtility.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <Eigen/Geometry>

#include "GlTransformable.h"
#include "model.h"
#include "Camera.h"
#include "openglincludes.h"

using namespace std;
using namespace Eigen;

// Globals.
static bool ortho = true;
static bool showFog = false;
static Model* myModel;
static Camera* myCamera = new Camera();
GLuint displayListID;

struct Vb {
    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
} vb = {-1.0, 1.0, -1.0, 1.0, 1, 100};

void (*drawModel)(void);

void placeCamera();
void translateAndDraw(GlTransformable* obj, double x, double y, double z);
void rotateAndDraw(GlTransformable* obj, double angle, Vector3d axis);

// Drawing routine.
void drawScene(void) {
    placeCamera();
    
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Clear the background of our window to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    
    if (showFog) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FOG);
        float FogCol[3]={1,1,1};
        glFogfv(GL_FOG_COLOR,FogCol);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 1.f);
        glFogf(GL_FOG_END, 5.f);        
    } else {
        glDisable(GL_FOG);
        glDisable(GL_DEPTH_TEST);
    }

    glColor3f(0.0, 0.0, 0.0);
    
    myCamera->applyGlTransforms();
    
    myModel->applyGlTransforms();
    
    drawModel();
    
    glutSwapBuffers();
}


void drawModelDisplayList() {
    glCallList(myModel->displayList);
}
void drawModelVBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myModel->ibo);
    glDrawElements(GL_TRIANGLES, (GLsizei) myModel->facesFlattened.size(), GL_UNSIGNED_INT, (void*)0);            
}

void setDrawingFunc(void (*drawFunc)(void)) {
    drawModel = drawFunc;
}

void placeCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
        
    if (ortho) {
        glOrtho(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    } else {
        glFrustum(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    }
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
    // move the model to 0,0,-2
    myModel->translateBy(Vector3d(0, 0, -2));

    glClearColor(1.0, 1.0, 1.0, 0.0);
    
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    glViewport(0, 0, w, h);
}

void resetAndDraw() {
    myModel->reset();
    myCamera->reset();
    setup();
    drawScene();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'q':
            exit(0);
            break;
        case 'x':
            resetAndDraw();
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
            translateAndDraw(myModel, 0, 0, -0.1);
            break;
        case 'N':
            translateAndDraw(myModel, 0, 0, 0.1);
            break;
        case 'p':
            rotateAndDraw(myModel, -10, Vector3d::UnitX());
            break;
        case 'P':
            rotateAndDraw(myModel, 10, Vector3d::UnitX());
            break;
        case 'y':
            rotateAndDraw(myModel, -10, Vector3d::UnitY());
            break;
        case 'Y':
            rotateAndDraw(myModel, 10, Vector3d::UnitY());
            break;
        case 'r':
            rotateAndDraw(myModel, -10, Vector3d::UnitZ());
            break;
        case 'R':
            rotateAndDraw(myModel, 10, Vector3d::UnitZ());
            break;
        case 'd':
            translateAndDraw(myCamera, -0.1,0.0,0.0);
            break;
        case 'D':
            translateAndDraw(myCamera, 0.1,0.0,0.0);
            break;
        case 'c':
            translateAndDraw(myCamera, 0.0,-0.1,0.0);
            break;
        case 'C':
            translateAndDraw(myCamera, 0.0,0.1,0.0);
            break;
        case 'i':
            translateAndDraw(myCamera, 0.0,0.0,-0.1);
            break;
        case 'I':
            translateAndDraw(myCamera, 0.0,0.0,0.1);
            break;
        case 't':
            rotateAndDraw(myCamera, -10, Vector3d::UnitX());
            break;
        case 'T':
            rotateAndDraw(myCamera, 10, Vector3d::UnitX());
            break;
        case 'a':
            rotateAndDraw(myCamera, -10, Vector3d::UnitY());
            break;
        case 'A':
            rotateAndDraw(myCamera, 10, Vector3d::UnitY());
            break;
        case 'l':
            rotateAndDraw(myCamera, -10, Vector3d::UnitZ());
            break;
        case 'L':
            rotateAndDraw(myCamera, 10, Vector3d::UnitZ());
            break;
        case 'f':
            showFog = false;
            drawScene();
            break;
        case 'F':
            showFog = true;
            drawScene();
            break;
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: {
            translateAndDraw(myModel, 0.0,0.1,0.0);
            break;
        }
        case GLUT_KEY_DOWN: {
            translateAndDraw(myModel, 0.0,-0.1,0.0);
            break;
        }
        case GLUT_KEY_RIGHT: {
            translateAndDraw(myModel, 0.1,0.0,0.0);
            break;
        }
        case GLUT_KEY_LEFT: {
            translateAndDraw(myModel, -0.1,0.0,0.0);
            break;
        }
        default:
            break;
    }
}

void translateAndDraw(GlTransformable* obj, double x, double y, double z) {
    obj->translateBy(Vector3d(x,y,z));
    drawScene();
}

void rotateAndDraw(GlTransformable* obj, double angle, Vector3d axis) {
    obj->rotateByAngleAxis(angle, axis);
    drawScene();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void) {
    cout << "Interaction:" << endl;
    cout << "Press 'q' to quit" << endl;
    cout << "Press 'w' to save the model to disk" << endl;
    cout << "Press 'v' for orthographic projection" << endl;
    cout << "Press 'V' for perspective projection" << endl;
    cout << "Press 'up arrow' to translate the model up" << endl;
    cout << "Press 'down arrow' to translate the model down" << endl;
    cout << "Press 'left arrow' to translate the model left" << endl;
    cout << "Press 'right arrow' to translate the model right" << endl;
    cout << "Press 'n' to translate the model -0.1 z" << endl;
    cout << "Press 'N' to translate the model 0.1 z" << endl;
    cout << "Press 'p' to pitch the model by -10 degrees" << endl;
    cout << "Press 'P' to pitch the model by 10 degrees" << endl;
    cout << "Press 'y' to yaw the model by -10 degrees" << endl;
    cout << "Press 'Y' to yaw the model by 10 degrees" << endl;
    cout << "Press 'r' to roll the model by -10 degrees" << endl;
    cout << "Press 'R' to roll the model by 10 degrees" << endl;
    cout << "Press 'd' to translate the camera -0.1 x" << endl;
    cout << "Press 'D' to translate the camera 0.1 x" << endl;
    cout << "Press 'c' to translate the camera -0.1 y" << endl;
    cout << "Press 'C' to translate the camera 0.1 y" << endl;
    cout << "Press 'i' to translate the camera -0.1 z" << endl;
    cout << "Press 'I' to translate the camera 0.1 z" << endl;
    cout << "Press 't' to tilt the camera by -10 degrees" << endl;
    cout << "Press 'T' to tilt the camera by 10 degrees" << endl;
    cout << "Press 'a' to pan the camera by -10 degrees" << endl;
    cout << "Press 'A' to pan the camera by 10 degrees" << endl;
    cout << "Press 'l' to roll the camera by -10 degrees" << endl;
    cout << "Press 'L' to roll the camera by 10 degrees" << endl;
    cout << "Press 'x' to reset model and camera position" << endl;
    cout << "Press 'f' to turn off fog" << endl;
    cout << "Press 'F' to turn on fog" << endl;
}

void loadDataIntoVAO() {
#ifdef __APPLE__
    glGenVertexArraysAPPLE(1, &myModel->vao);
    glBindVertexArrayAPPLE(myModel->vao);
#else
    glGenVertexArrays(1, &myModel->vao);
    glBindVertexArray(myModel->vao);
#endif
    glEnableVertexAttribArray(0);
    loadDataIntoVBO();
}

void loadDataIntoVBO() {
    
    const Model::Vertex* data = myModel->vertexArray();
    glGenBuffers(1, &myModel->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, myModel->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Model::Vertex) * myModel->vertices.size(), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, myModel->vbo);

    
    int* indices = &myModel->facesFlattened[0];
    glGenBuffers(1, &myModel->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myModel->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * myModel->facesFlattened.size(), indices, GL_STATIC_DRAW);
        
}

void loadDataIntoVertexPointer() {
    // pass the vertex pointer:
    glVertexPointer(3,   //3 components per vertex (x,y,z)
                    GL_DOUBLE,
                    sizeof(Model::Vertex),
                    0);
}

void loadDataIntoVertexAttribPointer() {
    glVertexAttribPointer(0,
                          3,   //3 components per vertex (x,y,z)
                          GL_DOUBLE,
                          GL_FALSE,
                          sizeof(Model::Vertex),
                          0);    
}

void loadDataIntoVertexArray() {
    //pass the vertex pointer:
    glVertexPointer(3,   //3 components per vertex (x,y,z)
                    GL_DOUBLE,
                    sizeof(Model::Vertex),
                    myModel->vertexArray());
    
    createDisplayList();
}

void initializeGlutGlewModel(int* argc, char **argv) {
    if (*argc < 2) {
        cerr << "Require argument specifying .obj file.\n";
        exit(EXIT_FAILURE);
    }

    myModel = new Model(argv[1]);
    
    printInteraction();
    glutInit(argc, argv);
    
    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("modelviewer.cpp");
    
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    
    //Enable the vertex array functionality:
    glEnableClientState(GL_VERTEX_ARRAY);
}

void prepareAndStartMainLoop() {
    
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    
    setup();
    
    glutMainLoop();    
}
