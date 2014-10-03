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

void drawScene(void);

void drawSceneVBO();

void resize(int w, int h);

void keyInput(unsigned char key, int x, int y);

void specialKeyInput(int key, int x, int y);

void prepareAndStartMainLoop();
    
void initializeGlutGlewModel(int* argc, char **argv);

void loadDataIntoVBO();

#endif // MAIN_UTILITY_H