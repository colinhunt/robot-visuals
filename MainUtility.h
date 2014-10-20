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
#include "FrameTimer.h"

class GlViewer {
public:
    static struct Vb {
        double left;
        double right;
        double bottom;
        double top;
        double near;
        double far;
    } vb;

    static Camera myCamera;
    static unsigned currFrame;
    static TimeVal frameTime;
    static FrameTimer timer;
    static bool animateOn;

    GlViewer();
    static void resize(int w, int h);

    static void keyInput(unsigned char key, int x, int y);

    static void specialKeyInput(int key, int x, int y);

    void prepareAndStartMainLoop();

    void initializeGlutGlewModel(int* argc, char** argv);

    static void resetAndDraw();

    virtual static void drawModel() = 0;
    static void drawScene(void);
    void placeCamera();

    void animate();
    void setup(void);

    void setVb(Vb);

    static void translateAndDraw(GlTransformable &obj, double x, double y, double z);
    static void rotateAndDraw(GlTransformable &obj, double angle, Vector3d axis);
    void printInteraction(void);

};
#endif // MAIN_UTILITY_H