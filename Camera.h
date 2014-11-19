/*
 *  Camera.h
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "GlTransformable.h"
#include "BvhParser.h"

struct Vb {
    Vb() {}

    Vb(double left, double right, double bottom, double top, double near, double far)
            : left(left), right(right), bottom(bottom), top(top), near(near), far(far) {
    }

    double left;
    double right;
    double bottom;
    double top;
    double near;
    double far;
};

class Camera : public GlTransformable {
public:

    void applyGlTransforms() const;
    void applyGlProjection();

    Vb vb;
    Vb iVb;

    bool ortho;
    bool iOrtho;

    GlTransformable::initialize;

    void initialize(Vb viewBox, bool ortho);

    virtual void reset();

    void positionMotion(const BvhData &data);
};

#endif // CAMERA_H