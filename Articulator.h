#include "Skeleton.h"
#include "BvhParser.h"
#include "model.h"
#include "FrameTimer.h"

#ifndef ARTICULATOR_H
#define ARTICULATOR_H

class Articulator {
public:
    Articulator() :
            hlBone(-1),
            frameTime(0.008333),
            timer(frameTime),
            ofps((int) round(1.0 / frameTime))
    {
        fps = ofps;
        animateOn = false;
    }

    void pose(Skeleton& skeleton, const Frame& frame);

    void poseJoints(Joint& joint, vector<Quaterniond> const& rotations);

    void glDrawAttachments();

    void attach(BvhData*, Model*);

    BvhData* bvhData;
    Model* mesh;

    int hlBone;

    int currFrame;
    TimeVal frameTime;
    const int ofps;
    int fps;
    FrameTimer timer;
    bool animateOn;

    vector<vector<pair<int,double> > > attachments;

    void highlightNextBone();

    void highlightPrevBone();

    void reset();

    void initAttachments(char* attFileName);

    void drawNextFrame();

    void animate();
};

#endif // ARTICULATOR_H