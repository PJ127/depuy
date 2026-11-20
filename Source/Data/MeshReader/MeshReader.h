#pragma once

#include <QList>
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

    //! Checks that \p tokens are the words of \p prefix (case-insensitive)
    //! followed by \p nbFloats numbers, stored in \p values if not null.
    //! Extra tokens are ignored. Returns false if the line does not match.
    static bool parseLine(const QStringList &tokens, const QStringList &prefix,
                          int nbFloats, QList<double> *values = nullptr);

    //! Reads the next line of \p stream and parses it with parseLine.
    static bool readLine(QTextStream &stream, const QStringList &prefix,
                         int nbFloats, QList<double> *values = nullptr);

    //! Reads a line "[prefix] x y z" into \p vertex.
    static bool readVertex(QTextStream &stream, Vertex &vertex,
                           const QStringList &prefix = QStringList());
};

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
