#include <fstream>
#include "Articulator.h"

void Articulator::poseJoints(Joint &joint, vector<Quaterniond> const &rotations) {
//    cout << "Posing joint " << joint.name << " " << joint.id << endl;
//    cout << "children: " << joint.children.size() << endl;
    if (joint.rId == -1) // end site
        return;

    glPushMatrix();

    if (joint.id == hlBone) {
        glPushAttrib(GL_CURRENT_BIT);
        glColor3d(1, 0, 0);
//        cout << joint.name << endl;
    }

    joint.orientation = rotations[joint.rId];
    joint.draw();
    joint.applyGlTransforms();

    if (joint.id == hlBone) {
        glPopAttrib();
    }

    for (int i = 0; i < joint.children.size(); i++) {
        poseJoints(joint.children[i], rotations);
    }

    glPopMatrix();
}

void Articulator::pose(Skeleton &skeleton, const Frame &frame) {
    const Vector3d &translation = frame.translation;
    const vector<Quaterniond> &rotations = frame.rotations;
    glTranslated(translation.x(), translation.y(), translation.z());

    poseJoints(skeleton.root, rotations);
}

void Articulator::glDrawAttachments() {
    static bool enabled = false;
    Frame f;
    f.translation.setZero();
    f.rotations = vector<Quaterniond>(bvhData->motion.rotationsPerFrame,
            Quaterniond::Identity());
    if (hlBone == -1)
        glColor3d(1, 0, 0);
    else
        glColor3d(0,0,0);
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
    if (!enabled)
        mesh->glEnableColorArray();
    mesh->glDrawVertexArray();
}

void Articulator::attach(BvhData* data, Model* mesh) {
    this->bvhData = data;
    this->mesh = mesh;
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
