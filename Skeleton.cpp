#include "Skeleton.h"

void Joint::draw() {
    glBegin(GL_LINES);

    for (int i = 0; i < children.size(); i++) {
        glVertex3d(0, 0, 0);
        glVertex3d(children[i].offset.x(),
                children[i].offset.y(),
                children[i].offset.z());
    }

    glEnd();

    glPointSize(5);
    glBegin(GL_POINTS);

    for (int i = 0; i < children.size(); i++) {
        glVertex3d(0, 0, 0);
        glVertex3d(children[i].offset.x(),
                children[i].offset.y(),
                children[i].offset.z());
    }

    glEnd();
}

Translation3d Joint::getTranslation(double v1, double v2, double v3) const {
    double mapped[3];

    mapped[positionMap[0]] = v1;
    mapped[positionMap[1]] = v2;
    mapped[positionMap[2]] = v3;

    return Translation3d(mapped[0], mapped[1], mapped[2]);
}
