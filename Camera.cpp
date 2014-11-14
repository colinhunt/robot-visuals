/*
 *  Camera.cpp
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#include "Camera.h"

void Camera::reset() {
    GlTransformable::reset();
    vb = iVb;
    ortho = iOrtho;
}

void Camera::applyGlTransforms() const {
    GltUtil::applyGlRotation(orientation.inverse());
    GltUtil::applyGlTranslation(-translationOffset);
}

void Camera::applyGlProjection() {
    if (ortho) {
        glOrtho(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    } else {
        glFrustum(vb.left, vb.right, vb.bottom, vb.top, vb.near, vb.far);
    }
}

void Camera::initialize(Vb viewBox, bool ortho) {
    this->iVb = this->vb = viewBox;
    this->iOrtho = this->ortho = ortho;
}
