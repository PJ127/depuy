#pragma once

#include <vector>

#include <QString>

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Vertex
struct Node {
    Node() { coo[0] = coo[1] = coo[2] = 0.0; }

    double coo[3];
};

struct Triangle {
    Triangle() {
        normal[0] = normal[1] = normal[2] = 0.0;
        nodes[0] = nodes[1] = nodes[2] = nullptr;
    }

    Node *nodes[3];
    double normal[3];
};

//! Triangular mesh
class Mesh {
  public:
    Mesh();
    virtual ~Mesh();

  public:
    //! Loads a STL mesh
    bool load(const QString &path);

  public:
    std::vector<Triangle> triangles;
    std::vector<Node *> nodes;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
