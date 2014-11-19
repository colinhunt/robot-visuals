/*
 *  modelViewer.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "openglincludes.h"
#include "Camera.h"
#include "model.h"

enum shadeType {OFF, FLAT, SMOOTH, TEXTURE, NUM_SHADE_TYPES};

void setup(char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);
void printInteraction(void);

// Globals.
Model myModel;
Camera myCamera;
unsigned shading = OFF;

// Main routine.
int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glewExperimental = GL_TRUE;
    glewInit();
    setup(argv[1]);
    printInteraction();
    glutMainLoop();
    return 0;
}

void setup(char* fileName)
{
    myModel.initFromObjFile(fileName);
    myModel.initialize(Vector3d(0, 0, -10));
    myModel.glEnableVertexArray();
    myCamera.initialize(Vb(-1.0, 1.0, -1.0, 1.0, 1, 100), false);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    float Ambient[4]={0.5f,0.5f,0.5f,1.0f};
    glLightfv(GL_LIGHT0,GL_AMBIENT,Ambient);
}

void drawScene(void)
{
    glEnable(GL_LIGHT0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    myCamera.applyGlProjection(); // camera view volume

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    myCamera.applyGlTransforms(); // camera transformation

    float LightPos[4]={-5.0f,5.0f,10.0f,1.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,LightPos);

    myModel.applyGlTransforms(); // object transformation

    // draw model
    if (shading == OFF) {
        glDisable(GL_LIGHTING);
        myModel.glDisableTextures();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (shading == FLAT) {
        glShadeModel(GL_FLAT);
    } else if (shading == SMOOTH) {
        glShadeModel(GL_SMOOTH);
    } else if (shading == TEXTURE) {
        myModel.glEnableTextures();
    }

    myModel.glColor();
    myModel.glDrawVertexArray();
    glutSwapBuffers();
}


void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
        case 'x': {
            myModel.reset();
            myCamera.reset();
            shading = OFF;
        }
            break;
        case 'w':
            myModel.saveToFile();
            break;
        case 'n': {
            myModel.translateBy(Vector3d(0, 0, -0.1));
        }
            break;
        case 'N': {
            myModel.translateBy(Vector3d(0, 0, 0.1));
        }
            break;
        case 'p':
            myModel.rotateByAngleAxis(-10, Vector3d::UnitX());
            break;
        case 'P':
            myModel.rotateByAngleAxis(10, Vector3d::UnitX());
            break;
        case 'y':
            myModel.rotateByAngleAxis(-10, Vector3d::UnitY());
            break;
        case 'Y':
            myModel.rotateByAngleAxis(10, Vector3d::UnitY());
            break;
        case 'r':
            myModel.rotateByAngleAxis(-10, Vector3d::UnitZ());
            break;
        case 'R':
            myModel.rotateByAngleAxis(10, Vector3d::UnitZ());
            break;
        case 'd':
            myCamera.translateBy(Vector3d(-0.1, 0.0, 0.0));
            break;
        case 'D':
            myCamera.translateBy(Vector3d(0.1, 0.0, 0.0));
            break;
        case 'c':
            myCamera.translateBy(Vector3d(0.0, -0.1, 0.0));
            break;
        case 'C':
            myCamera.translateBy(Vector3d(0.0,0.1,0.0));
            break;
        case 'i':
            myCamera.translateBy(Vector3d(0.0,0.0,-0.1));
            break;
        case 'I':
            myCamera.translateBy(Vector3d(0.0,0.0,0.1));
            break;
        case 't':
            myCamera.rotateByAngleAxis(-10, Vector3d::UnitX());
            break;
        case 'T':
            myCamera.rotateByAngleAxis(10, Vector3d::UnitX());
            break;
        case 'a':
            myCamera.rotateByAngleAxis(-10, Vector3d::UnitY());
            break;
        case 'A':
            myCamera.rotateByAngleAxis(10, Vector3d::UnitY());
            break;
        case 'l':
            myCamera.rotateByAngleAxis(-10, Vector3d::UnitZ());
            break;
        case 'L':
            myCamera.rotateByAngleAxis(10, Vector3d::UnitZ());
            break;
        case 's':
            shading = (shading + 1) % NUM_SHADE_TYPES;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            myModel.translateBy(Vector3d(0.0, 0.1, 0.0));
            break;
        case GLUT_KEY_DOWN:
            myModel.translateBy(Vector3d(0.0, -0.1, 0.0));
            break;
        case GLUT_KEY_RIGHT:
            myModel.translateBy(Vector3d(0.1, 0.0, 0.0));
            break;
        case GLUT_KEY_LEFT:
            myModel.translateBy(Vector3d(-0.1, 0.0, 0.0));
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
}
