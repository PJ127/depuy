#pragma once

#include "MeshReader.h"

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Parser for OFF (Object File Format) meshes
class OffMeshReader : public MeshReader {
public:
    bool read(QTextStream &in, Mesh &mesh) override;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
