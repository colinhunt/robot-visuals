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

#include <iostream>

void drawSkeleton();
void poseJoints(Joint& joint, vector<Quaterniond> const& rotations);
void pose(Skeleton& skeleton, const Frame& frame);
void calculateMovementBox(Vector3d& maxP, Vector3d& minP);

void drawBox(Vector3d& maxP, Vector3d& minP) ;

// Globals
unsigned currFrame = 0;

BvhData data;

Vector3d maxP, minP;
double scale = 1;

// Main routine.
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Require argument specifying .bvh file.\n";
        exit(EXIT_FAILURE);
    }

    parseBvhFile(argv[1], data);

    calculateMovementBox(maxP, minP);

    Vector3d span = maxP - minP;
    cout << "Span: " << span << endl;

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
    
    prepareAndStartMainLoop();
    
    return 0;
}

void drawSkeleton() {

    myCamera.applyGlTransforms();
    // center of movement box
    Vector3d v = (maxP + minP) / 2;
    // center minus one end gives half dist in z
    double dist = abs(v.z() - maxP.z());
    // translate back half the movement box z length plus our vb near (scaled)
    glTranslated(0, 0, -(vb.near + (dist * scale)));
    // scale it all down to fit in the frustum
    glScaled(scale, scale, scale);
    // translate to origin for scaling and camera positioning above
    glTranslated(-v.x(), -v.y(), -v.z());

    // show the movement box
    drawBox(maxP, minP);

    currFrame = currFrame % data.motion.frames.size();
    pose(data.skeleton, data.motion.frames[currFrame]);
}

void pose(Skeleton& skeleton, const Frame& frame) {
    const Translation3d& translation = frame.translation;
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
