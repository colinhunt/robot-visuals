#pragma once

#include <string>
#include <vector>
#include <Eigen/Geometry>

#include "GlTransformable.h"
#include "openglincludes.h"
#include "TGALoader.h"

using namespace Eigen;
using namespace std;

class Model : public GlTransformable {

public:
    
    struct Vertex {
        double x, y, z;
    };

    struct Texel {
        double x, y;
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
    vector<Vertex> colors;
    vector<Texel> texels;
    vector<Face> faces;
    vector<int> triangleIndices;

    NS_TGALOADER::IMAGE textureImage;
    GLuint textureId;

    string mtlFileName;

    void initFromObjFile(char const *fileName);

    void glEnableVertexArray();

    void glColor();

    void glDrawVertexArray();

    void glDisableTextures();

    void glEnableTextures();

    void glEnableColorArray();

private:

    void normalize();


    Vector3d calculateMaxVertex() const;

    Vector3d calculateMinVertex() const;

    void normalizeCenter();

    void normalizeScale();

    void loadIndexArrays();

    void loadTexture(string mtlFileName, string objFileName);
};
