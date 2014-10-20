/*
 *  MainUtility.h
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-03.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#ifndef MAIN_UTILITY_H
#define MAIN_UTILITY_H

#include "Camera.h"

static struct Vb {
    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
} vb = {-1.0, 1.0, -1.0, 1.0, 1, 100};

// Globals.
extern Camera myCamera;
extern unsigned currFrame;

void resize(int w, int h);

void keyInput(unsigned char key, int x, int y);

void specialKeyInput(int key, int x, int y);

void prepareAndStartMainLoop();
    
void initializeGlutGlewModel(int* argc, char **argv);

void loadDataIntoVAO();

void loadDataIntoVBO();

void loadDataIntoVertexPointer();
    
void loadDataIntoVertexAttribPointer();
    
void loadDataIntoVertexArray();
    
void drawModelVBO();

void drawModelDisplayList();

void drawSkeleton();

void setDrawingFunc(void (*func)(void));

void setVb(Vb);

#endif // MAIN_UTILITY_H