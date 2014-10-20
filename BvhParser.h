#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include "Skeleton.h"

#include <limits>


struct Frame {
    Translation3d translation;
    vector<Quaterniond> rotations;
};

struct Motion {
    Motion() : maxP(-GltUtil::INF, -GltUtil::INF, -GltUtil::INF), minP(GltUtil::INF, GltUtil::INF, GltUtil::INF) {}
    int numFrames;
    int rotationsPerFrame;
    double frameTime;
    vector<string> channels;
    vector<Frame> frames;
    Vector3d maxP, minP;
};

struct BvhData {
    Skeleton skeleton;
    Motion motion;
};

void parseBvhFile(char* filename, BvhData &data);

#endif // BVH_PARSER_H