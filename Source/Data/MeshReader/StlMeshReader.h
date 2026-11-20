#pragma once

#include "MeshReader.h"

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Parser for ASCII STL meshes
class StlMeshReader : public MeshReader {
public:
    bool read(QTextStream &stream, Mesh &mesh) override;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
