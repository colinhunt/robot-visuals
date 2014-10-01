#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/errno.h>
#include <Eigen/Geometry>
#include <cmath>
using namespace Eigen;

#ifdef __APPLE__

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>

#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif


#include "model.h"

using namespace std;

const double PIOVER180 = M_PI / 180;

Model::Model(const char* fileName) : name("nameless"), translationOffset(0,0,0), orientation(1, 0, 0, 0) {
    initFromObjFile(fileName);
    normalize();
}

void Model::saveToFile() const {
    cout << "Saving model to output.obj" << endl;
    ofstream myfile("output.obj");
    if (myfile.is_open()) {
        myfile.precision(10);
        myfile << 'o' << ' ' << name << endl;
        for (int i = 0; i < vertices.size(); ++i) {
            myfile << 'v' << ' ' << vertices[i].x << ' ' << vertices[i].y << ' ' << vertices[i].z << endl;
        }
        for (int i = 0; i < faces.size(); ++i) {
            myfile << 'f';
            for (int j = 0; j < faces[i].size(); ++j) {
                myfile << ' ' << faces[i][j] + 1;
            }
            myfile << endl;
        }
    }
    else cerr << "Error: " << strerror(errno) << endl;
}

void Model::initFromObjFile(char const *fileName) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            stringstream lineStream(line.substr(2));
            if (line[0] == 'v') {
                Vertex vertex;
                lineStream >> vertex.x >> ws >> vertex.y >> ws >> vertex.z;
                vertices.push_back(vertex);
            } else if (line[0] == 'f') {
                int faceA[4] = {0,0,0,0};
                vector<int> faceV;
                lineStream >> faceA[0] >> ws >> faceA[1] >> ws >> faceA[2] >> ws >> faceA[3];
                for (int i = 0; i < 4; ++i)
                    if (faceA[i] != 0)
                        faceV.push_back(faceA[i] - 1);
                faces.push_back(faceV);
            } else if (line[0] == 'o') {
                lineStream >> name;
            } else {
                cerr << "Unrecognized line: " << line << endl;
            }
        }
        myfile.close();
    }
    else {
        if (myfile.fail())
            cout << "Logical error" << endl;
        if (myfile.bad())
            cout << "Read/writing error" << endl;
        cout << "Unable to open file" << endl;
        cerr << "Error: " << strerror(errno) << endl;
    }
}

Model::Vertex::Vertex(double x, double y, double z) : x(x), y(y), z(z) {}

//void Model::translateCenterTo(const Model::Vertex & p) {
//    Vertex center = calculateCenter();
//    Vector offset = p - center;
//    translateBy(offset);
//}

void Model::normalize() {
    Vertex pmax = calculateMaxVertex();
    Vertex pmin = calculateMinVertex();
    Vertex center = calculateCenter();
    Vertex origin;
    Vector offset = origin - center;

    Vector diagonal = pmax - pmin;

    double scale = max(max(diagonal.x, diagonal.y), diagonal.z);
    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i] += offset;

        vertices[i].x /= scale;
        vertices[i].y /= scale;
        vertices[i].z /= scale;
    }
}

Model::Vertex Model::calculateCenter() const {
    Vertex pmax = calculateMaxVertex();
    Vertex pmin = calculateMinVertex();

    Vertex center((pmax.x + pmin.x)/2, (pmax.y + pmin.y)/2, (pmax.z + pmin.z)/2);
    return center;
}

Model::Vertex Model::calculateMinVertex() const {
    Vertex pmin(vertices[0].x,
            vertices[0].y,
            vertices[0].z);

    for (int i = 0; i < vertices.size(); ++i) {
        pmin.x = min(pmin.x, vertices[i].x);
        pmin.y = min(pmin.y, vertices[i].y);
        pmin.z = min(pmin.z, vertices[i].z);
    }
    return pmin;
}

Model::Vertex Model::calculateMaxVertex() const {
    Vertex pmax(vertices[0].x,
            vertices[0].y,
            vertices[0].z);

    for (int i = 0; i < vertices.size(); ++i) {
        pmax.x = max(pmax.x, vertices[i].x);
        pmax.y = max(pmax.y, vertices[i].y);
        pmax.z = max(pmax.z, vertices[i].z);
    }
    return pmax;
}

Model::Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

Model::Vector operator-(Model::Vertex lhs, Model::Vertex const &rhs) {
    return Model::Vector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Model::Vertex operator+(Model::Vertex lhs, const Model::Vector & rhs) {
    return Model::Vertex(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}


int const* Model::faceArray(int i) {
    return &faces[i][0];
}

Model::Vertex const* Model::vertexArray() {
    return &vertices[0];
}

void Model::draw(GLenum mode) {
    for (int i = 0; i < faces.size(); ++i) {
        glDrawElements(mode, //mode
                (GLsizei) faces[i].size(),  //count, ie. how many indices
                GL_UNSIGNED_INT, //type of the index array
                faceArray(i));
    }
}

void Model::setDisplayList(GLuint displayList) {
    this->displayList = displayList;
}

const Model::Vector& Model::translateBy(Model::Vector offset) {
    Vector3d v(offset.x, offset.y, offset.z);
    v = orientation * v;
    translationOffset += Model::Vector(v.x(), v.y(), v.z());
    return translationOffset;
}

void Model::Vector::operator+=(Model::Vector const &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

void Model::Vertex::operator+=(const Model::Vector& offset) {
    this->x += offset.x;
    this->y += offset.y;
    this->z += offset.z;
}

const Model::Vector& Model::translateCenterTo(Model::Vertex vertex) {
    Vertex center = calculateCenter();
    Model::Vector offset = vertex - center;
    translationOffset += offset;
    return translationOffset;
}

void Model::rotateByAngleAxis(double degrees, Vector3d axis) {
    double radians = degrees * PIOVER180;
    Quaterniond r;
    r = AngleAxisd(radians, axis);
    cout << "rotation: " << r.w() << "," << r.x() << "," << r.y() << "," << r.z() << endl;
    orientation *= r;
    cout << "orientation right after rotation: " << orientation.w() << "," << orientation.x() << "," << orientation.y() << "," << orientation.z() << endl;
}

void Model::rotateX(double degrees) {
    rotateByAngleAxis(degrees, Vector3d::UnitX());
}

void Model::rotateY(double degrees) {
    rotateByAngleAxis(degrees, Vector3d::UnitY());    
}
void Model::rotateZ(double degrees) {
    rotateByAngleAxis(degrees, Vector3d::UnitZ());
}
