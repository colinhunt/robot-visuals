#pragma once

#include <string>
#include <vector>
#include <Eigen/Geometry>

#include "openglincludes.h"

using namespace Eigen;
using namespace std;

class Model {

public:
    
    struct Vertex {
        double x, y, z;
    };

    Model(char const *fileName);

    void saveToFile() const;

    Vector3d calculateCenter() const;

    const int* faceArray(int i);

    Vertex const* vertexArray();

    void draw(GLenum mode);

    void setDisplayList(unsigned int displayList);

    Vector3d const & translateBy(Vector3d offset);

    Vector3d const & translateCenterTo(Vector3d vertex);
    
    void applyGlTransforms();
    
    void rotateX(double degrees);
    void rotateY(double degrees);
    void rotateZ(double degrees);
    
    string name;
    vector<Vertex> vertices;
    vector< vector<int> > faces;
    unsigned int displayList;
    Vector3d translationOffset;
    Quaterniond orientation;
    
private:

    void initFromObjFile(char const *fileName);

    void normalize();


    Vector3d calculateMaxVertex() const;

    Vector3d calculateMinVertex() const;

    void rotateByAngleAxis(double degrees, Vector3d axis);

};
