#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/errno.h>
#include <Eigen/Geometry>
#include <cmath>

#include "model.h"

#include "openglincludes.h"

using namespace std;
using namespace Eigen;

Model::Model(const char* fileName) : name("nameless") {
    initFromObjFile(fileName);
    normalize();
}

void Model::saveToFile() const {
//    cout << "Saving model to output.obj" << endl;
//    ofstream myfile("output.obj");
//    if (myfile.is_open()) {
//        myfile.precision(10);
//        myfile << 'o' << ' ' << name << endl;
//        for (int i = 0; i < vertices.size(); ++i) {
//            myfile << 'v' << ' ' << vertices[i].x << ' ' << vertices[i].y << ' ' << vertices[i].z << endl;
//        }
//        for (int i = 0; i < faces.size(); ++i) {
//            myfile << 'f';
//            for (int j = 0; j < faces[i].size(); ++j) {
//                myfile << ' ' << faces[i][j] + 1;
//            }
//            myfile << endl;
//        }
//    }
//    else {
//        cerr << "Error: " << strerror(errno) << endl;
//        exit(errno);
//    }
}

void Model::initFromObjFile(char const *fileName) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            string token;
            stringstream lineStream(line);
            lineStream >> token;
            if (token == "v") {
                Vertex vertex;
                lineStream >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            } else if (token == "vn") {
                Vertex normal;
                lineStream >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            } else if (token == "f") {
                Face face;
                unsigned int v, vt, vn;
                char sep = '/';
                while (lineStream >> v >> sep >> vt >> sep >> vn) {
//                    cout << v << sep << vt << sep << vn << endl;
                    Vdata vData;
                    vData.push_back(v - 1);
                    vData.push_back(vt - 1);
                    vData.push_back(vn - 1);
                    face.push_back(vData);
                }
                faces.push_back(face);
            } else if (token == "o") {
                lineStream >> name;
            } else {
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
        exit(errno);
    }
    loadIndexArrays();
    normalizeCenter();
    color[0] = color[1] = color[2] = 0.0;
    color[3] = 1.0;
}

void Model::normalize() {
    normalizeCenter();
    normalizeScale();
}

void Model::normalizeScale() {
    Vector3d pmax = calculateMaxVertex();
    Vector3d pmin = calculateMinVertex();
    Vector3d diagonal = pmax - pmin;

    double scale = max(max(diagonal.x(), diagonal.y()), diagonal.z());
    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i].x /= scale;
        vertices[i].y /= scale;
        vertices[i].z /= scale;
    }
}

void Model::normalizeCenter() {
    Vector3d center = calculateCenter();
    Vector3d origin;
    Vector3d offset = origin - center;

    for (int i = 0; i < vertices.size(); ++i) {
        vertices[i].x += offset.x();
        vertices[i].y += offset.y();
        vertices[i].z += offset.z();
    }
}

Vector3d Model::calculateCenter() const {
    Vector3d pmax = calculateMaxVertex();
    Vector3d pmin = calculateMinVertex();

    Vector3d center((pmax.x() + pmin.x())/2, (pmax.y() + pmin.y())/2, (pmax.z() + pmin.z())/2);
    return center;
}

Vector3d Model::calculateMinVertex() const {
    Vector3d pmin(vertices[0].x,
            vertices[0].y,
            vertices[0].z);

    for (int i = 0; i < vertices.size(); ++i) {
        pmin.x() = min(pmin.x(), vertices[i].x);
        pmin.y() = min(pmin.y(), vertices[i].y);
        pmin.z() = min(pmin.z(), vertices[i].z);
    }
    return pmin;
}

Vector3d Model::calculateMaxVertex() const {
    Vector3d pmax(vertices[0].x,
            vertices[0].y,
            vertices[0].z);

    for (int i = 0; i < vertices.size(); ++i) {
        pmax.x() = max(pmax.x(), vertices[i].x);
        pmax.y() = max(pmax.y(), vertices[i].y);
        pmax.z() = max(pmax.z(), vertices[i].z);
    }
    return pmax;
}


const Vector3d& Model::translateCenterTo(Vector3d vertex) {
    Vector3d center = calculateCenter();
    Vector3d offset = vertex - center;
    translationOffset += offset;
    return translationOffset;
}


void Model::glEnableVertexArray() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,   //3 components per vertex (x,y,z)
            GL_DOUBLE,
            sizeof(vertices[0]),
            &vertices[0]);
    glNormalPointer(
            GL_DOUBLE,
            sizeof(normals[0]),
            &normals[0]);
}

void Model::glColor() {
    glColor4fv(color);
}

void Model::glDrawVertexArray() {
    glDrawElements(GL_TRIANGLES, (GLsizei) triangleIndices.size(), GL_UNSIGNED_INT, &triangleIndices[0]);
}

void Model::loadIndexArrays() {
    for (int i = 0; i < faces.size(); ++i) {
        triangleIndices.push_back(faces[i][0][0]);
        triangleIndices.push_back(faces[i][1][0]);
        triangleIndices.push_back(faces[i][2][0]);
        // triangulate larger polygons
        if (faces[i].size() > 3) {
            triangleIndices.push_back(faces[i][2][0]);
            triangleIndices.push_back(faces[i][3][0]);
            triangleIndices.push_back(faces[i][0][0]);
        }
    }
}
