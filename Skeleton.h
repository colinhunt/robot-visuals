#ifndef SKELETON_H
#define SKELETON_H

#include "GlTransformable.h"

#include <vector>

class Joint : public GlTransformable {
public:

    void draw();
    Translation3d getTranslation(double,double,double) const;

    int id;
    string name;
    int positionMap[3];
    int rotationMap[3];
    vector<Joint> children;
};

class Skeleton {
public:
    Joint root;
};

#endif // SKELETON_H