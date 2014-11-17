#include "Skeleton.h"
#include "BvhParser.h"
#include "model.h"

#ifndef ARTICULATOR_H
#define ARTICULATOR_H

class Articulator {
public:
    Articulator() : hlBone(-1) {}

    void pose(Skeleton& skeleton, const Frame& frame);

    void poseJoints(Joint& joint, vector<Quaterniond> const& rotations);

    void glDrawAttachments();

    void attach(BvhData*, Model*);

    BvhData* bvhData;
    Model* mesh;

    void highlightNextBone();

    void highlightPrevBone();

    int hlBone;

    void reset();

    void initAttachments(char* attFileName);
};

#endif // ARTICULATOR_H