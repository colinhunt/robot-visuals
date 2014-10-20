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
#include "openglincludes.h"
#include "Skeleton.h"

using namespace std;
using namespace Eigen;


GlViewer::GlViewer() {
    frameTime = 0.008333;
    timer = frameTime;
    animateOn = false;
    vb = {-1.0, 1.0, -1.0, 1.0, 1, 100};
}

void GlViewer::setVb(Vb newVb) {
    vb = newVb;
}

// Drawing routine.
void GlViewer::drawScene(void) {
    placeCamera();
    
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Clear the background of our window to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glColor3f(0.0, 0.0, 0.0);

    drawModel();
    
    glutSwapBuffers();
}

void GlViewer::animate() {
    if (animateOn && timer.TimeLeft() <= 0) {
        glutPostRedisplay();
        timer.Start();
        ++currFrame;
    }
}


void GlViewer::placeCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
}


// Initialization routine.
void GlViewer::setup(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void GlViewer::resize(int w, int h) {
    glViewport(0, 0, w, h);
}

void GlViewer::resetAndDraw() {
    myCamera.reset();
    setup();
    drawScene();
}

// Keyboard input processing routine.
void GlViewer::keyInput(unsigned char key, int x, int y) {
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
        case 'p':
            animateOn = true;
            break;
        case 'P':
            animateOn = false;
            break;
        case '-':
            frameTime = 1 / (1 / frameTime - 10);
            cout << "Framerate: " << 1 / frameTime << endl;
            timer.Set(frameTime);
            timer.Start();
            break;
        case '+':
            frameTime = 1 / (1 / frameTime + 10);
            cout << "Framerate: " << 1 / frameTime << endl;
            timer.Set(frameTime);
            timer.Start();
            break;
        default:
            break;
    }
}

void GlViewer::specialKeyInput(int key, int x, int y) {
    switch (key) {
        default:
            break;
    }
}

void GlViewer::translateAndDraw(GlTransformable &obj, double x, double y, double z) {
    obj.translateBy(Vector3d(x,y,z));
    drawScene();
}

void GlViewer::rotateAndDraw(GlTransformable &obj, double angle, Vector3d axis) {
    obj.rotateByAngleAxis(angle, axis);
    drawScene();
}

// Routine to output interaction instructions to the C++ window.
void GlViewer::printInteraction(void) {
    cout << "Interaction:" << endl;
    cout << "Press 'q' to quit" << endl;
    cout << "Press 'w' to save the model to disk" << endl;
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
}

void GlViewer::initializeGlutGlewModel(int* argc, char **argv) {
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

void GlViewer::prepareAndStartMainLoop() {
    
    glClearColor(1.0, 1.0, 1.0, 0.0);
    
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glutIdleFunc(animate);
    setup();

    timer.Start();
    glutMainLoop();    
}