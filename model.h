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
#include <Eigen/Geometry>
using namespace Eigen;
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

    Model(char const *fileName);

    void saveToFile() const;

    Vertex calculateCenter() const;

    const int* faceArray(int i);

    Model::Vertex const* vertexArray();

    void draw(GLenum mode);

    void setDisplayList(unsigned int displayList);

    Vector const & translateBy(Model::Vector offset);

    Vector const & translateCenterTo(Vertex vertex);
    
    void rotateX(double degrees);
    void rotateY(double degrees);
    void rotateZ(double degrees);
    
    string name;
    vector<Vertex> vertices;
    vector< vector<int> > faces;
    unsigned int displayList;
    Vector translationOffset;
    Quaterniond orientation;
    
private:

    void initFromObjFile(char const *fileName);

    void normalize();


    Vertex calculateMaxVertex() const;

    Vertex calculateMinVertex() const;

    void rotateByAngleAxis(double degrees, Vector3d axis);

};

Model::Vector operator-(Model::Vertex lhs, const Model::Vertex & rhs);
Model::Vertex operator+(Model::Vertex lhs, const Model::Vector & rhs);
