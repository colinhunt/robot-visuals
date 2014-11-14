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

Model::Model(const char* fileName) {
    initFromObjFile(fileName);
    normalize();
}

void Model::saveToFile() const {
    cout << "Saving model to meshout.obj" << endl;
    ofstream myfile("meshout.obj");
    if (myfile.is_open()) {
        myfile.precision(10);
        if (!name.empty())
            myfile << "o " << name << endl;
        if (!mtlFileName.empty())
            myfile << "mtlib " << mtlFileName << endl;
        for (int i = 0; i < vertices.size(); ++i) {
            myfile << "v " << vertices[i].x << ' ' << vertices[i].y << ' ' << vertices[i].z << endl;
        }
        for (int i = 0; i < normals.size(); ++i) {
            myfile << "vn " << normals[i].x << ' ' << normals[i].y << ' ' << normals[i].z << endl;
        }
        for (int i = 0; i < texels.size(); ++i) {
            myfile << "vt " << texels[i].x << ' ' << texels[i].y << endl;
        }
        // for each face
        for (int i = 0; i < faces.size(); ++i) {
            myfile << 'f';
            // for each vData in face
            for (int j = 0; j < faces[i].size(); ++j) {
                myfile << ' ' << faces[i][j][0] + 1 << '/' << faces[i][j][1] + 1 << '/' << faces[i][j][2] + 1;
            }
            myfile << endl;
        }
    }
    else {
        cerr << "Error: " << strerror(errno) << endl;
        exit(errno);
    }
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
            } else if (token == "vt") {
                Texel texel;
                lineStream >> texel.x >> texel.y;
                texels.push_back(texel);
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
            } else if (token == "mtllib") {
                lineStream >> mtlFileName;
                loadTexture(mtlFileName, string(fileName));
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
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,   //3 components per vertex (x,y,z)
            GL_DOUBLE,
            sizeof(vertices[0]),
            &vertices[0]);
    glNormalPointer(
            GL_DOUBLE,
            sizeof(normals[0]),
            &normals[0]);
    glTexCoordPointer(2,
            GL_DOUBLE,
            sizeof(texels[0]),
            &texels[0]);
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

void Model::loadTexture(string mtlFileName, string objFileName) {
    string line;
    string tgaFullPath;

    string path = objFileName.substr(0, objFileName.find_last_of('/') + 1);
    string mtlFullPath = path + mtlFileName;
    ifstream myfile(mtlFullPath.c_str());
    if (myfile.is_open()) {
        for (; line.substr(0, 7) != "map_Kd "; getline(myfile, line));
        tgaFullPath = path + line.substr(7);
    } else {
        perror("Unable to open mtl file");
        exit(errno);
    }
    myfile.close();

    // only try and load a tga for now
    size_t extP = tgaFullPath.find_last_of('.');
    tgaFullPath.replace(extP, string::npos, ".tga");

    bool success = textureImage.LoadTGA(tgaFullPath.c_str());
    if (!success) {
        perror("Unable to load texture file");
        exit(errno);
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, textureImage.getWidth(), textureImage.getHeight(), GL_BGRA, GL_UNSIGNED_BYTE,
            textureImage.getDataForOpenGL());
}

void Model::glEnableTextures() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Model::glDisableTextures() {
    glDisable(GL_TEXTURE_2D);
}
