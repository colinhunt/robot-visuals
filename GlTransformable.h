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
    
    Vector3d const & translateBy(const Vector3d& offset);

    void rotateByAngleAxis(double degrees, const Vector3d& axis);
            
    void applyGlTransforms() const;

    Vector3d translationOffset;
    Quaterniond orientation;
};

namespace GltUtil {
    void applyGlRotation(const Quaterniond& rotation);
    void applyGlTranslation(const Vector3d& translation);
};

#endif // GL_TRANSLATABLE_H