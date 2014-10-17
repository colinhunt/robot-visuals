#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include "Skeleton.h"

struct Frame {
    Translation3d translation;
    vector<Quaterniond> rotations;
};

struct Motion {
    int numFrames;
    int rotationsPerFrame;
    double frameTime;
    vector<Frame> frames;
};

struct BvhData {
    Skeleton skeleton;
    Motion motion;
};

void parseBvhFile(char* filename, BvhData &data);

#endif // BVH_PARSER_H