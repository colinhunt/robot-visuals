#include "Skeleton.h"

void Joint::draw() {
    glBegin(GL_LINES);

    glVertex3d(0, 0, 0);
    glVertex3d(offset.x(),
               offset.y(),
               offset.z());

//    for (int i = 0; i < children.size(); i++) {
//        glVertex3d(0, 0, 0);
//        glVertex3d(children[i].offset.x(),
//                children[i].offset.y(),
//                children[i].offset.z());
//    }


    glEnd();
}

void Skeleton::glDraw() {

}
