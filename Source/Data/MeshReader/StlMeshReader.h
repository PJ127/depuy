#pragma once

#include "MeshReader.h"

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Parser for ASCII STL meshes
class StlMeshReader : public MeshReader {
public:
    bool read(QTextStream &in, Mesh &mesh) override;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
