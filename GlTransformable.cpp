/*
 *  GlTransformable.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "GlTransformable.h"

const double PIOVER180 = M_PI / 180;


GlTransformable::GlTransformable() : translationOffset(0,0,0), orientation(1, 0, 0, 0) {

}

const Vector3d& GlTransformable::translateBy(Vector3d offset) {
    offset = orientation * offset;
    translationOffset += offset;
    return translationOffset;
}

void GlTransformable::rotateByAngleAxis(double degrees, Vector3d axis) {
    double radians = degrees * PIOVER180;
    Quaterniond r;
    r = AngleAxisd(radians, axis);
    cout << "rotation: " << r.w() << "," << r.x() << "," << r.y() << "," << r.z() << endl;
    orientation *= r;
    cout << "orientation right after rotation: " << orientation.w() << "," << orientation.x() << "," << orientation.y() << "," << orientation.z() << endl;
}

void GlTransformable::applyGlTransforms() {
    glTranslatef((GLfloat) translationOffset.x(), 
                 (GLfloat) translationOffset.y(), 
                 (GLfloat) translationOffset.z());
    cout << translationOffset << endl;
    
    Quaterniond o = orientation;
    cout << "Quaternion: " << o.w() << "," << o.x() << "," << o.y() << "," << o.z() << endl;
    double halfTheta = acos(min(max(o.w(),-1.0),1.0));
    cout << "halfTheta: " << halfTheta << endl;
    
    double angle = 2 * halfTheta * (180 / M_PI);
    
    cout << "Angle: " << angle << endl;
    
    glRotatef(angle, o.x(), o.y(), o.z());    
}
