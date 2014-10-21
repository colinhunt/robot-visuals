#ifndef BVH_PARSER_H
#define BVH_PARSER_H

#include "Skeleton.h"

#include <limits>


struct Frame {
    Vector3d translation;
    vector<Quaterniond> rotations;
};

struct Motion {
    Motion() : maxP(-GltUtil::INF, -GltUtil::INF, -GltUtil::INF), minP(GltUtil::INF, GltUtil::INF, GltUtil::INF) {}
    int numFrames;
    int rotationsPerFrame;
    double frameTime;
    vector<string> channels;
    vector<Frame> frames;
    vector<Frame> interpolatedFrames;
    Vector3d maxP, minP;
    int fps;
    void interpolate(int fps);

    Frame interpolate(Frame const &frame1, Frame const &frame2, double lambda);
};

struct BvhData {
    Skeleton skeleton;
    Motion motion;
};

void parseBvhFile(char* filename, BvhData &data);

#endif // BVH_PARSER_H