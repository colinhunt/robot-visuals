/*
 *  GlTransformable.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "model.h"
#include <Eigen/Geometry>
#include <iostream>
#include <Python/Python.h>

#include "GlTransformable.h"

using namespace std;

double toRadians(double degrees) {
    return degrees * (M_PI / 180);
}

double toDegrees(double radians) {
    return radians * (180 / M_PI);
}

GlTransformable::GlTransformable() {
    initialize();
}

const Vector3d& GlTransformable::translateBy(const Vector3d& offset) {
    translationOffset += (orientation * offset);
    return translationOffset;
}

void GlTransformable::rotateByAngleAxis(double degrees, const Vector3d& axis) {
    double radians = toRadians(degrees);
    Quaterniond r;
    r = AngleAxisd(radians, axis);
    orientation *= r;
}

void GlTransformable::applyGlTransforms() const {
    GltUtil::applyGlTranslation(translationOffset);
    GltUtil::applyGlRotation(orientation);
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

void GlTransformable::reset() {
    translationOffset = iOffset;
    orientation = iOrientation;
}

void GlTransformable::initialize(Vector3d offset, Quaterniond orientation) {
    this->iOffset = this->translationOffset = offset;
    this->iOrientation = this->orientation = orientation;
}