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
void drawSkeleton2();
void poseJoints(Joint& joint, vector<Quaterniond> const& rotations);
void pose(Skeleton& skeleton, const Frame& frame);
void calculateMovementBox(Vector3d& maxP, Vector3d& minP);

BvhData data;

Vector3d maxP, minP;


// Main routine.
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Require argument specifying .bvh file.\n";
        exit(EXIT_FAILURE);
    }

    parseBvhFile(argv[1], data);

    calculateMovementBox(maxP, minP);

    // center of movement box
    Vector3d v = (maxP + minP) / 2;
    // center of closest side plus offset
    v[2] = maxP.z() + 20;

    // move the camera to the front of the box
    myCamera->translateBy(v);
    myCamera->translateBy(Vector3d(0,0,0));

    // calculate the dimensions of the box from the origin
    Vector3d p, q;
    p = maxP - v;
    q = minP - v;

    cout << "p " << p << endl;
    cout << "q " << q << endl;

    Vb vb = {-1.0, 1.0, -1.0, 1.0, 1, 100};

//    vb.right = p.x();
//    vb.top = p.y();

//    vb.left = q.x();
//    vb.bottom = q.y();
    // set the far plane to contain the box plus offset
    vb.far = -q.z() + 5;

    setVb(vb);

    initializeGlutGlewModel(&argc, argv);
    
    setDrawingFunc(drawSkeleton2);
    
    prepareAndStartMainLoop();
    
    return 0;
}

void pose(Skeleton& skeleton, const Frame& frame) {
    const Translation3d& translation = frame.translation;
    const vector<Quaterniond>& rotations = frame.rotations;
    glTranslated(translation.x(), translation.y(), translation.z());

    poseJoints(skeleton.root, rotations);
}

void poseJoints(Joint& joint, vector<Quaterniond> const& rotations) {
    cout << "Posing joint " << joint.name << " " << joint.id << endl;
    cout << "children: " << joint.children.size() << endl;
    if (joint.id == -1)
        return;

    glPushMatrix();

    joint.orientation = rotations[joint.id];
    joint.applyGlTransforms();
    joint.draw();

//    if (joint.id == 0) {
//        glBegin(GL_POLYGON);
//        glVertex3d(data.skeleton.maxP[0], data.skeleton.maxP[1], data.skeleton.maxP[2]);
//        glVertex3d(data.skeleton.maxP[0], data.skeleton.minP[1], data.skeleton.maxP[2]);
//        glVertex3d(data.skeleton.minP[0], data.skeleton.minP[1], data.skeleton.maxP[2]);
//        glVertex3d(data.skeleton.minP[0], data.skeleton.maxP[1], data.skeleton.maxP[2]);
//        glEnd();
//    }

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
void drawSkeleton2() {
    static unsigned currFrame = 0;
//    Frame f;
//    f.translation = Translation3d(0, 0, -25);
//    for (int i = 0; i < 100; ++i) {
//        f.rotations.push_back(Quaterniond(1,0,0,0));
//    }
//    glTranslated(0, -20, -65);
//    glRotated(90, 0, 1, 0);
//    Vector3d maxP, minP;
//    calculateMovementBox(maxP, minP);

    myCamera->applyGlTransforms();

    drawBox(maxP, minP);

    cout << "Skel box: \n" << data.skeleton.maxP << endl << data.skeleton.minP << endl;
    cout << "Motion box: \n" << data.motion.maxP << endl << data.motion.minP << endl;


//    Vector3d maxP, minP;
//    calculateMovementBox(maxP, minP);


//    drawBox(maxP, minP);
//    drawBox(data.motion.maxP, data.motion.minP);
//
//    cout << "Movement box: \n" << maxP << minP;
    pose(data.skeleton, data.motion.frames[currFrame++]);
//    pose(data.skeleton, data.motion.frames[0]);

    currFrame = currFrame % data.motion.frames.size();
}


void drawSkeleton() {
    Skeleton skeleton;

    Frame f;
    f.translation = Translation3d(0, 0, -10);

    Quaterniond q;
    q = AngleAxisd(-4.5, Vector3d::UnitZ())
            * AngleAxisd(1.4, Vector3d::UnitY())
            * AngleAxisd(2.36, Vector3d::UnitX());

    f.rotations.push_back(q);

    q = AngleAxisd(-18.1, Vector3d::UnitZ())
            * AngleAxisd(-11, Vector3d::UnitY())
            * AngleAxisd(-25.4, Vector3d::UnitX());

    f.rotations.push_back(q);

    q = AngleAxisd(4, Vector3d::UnitZ())
            * AngleAxisd(12.3, Vector3d::UnitY())
            * AngleAxisd(36.7, Vector3d::UnitX());

    f.rotations.push_back(q);

    Joint root;
    root.id = 0;
    root.name = "Hips";
    root.offset = Vector3d(0,0,0);

    Joint one;
    one.id = 1;
    one.name = "LHipJoint";
    one.offset = Vector3d(2, 2, 2);

    Joint two;
    two.id = 2;
    two.name = "LeftUpLeg";
    two.offset = Vector3d(1.6, -1.8, 0.6);

    Joint three;
    three.id = -1;
    three.name = "End Site";
    three.offset = Vector3d(2.5,-7,0);

    two.children.push_back(three);
    one.children.push_back(two);
    root.children.push_back(one);

    skeleton.root = root;

    pose(skeleton, f);
}
