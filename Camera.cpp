/*
 *  Camera.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "Camera.h"

void Camera::applyGlTransforms() const {
    GltUtil::applyGlRotation(orientation.inverse());
    GltUtil::applyGlTranslation(-translationOffset);
}
