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

extern struct Vb {
    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
} vb;

// Globals.
extern Camera myCamera;

void resize(int w, int h);

void prepareAndStartMainLoop();
    
void initializeGlutGlewModel(int* argc, char **argv);

void setDrawingFunc(void (*func)(void));

void setUserKeyInputFunc(void (*userKeyInputFunc)(unsigned char key, int x, int y));

void setPrintUserInteractionFunc(void (*printUserInteractionFunc)(void));

void setUserResetFunc(void (* userResetFunc)(void));

void setVb(Vb);

#endif // MAIN_UTILITY_H