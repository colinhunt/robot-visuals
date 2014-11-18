#ifndef SKELETON_H
#define SKELETON_H

#include "GlTransformable.h"

#include <vector>

class Joint : public GlTransformable {
public:
    Joint(): rId(-1), absPosition(Vector3d::Zero()){}
    void draw();

    int id;
    int rId;
    string type;
    string name;
    vector<Joint> children;
    vector<string> channels;
    Vector3d absPosition;
};

class Skeleton {
public:
    Skeleton() : size(0), maxP(-GltUtil::INF, -GltUtil::INF, -GltUtil::INF), minP(GltUtil::INF, GltUtil::INF, GltUtil::INF) {}

    Joint root;
    Vector3d maxP, minP;

    void glDraw();

    int size;
};

#endif // SKELETON_H