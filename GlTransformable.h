/*
 *  GlTransformable.h
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#ifndef GL_TRANSLATABLE_H
#define GL_TRANSLATABLE_H

#include <Eigen/Geometry>
#include "openglincludes.h"

using namespace std;
using namespace Eigen;

class GlTransformable {
    
public:
    GlTransformable();
    
    Vector3d const & translateBy(Vector3d offset);

    void rotateByAngleAxis(double degrees, Vector3d axis);
        
    void applyGlTransforms();

    Vector3d translationOffset;
    Quaterniond orientation;
};

#endif // GL_TRANSLATABLE_H