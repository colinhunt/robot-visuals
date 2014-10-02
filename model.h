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

    Model(char const *fileName);

    void saveToFile() const;

    Vector3d calculateCenter() const;

    const int* faceArray(int i);

    Vertex const* vertexArray();

    void draw(GLenum mode);

    void setDisplayList(unsigned int displayList);
    
    Vector3d const & translateCenterTo(Vector3d vertex);
    
    string name;
    vector<Vertex> vertices;
    vector< vector<int> > faces;
    unsigned int displayList;
    
private:

    void initFromObjFile(char const *fileName);

    void normalize();


    Vector3d calculateMaxVertex() const;

    Vector3d calculateMinVertex() const;

};
