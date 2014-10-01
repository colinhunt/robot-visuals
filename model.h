#pragma once

#ifdef __APPLE__

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#include <AGL/agl.h>

#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif


#include <string>
#include <vector>

using namespace std;

class Model {

public:

    struct Vector {
        Vector(double x, double y, double z);

        void operator+=(const Vector& other);

        double x, y, z;
    };

    struct Vertex {
        Vertex(double x = 0, double y = 0, double z = 0);

        void operator+=(const Vector& offset);

        double x, y, z;
    };

    struct Rotation {
        GLfloat angle, x, y, z;
    };

    Model(char const *fileName);

    void saveToFile() const;

    Vertex calculateCenter() const;

    const int* faceArray(int i);

    Model::Vertex const* vertexArray();

    void draw(GLenum mode);

    void setDisplayList(unsigned int displayList);

    Vector const & translateBy(Model::Vector offset);

    Vector const & translateCenterTo(Vertex vertex);

    string name;
    vector<Vertex> vertices;
    vector< vector<int> > faces;
    unsigned int displayList;
    Vector translationOffset;
    GLfloat alpha;
    GLfloat beta;
    GLfloat gamma;
    vector<Rotation> rotations;


private:

    void initFromObjFile(char const *fileName);

    void normalize();


    Vertex calculateMaxVertex() const;

    Vertex calculateMinVertex() const;

};

Model::Vector operator-(Model::Vertex lhs, const Model::Vertex & rhs);
Model::Vertex operator+(Model::Vertex lhs, const Model::Vector & rhs);
