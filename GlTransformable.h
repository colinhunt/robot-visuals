/*
 *  GlTransformable.h
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#ifndef GL_TRANSFORMABLE_H
#define GL_TRANSFORMABLE_H

#include <Eigen/Geometry>
#include "openglincludes.h"

using namespace std;
using namespace Eigen;

class GlTransformable {
    
public:
    GlTransformable();
    
    Vector3d const & translateBy(const Vector3d& offset);

    void rotateByAngleAxis(double degrees, const Vector3d& axis);
            
    void applyGlTransforms() const;

    void reset();

    Vector3d offset;
    Quaterniond orientation;
};

namespace GltUtil {
    void applyGlRotation(const Quaterniond& rotation);
    void applyGlTranslation(const Vector3d& translation);

    inline
    double toRadians(double degrees) {
        return degrees * (M_PI / 180);
    }

    inline
    double toDegrees(double radians) {
        return radians * (180 / M_PI);
    }
};

#endif // GL_TRANSFORMABLE_H