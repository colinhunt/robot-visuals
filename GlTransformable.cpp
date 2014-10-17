/*
 *  GlTransformable.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include <iostream>

#include "GlTransformable.h"

using namespace std;
using namespace GltUtil;

GlTransformable::GlTransformable() {
    reset();
}

const Vector3d& GlTransformable::translateBy(const Vector3d& offset) {
    this->offset += (orientation * offset);
    return offset;
}

void GlTransformable::rotateByAngleAxis(double degrees, const Vector3d& axis) {
    double radians = toRadians(degrees);
    Quaterniond r;
    r = AngleAxisd(radians, axis);
    orientation *= r;
}

void GlTransformable::applyGlTransforms() const {
    GltUtil::applyGlTranslation(offset);
    GltUtil::applyGlRotation(orientation);
}

void GlTransformable::reset() {
    orientation.setIdentity();
    offset.setZero();
}


// GltUtil functions:

void GltUtil::applyGlRotation(const Quaterniond& rotation) {
    double halfTheta = acos(min(max(rotation.w(),-1.0),1.0));    
    double angle = toDegrees(2 * halfTheta);
    
    glRotated(angle, rotation.x(), rotation.y(), rotation.z());
}

void GltUtil::applyGlTranslation(const Vector3d& translation) {
    glTranslated(translation.x(),
                 translation.y(),
                 translation.z());
}
