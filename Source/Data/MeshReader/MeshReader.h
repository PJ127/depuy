#pragma once

class QTextStream;

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

class Mesh;

//! Interface of a mesh file parser (one implementation per file format)
class MeshReader {
public:
    virtual ~MeshReader() = default;

    //! Reads vertices and triangles from the stream and appends them to \p mesh.
    //! On failure, returns false; \p mesh may then be partially filled and is
    //! cleaned up by the caller.
    virtual bool read(QTextStream &in, Mesh &mesh) = 0;
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
