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

const double PIOVER180 = M_PI / 180;


GlTransformable::GlTransformable() : translationOffset(0,0,0), orientation(1, 0, 0, 0) {

}

const Vector3d& GlTransformable::translateBy(const Vector3d& offset) {
    translationOffset += (orientation * offset);
    return translationOffset;
}

void GlTransformable::rotateByAngleAxis(double degrees, const Vector3d& axis) {
    double radians = degrees * PIOVER180;
    Quaterniond r;
    r = AngleAxisd(radians, axis);
    cout << "rotation: " << r.w() << "," << r.x() << "," << r.y() << "," << r.z() << endl;
    orientation *= r;
    cout << "orientation right after rotation: " << orientation.w() << "," << orientation.x() << "," << orientation.y() << "," << orientation.z() << endl;
}

void GlTransformable::applyGlTransforms() const {
    GltUtil::applyGlTranslation(translationOffset);
    GltUtil::applyGlRotation(orientation);
}

void GlTransformable::reset() {
    orientation.setIdentity();
    translationOffset.setZero();
}


// GltUtil functions:

void GltUtil::applyGlRotation(const Quaterniond& rotation) {
    double halfTheta = acos(min(max(rotation.w(),-1.0),1.0));    
    double angle = 2 * halfTheta * (180 / M_PI);
    
    cout << "Angle: " << angle << endl;
    
    glRotatef(angle, rotation.x(), rotation.y(), rotation.z());        
}

void GltUtil::applyGlTranslation(const Vector3d& translation) {
    glTranslatef((GLfloat) translation.x(), 
                 (GLfloat) translation.y(), 
                 (GLfloat) translation.z());
    cout << translation << endl;
}
