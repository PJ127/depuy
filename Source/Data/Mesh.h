#pragma once

#include <vector>

#include <QString>

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Vertex (3D point)
struct Vertex {
    Vertex() : x(0.0), y(0.0), z(0.0) {}

    double x;
    double y;
    double z;
};

struct Triangle {
    Triangle() {
        normal[0] = normal[1] = normal[2] = 0.0;
        vertices[0] = vertices[1] = vertices[2] = nullptr;
    }

    Vertex *vertices[3];
    double normal[3];
};

//! Triangular mesh
class Mesh {
public:
    Mesh();
    virtual ~Mesh();

public:
    //! Loads a mesh (format deduced from the file extension: STL, OFF).
    //! Replaces the current content; the mesh is left empty on failure.
    bool load(const QString &path);

    //! Removes all triangles and vertices
    void clear();

public:
    std::vector<Triangle> triangles;
    std::vector<Vertex *> vertices;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
