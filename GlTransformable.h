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
            
    virtual void applyGlTransforms() const;

    virtual void reset();

    Vector3d offset;
    Quaterniond orientation;
};

namespace GltUtil {
    const static double INF = std::numeric_limits<double>::infinity();

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

    inline
    void setMax(Vector3d& maxSoFar, const Vector3d& contender) {
        maxSoFar[0] = max(maxSoFar[0], contender[0]);
        maxSoFar[1] = max(maxSoFar[1], contender[1]);
        maxSoFar[2] = max(maxSoFar[2], contender[2]);
    }

    inline
    void setMin(Vector3d& minSoFar, const Vector3d& contender) {
        minSoFar[0] = min(minSoFar[0], contender[0]);
        minSoFar[1] = min(minSoFar[1], contender[1]);
        minSoFar[2] = min(minSoFar[2], contender[2]);
    }
};

#endif // GL_TRANSFORMABLE_H