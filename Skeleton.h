#ifndef SKELETON_H
#define SKELETON_H

#include "GlTransformable.h"

#include <vector>

class Joint : public GlTransformable {
public:
    Joint(): absPosition(Vector3d::Zero()){}
    void draw();

    int id;
    string type;
    string name;
    vector<Joint> children;
    vector<string> channels;
    Vector3d absPosition;
};

class Skeleton {
public:
    Skeleton() : maxP(-GltUtil::INF, -GltUtil::INF, -GltUtil::INF), minP(GltUtil::INF, GltUtil::INF, GltUtil::INF) {}

    Joint root;
    Vector3d maxP, minP;
};

#endif // SKELETON_H