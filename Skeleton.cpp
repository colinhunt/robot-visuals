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

//    glPointSize(5);
//    glBegin(GL_POINTS);
//
//    for (int i = 0; i < children.size(); i++) {
//        glVertex3d(0, 0, 0);
//        glVertex3d(children[i].offset.x(),
//                children[i].offset.y(),
//                children[i].offset.z());
//    }
//
//    glEnd();
}
