#include "BvhParser.h"
#include "MainUtility.h"

#ifndef MOTION_VIEWER_H
#define MOTION_VIEWER_H

class MotionViewer : public GlViewer {
public:
    MotionViewer();
    static void drawModel();

    void poseJoints(Joint &joint, vector <Quaterniond> const &rotations);

    void pose(Skeleton &skeleton, const Frame &frame);

    void calculateMovementBox(Vector3d &maxP, Vector3d &minP);

    void drawBox(Vector3d &maxP, Vector3d &minP);

    static BvhData data;

    static Vector3d maxP, minP;
    static double scale;
};

#endif // MOTION_VIEWER_H