/*
 *  modelViewer.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "MainUtility.h"
#include "Skeleton.h"
#include "BvhParser.h"
#include "FrameTimer.h"

#include <iostream>

void drawSkeleton();
void poseJoints(Joint& joint, vector<Quaterniond> const& rotations);
void pose(Skeleton& skeleton, const Frame& frame);
void calculateMovementBox(Vector3d& maxP, Vector3d& minP);
void myKeyInput(unsigned char key, int x, int y);
void animate();
int frameStep();
void drawBox(Vector3d& maxP, Vector3d& minP) ;
void userReset();
void userPrintInteraction();


// Globals
int currFrame = 0;
TimeVal frameTime = 0.008333;
const int ofps = (int) round(1.0 / frameTime);
int fps = ofps;
FrameTimer timer(frameTime);
bool animateOn = false;
Vector3d maxP, minP;
double scale = 1;
bool start = true;

BvhData data;


// Main routine.
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Require argument specifying .bvh file.\n";
        exit(EXIT_FAILURE);
    }

    parseBvhFile(argv[1], data);

    frameTime = data.motion.frameTime;

    data.motion.interpolate(10);

    calculateMovementBox(maxP, minP);

    Vector3d span = maxP - minP;
//    cout << "Span: " << span << endl;

    // calculate scale required to resize movement box to fit inside frustum
    double viewWidth = abs(vb.right) + abs(vb.left);
    double viewHeight = abs(vb.top) + abs(vb.bottom);
    double scale1 = 1, scale2 = 2;
    if (span.x() > viewWidth) {
        scale1 *= viewWidth / span.x();
    }
    if (span.y() > viewHeight) {
        scale2 *= viewHeight / span.y();
    }

    scale = min(scale1, scale2);

    initializeGlutGlewModel(&argc, argv);

    setDrawingFunc(drawSkeleton);
    setUserKeyInputFunc(myKeyInput);
    setUserResetFunc(userReset);
    setPrintUserInteractionFunc(userPrintInteraction);
    glutIdleFunc(animate);
    timer.Start();

    prepareAndStartMainLoop();
    
    return 0;
}

void userPrintInteraction() {
    cout << "Press 'w' to save data to bvh file" << endl;
}

int frameStep() {
    return fps / data.motion.fps;
}

void animate() {
    TimeVal timeLeft = timer.TimeLeft();
    if (animateOn && timeLeft <= 0) {
        TimeVal delta = frameTime - timeLeft;
        int correction = (int) round(delta / frameTime);
//        cout << "correction " << correction << endl;
        glutPostRedisplay();
//        cout << "frameStep: " << frameStep() * correction << endl;
        currFrame += frameStep() * correction;
        timer.Start();
    } else {
//        cout << "no display frame " << timer.TimeLeft() << endl;
    }
}


void userReset() {
    start = true;
    fps = ofps;
    currFrame = 0;
    animateOn = false;
}

void myKeyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
            animateOn = true;
            start = false;
            cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
            break;
        case 'P':
            animateOn = false;
            break;
        case '-':
            fps -= 10;
            cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
            break;
        case '+':
            fps += 10;
            cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
            break;
        case 'w':
            saveAsBvhFile(data);
        default:
            break;
    }
}

int mod(int a, int b) {
    return (a%b+b)%b;
}

void drawSkeleton() {

    myCamera.applyGlTransforms();
    // center of movement box

    // show the movement box
//    drawBox(maxP, minP);

    if (start) {
        glTranslated(0, 0, -(data.skeleton.maxP.z() * scale));
        glScaled(scale, scale, scale);
        Frame f;
        f.rotations = vector<Quaterniond>(data.motion.rotationsPerFrame,
                Quaterniond::Identity());
        pose(data.skeleton, f);
    } else {
        Vector3d v = (maxP + minP) / 2;
        // center minus one end gives half dist in z
        double dist = abs(v.z() - maxP.z());
        // translate back half the movement box z length plus our vb near (scaled)
        glTranslated(0, 0, -(vb.near + (dist * scale)));
        // scale it all down to fit in the frustum
        glScaled(scale, scale, scale);
//    myCamera.scaleUniform(scale);
        // translate to origin for scaling and camera positioning above
        glTranslated(-v.x(), -v.y(), -v.z());

        currFrame = mod(currFrame, data.motion.interpolatedFrames.size());
        pose(data.skeleton, data.motion.interpolatedFrames[currFrame]);
    }
}

void pose(Skeleton& skeleton, const Frame& frame) {
    const Vector3d& translation = frame.translation;
    const vector<Quaterniond>& rotations = frame.rotations;
    glTranslated(translation.x(), translation.y(), translation.z());

    poseJoints(skeleton.root, rotations);
}

void poseJoints(Joint& joint, vector<Quaterniond> const& rotations) {
//    cout << "Posing joint " << joint.name << " " << joint.id << endl;
//    cout << "children: " << joint.children.size() << endl;
    if (joint.id == -1)
        return;

    glPushMatrix();

    joint.orientation = rotations[joint.id];
    joint.applyGlTransforms();
    joint.draw();

    for (int i = 0; i < joint.children.size(); i++) {
        poseJoints(joint.children[i], rotations);
    }

    glPopMatrix();
}

void calculateMovementBox(Vector3d& maxP, Vector3d& minP) {
    maxP = data.motion.maxP + data.skeleton.maxP;
    minP = data.motion.minP + data.skeleton.minP;
}

void drawBox(Vector3d& maxP, Vector3d& minP) {
    glBegin(GL_POLYGON);
    glVertex3d(maxP[0], maxP[1], maxP[2]);
    glVertex3d(maxP[0], minP[1], maxP[2]);
    glVertex3d(minP[0], minP[1], maxP[2]);
    glVertex3d(minP[0], maxP[1], maxP[2]);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3d(maxP[0], maxP[1], minP[2]);
    glVertex3d(maxP[0], minP[1], minP[2]);
    glVertex3d(minP[0], minP[1], minP[2]);
    glVertex3d(minP[0], maxP[1], minP[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(maxP[0], maxP[1], maxP[2]);
    glVertex3d(maxP[0], maxP[1], minP[2]);
    glVertex3d(maxP[0], minP[1], maxP[2]);
    glVertex3d(maxP[0], minP[1], minP[2]);
    glVertex3d(minP[0], minP[1], maxP[2]);
    glVertex3d(minP[0], minP[1], minP[2]);
    glVertex3d(minP[0], maxP[1], maxP[2]);
    glVertex3d(minP[0], maxP[1], minP[2]);
    glEnd();
}
