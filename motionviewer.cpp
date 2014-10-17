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


BvhData data;


// Main routine.
int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Require argument specifying .bvh file.\n";
        exit(EXIT_FAILURE);
    }

    parseBvhFile(argv[1], data);

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

    for (int i = 0; i < joint.children.size(); i++) {
        poseJoints(joint.children[i], rotations);
    }

    glPopMatrix();
}

void drawSkeleton2() {
    static unsigned currFrame = 0;
//    Frame f;
//    f.translation = Translation3d(0, 0, -25);
//    for (int i = 0; i < 100; ++i) {
//        f.rotations.push_back(Quaterniond(1,0,0,0));
//    }
    glTranslated(0, -17, 0);

    pose(data.skeleton, data.motion.frames[currFrame++]);

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
