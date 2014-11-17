#include "Skeleton.h"

void Joint::draw() {
    glBegin(GL_LINES);

    glVertex3d(0, 0, 0);
    glVertex3d(offset.x(),
               offset.y(),
               offset.z());

    glEnd();
}

void Skeleton::glDraw() {

}
