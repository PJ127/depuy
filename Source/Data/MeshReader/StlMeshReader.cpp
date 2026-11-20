#include "StlMeshReader.h"

#include <QDebug>
#include <QStringList>
#include <QTextStream>

#include "../Mesh.h"

//------------------------------------------------------------------------------
bool dm::StlMeshReader::read(QTextStream &stream, Mesh &mesh)
//------------------------------------------------------------------------------
{
    {
        QString firstLine = stream.readLine();
        if (!firstLine.toUpper().startsWith("SOLID")) {
            qCritical() << "Input file is not an ASCII STL file";
            return false;
        }
    }

    while (true) {
        Triangle tri;

        // 1st line of a 'facet': "facet normal ni nj nk" / or 'endsolid' (i.e.
        // end of file)
        QStringList tokens = readTokens(stream);
        if (tokens.isEmpty()) {
            if (stream.status() != QTextStream::Ok) {
                qCritical() << "An error occurred while reading the file";
                return false;
            }
            // end of file reached
            break;
        }
        if (tokens[0].toUpper() == "ENDSOLID") {
            break;
        }
        if (tokens[0].toUpper() != "FACET") {
            return false;
        }

        Vertex normal;
        if (!parseVertex(tokens, normal, "facet normal")) {
            return false;
        }
        tri.normal[0] = normal.x;
        tri.normal[1] = normal.y;
        tri.normal[2] = normal.z;

        // 2nd line: 'outer loop'
        if (readTokens(stream).join(' ').toUpper() != "OUTER LOOP") {
            return false;
        }

        // 3rd to 5th lines: 'vertex x y z'
        for (unsigned i = 0; i < 3; ++i) {
            Vertex vertex;
            if (!readVertex(stream, vertex, "vertex")) {
                return false;
            }
            // The mesh owns the vertex; the triangle only refers to it.
            mesh.vertices.push_back(new Vertex(vertex));
            tri.vertices[i] = mesh.vertices.back();
        }

        mesh.triangles.push_back(tri);

        // 6th line: 'endloop'
        if (readTokens(stream).join(' ').toUpper() != "ENDLOOP") {
            return false;
        }

        // 7th and last line: 'endfacet'
        if (readTokens(stream).join(' ').toUpper() != "ENDFACET") {
            return false;
        }
    }

    return true;
}
