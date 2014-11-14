#pragma once

#include <string>
#include <vector>
#include <Eigen/Geometry>

#include "GlTransformable.h"
#include "openglincludes.h"

using namespace Eigen;
using namespace std;

class Model : public GlTransformable {

public:
    
    struct Vertex {
        double x, y, z;
    };

    Model() {};

    Model(char const *fileName);

    void saveToFile() const;

    Vector3d calculateCenter() const;

    Vector3d const & translateCenterTo(Vector3d vertex);

    typedef vector<unsigned int> Vdata;
    typedef vector<Vdata> Face;

    string name;
    float color[4];
    vector<Vertex> vertices;
    vector<Vertex> normals;
    vector<Face> faces;
    vector<int> triangleIndices;
    unsigned int displayList;
    GLuint vbo;
    GLuint vao;
    GLuint ibo;

    void initFromObjFile(char const *fileName);

    void glEnableVertexArray();

    void glColor();

    void glDrawVertexArray();

private:

    void normalize();


    Vector3d calculateMaxVertex() const;

    Vector3d calculateMinVertex() const;

    void normalizeCenter();

    void normalizeScale();

    void loadIndexArrays();

};
