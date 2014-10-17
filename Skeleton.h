#ifndef SKELETON_H
#define SKELETON_H

#include "GlTransformable.h"

#include <vector>

class Joint : public GlTransformable {
public:

    void draw();

    int id;
    string name;
    vector<Joint> children;
};

class Skeleton {
public:
    Joint root;
};

#endif // SKELETON_H