#pragma once

#include <QString>
#include <QStringList>

class QTextStream;

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

struct Vertex;
class Mesh;

//! Interface of a mesh file parser (one implementation per file format)
class MeshReader {
public:
    virtual ~MeshReader() = default;

    //! Reads vertices and triangles from the stream and appends them to \p mesh.
    //! On failure, returns false; \p mesh may then be partially filled and is
    //! cleaned up by the caller.
    virtual bool read(QTextStream &stream, Mesh &mesh) = 0;

protected:
    //! Tokens of the next line, skipping empty lines and '#' comments.
    //! Returns an empty list at the end of the file.
    static QStringList readTokens(QTextStream &stream);

    //! Reads the next line of \p stream as "[prefix] x y z" into \p vertex.
    //! Returns false if the line does not match (see parseVertex).
    static bool readVertex(QTextStream &stream, Vertex &vertex,
                           const QString &prefix = QString());

    //! Same as readVertex, from the tokens of a line that was already read.
    //! \p prefix is a list of words (e.g. "vertex", "facet normal") that must
    //! match case-insensitively. Extra tokens are ignored.
    static bool parseVertex(const QStringList &tokens, Vertex &vertex,
                            const QString &prefix = QString());
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
