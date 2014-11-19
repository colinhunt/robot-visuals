#include <fstream>
#include "Articulator.h"
#include "FrameTimer.h"

Articulator::Articulator() : hlBone(-1),
                             frameTime(0.008333),
                             timer(frameTime),
                             ofps((int) round(1.0 / frameTime)) {
    fps = ofps;
    animateOn = false;
}

int Articulator::frameStep() {
//    cout << fps << " " << bvhData->motion.lowestFps << endl;
    return fps / bvhData->motion.lowestFps;
}

void Articulator::animate() {
    TimeVal timeLeft = timer.TimeLeft();
//    cout << "timeLeft " << timeLeft << endl;
    if (animateOn && timeLeft <= 0) {
        TimeVal delta = frameTime - timeLeft;
        int correction = (int) round(delta / frameTime);
//        cout << "correction " << correction << endl;
        glutPostRedisplay();
//        cout << "frameStep: " << frameStep() * correction << endl;
        currFrame += frameStep() * correction;
        currFrame = GltUtil::mod(currFrame, bvhData->motion.interpolatedFrames.size());
//        cout << currFrame << endl;
        timer.Start();
    } else {
//        cout << "no display frame " << timer.TimeLeft() << endl;
    }
}

void Articulator::drawNextFrame() {
    // use current frame to pose skeleton (update transformations for each joint)
    // loop through vertices in mesh and update with info from joints and attachments
    // draw mesh
    Frame &frame = bvhData->motion.interpolatedFrames[currFrame];
    Model::Vertex zero = {0,0,0};
    mesh->vertexArray = vector<Model::Vertex>(mesh->vertices.size(), zero);
    glColor3d(1,0,0);
    pose(bvhData->skeleton, frame);
    mesh->glColor();
//    for (int i = 0; i < mesh->vertices.size(); ++i) {
//        Vector3d oldP(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
//        Vector3d newP(Vector3d::Zero());
//        for (int j = 0; j < attachments[i].size(); ++j) {
//            int bId = attachments[i][j].first; // bone id and weight
//            double weight = attachments[i][j].second;
//            // find joint in skel from id and get the orientation applied
//            Joint& joint = *bvhData->skeleton.joints[bId];
//            Vector3d jthP = oldP - joint.absPosition; // Mj
//            jthP = joint.orientation * jthP;                // Rj
//            jthP += joint.absPosition; // Mj^-1
//            jthP = Scaling(weight) * jthP;                        // w_ij
//            newP += jthP;
//        }
//        mesh->vertices[i].x = newP.x();
//        mesh->vertices[i].y = newP.y();
//        mesh->vertices[i].z = newP.z();
//    }
    mesh->glDrawVertexArray();
}

void Articulator::pose(Skeleton &skeleton, const Frame &frame) {
    const Vector3d &translation = frame.translation;
    const vector<Quaterniond> &rotations = frame.rotations;
    glTranslated(translation.x(), translation.y(), translation.z());

    poseJoints(skeleton.root, Quaterniond::Identity(), rotations, Vector3d::Zero());
}

