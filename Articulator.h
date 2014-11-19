#include "Skeleton.h"
#include "BvhParser.h"
#include "model.h"
#include "FrameTimer.h"

#ifndef ARTICULATOR_H
#define ARTICULATOR_H

class Articulator {
public:
    Articulator();

    void pose(Skeleton& skeleton, const Frame& frame);

    void poseJoints(Joint &joint, Quaterniond currRotation, vector<Quaterniond> const &rotations, Vector3d currOffset);

    void glDrawAttachments();

    void attach(BvhData*, Model*);

    BvhData* bvhData;
    Model* mesh;

    int hlBone;

    int currFrame;
    TimeVal frameTime;
    int ofps;
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

    int frameStep();

    void startAnimation();

    void stopAnimation();

    void decreaseFps();

    void increaseFps();

    void saveAttToFile();
};

#endif // ARTICULATOR_H