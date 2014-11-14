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

    const int* faceArray(int i);

    Vertex const* vertexArray();

    void draw(GLenum mode);

    void setDisplayList(unsigned int displayList);
    
    Vector3d const & translateCenterTo(Vector3d vertex);
    
    string name;
    float color[4];
    vector<Vertex> vertices;
    vector< vector<int> > faces;
    vector<int> facesFlattened;
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

};
