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

    Vector3d iOffset;
    Quaterniond iOrientation;
public:
    GlTransformable();
    
    Vector3d const & translateBy(const Vector3d& offset);

    void rotateByAngleAxis(double degrees, const Vector3d& axis);
            
    void applyGlTransforms() const;

    virtual void reset();

    Vector3d translationOffset;
    Quaterniond orientation;

    void initialize(Vector3d offset = Vector3d::Zero(),
            Quaterniond orientation = Quaterniond::Identity());
};

namespace GltUtil {
    void applyGlRotation(const Quaterniond& rotation);
    void applyGlTranslation(const Vector3d& translation);
};

#endif // GL_TRANSLATABLE_H