void Articulator::poseJoints(Joint &joint,
        Quaterniond currRotation,
        vector<Quaterniond> const &rotations,
        Vector3d currOffset) {
//    cout << "Posing joint " << joint.name << " " << joint.id << endl;
//    cout << "children: " << joint.children.size() << endl;
//    if (joint.rId == -1) // end site
//        return;

    glPushMatrix();

    if (joint.id == hlBone) {
        glPushAttrib(GL_CURRENT_BIT);
        glColor3d(1, 0, 0);
//        cout << joint.name << endl;
    }

    joint.draw();

    if (joint.rId != -1)
        joint.orientation = rotations[joint.rId];

    joint.applyGlTransforms();
    currRotation *= joint.orientation;


//    for (int i = 0; i < mesh->vertices.size(); ++i) {
//        Vector3d jthP = oldP - joint.absPosition; // Mj
//                jthP = joint.orientation * jthP;                // Rj
//                jthP += joint.absPosition; // Mj^-1
//                jthP = Scaling(weight) * jthP;                        // w_ij
//                newP += jthP;


    for (int i = 0; i < mesh->vertices.size(); ++i) {
        for (int j = 0; j < attachments[i].size(); ++j)
            if (attachments[i][j].first == joint.id) {
                Vector3d oldP(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
                double weight = attachments[i][j].second;
                Vector3d jthP = oldP - currOffset;   // Mj
//                Vector3d jthP = oldP;   // Mj
                jthP = currRotation * jthP;            // Rj
                jthP += currOffset;                  // Mj^-1
                jthP[0] = jthP[0] * weight;
                jthP[1] = jthP[1] * weight;
                jthP[2] = jthP[2] * weight;              // w_ij
//                cout << "weight " << weight << endl;
                mesh->vertexArray[i].x += jthP.x();
                mesh->vertexArray[i].y += jthP.y();
                mesh->vertexArray[i].z += jthP.z();
//                mesh->vertexArray[i].x = jthP.x();
//                mesh->vertexArray[i].y = jthP.y();
//                mesh->vertexArray[i].z = jthP.z();
                break;
            }
    }

    currOffset += joint.offset;

    if (joint.id == hlBone) {
        glPopAttrib();
    }

    for (int i = 0; i < joint.children.size(); i++) {
        poseJoints(*joint.children[i], currRotation, rotations, currOffset);
    }

    glPopMatrix();
}

void Articulator::glDrawAttachments() {
    Frame f;
    f.translation.setZero();
    f.rotations = vector<Quaterniond>(bvhData->motion.rotationsPerFrame,
            Quaterniond::Identity());
    if (hlBone == -1)
        glColor3d(1, 0, 0);
    else
        glColor3d(0,0,0);
    Model::Vertex zero = {0,0,0};
    mesh->vertexArray = vector<Model::Vertex>(mesh->vertices.size(), zero);
    pose(bvhData->skeleton, f);
    mesh->colors.clear();
    int att = 0;
    for (int i = 0; i < mesh->vertices.size(); ++i) {
        Model::Vertex color3 = {0.1,0.1,0.1};
        mesh->colors.push_back(color3);
        for (int j = 0; j < attachments[i].size(); ++j)
            if (attachments[i][j].first == hlBone) {
                double color = 0.9 * attachments[i][j].second + 0.1;
                mesh->colors.back().x = color;
                mesh->colors.back().y = color;
                mesh->colors.back().z = color;
                att++;
                break;
            }
    }
    cout << "attached: " << att << endl;
    if (hlBone == -1) {
        mesh->glDisableColorArray();
        mesh->glColor();
    } else {
        mesh->glEnableColorArray();
    }
    mesh->glDrawVertexArray();
}

void Articulator::attach(BvhData* data, Model* mesh) {
    this->bvhData = data;
    this->mesh = mesh;
    timer.Set(data->motion.frameTime);
    frameTime = data->motion.frameTime;
    ofps = (int) round(1.0 / frameTime);
    fps = ofps;
}

void Articulator::highlightNextBone() {
    hlBone = GltUtil::mod(hlBone + 1, bvhData->skeleton.size);
    cout << hlBone << endl;
}

void Articulator::highlightPrevBone() {
    hlBone = GltUtil::mod(hlBone - 1, bvhData->skeleton.size);
    cout << hlBone << endl;
}

void Articulator::reset() {
    hlBone = -1;
    fps = ofps;
    currFrame = 0;
    animateOn = false;
}

void Articulator::initAttachments(char* attFileName) {
    string line;
    ifstream myfile(attFileName);
    if (!myfile.is_open()) {
        perror("Unable to open .att file");
        exit(EXIT_FAILURE);
    }
    while (getline(myfile, line)) {
        char sep;
        stringstream lineStream(line);
        int v, bId;
        double w;
        lineStream >> v;
        vector<pair<int, double> > weights;
        while (lineStream >> sep >> bId >> sep >> w >> sep) {
//            cout << v << " " << bId << " " << w << endl;
            weights.push_back(pair<int,double>(bId, w));
        }
        attachments.push_back(weights);
    }
    myfile.close();
}

void Articulator::startAnimation() {
    animateOn = true;
    cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
    timer.Start();
}

void Articulator::stopAnimation() {
    timer.Stop();
    animateOn = false;
}

void Articulator::decreaseFps() {
    fps -= 10;
    cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
}

void Articulator::increaseFps() {
    fps += 10;
    cout << "Framerate: " << fps << endl;
//            cout << "frameStep: " << frameStep() << endl;
}

void Articulator::saveAttToFile() {
    cout << "Saving att data to attachout.att" << endl;
    ofstream myfile("attachout.att");
    if (myfile.is_open()) {
        myfile.precision(10);
        for (int i = 0; i < attachments.size(); ++i) {
            myfile << i + 1;
            for (int j = 0; j < attachments[i].size(); ++j) {
                myfile << " (" << attachments[i][j].first
                        << "," << attachments[i][j].second << ")";
            }
            myfile << endl;
        }
    } else {
        perror("Writing bvh file");
        exit(errno);
    }
    myfile.close();
}
