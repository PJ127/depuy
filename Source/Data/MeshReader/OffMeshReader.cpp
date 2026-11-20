#include "OffMeshReader.h"

#include <QDebug>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#include "../Mesh.h"

namespace {

//! Returns the tokens of the next line that is neither empty nor a comment.
//! Returns an empty list at the end of the file.
QStringList nextTokens(QTextStream &in)
{
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }
        return line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    }
    return {};
}

} // namespace

//------------------------------------------------------------------------------
bool dm::OffMeshReader::read(QTextStream &in, Mesh &mesh)
//------------------------------------------------------------------------------
{
    {
        QStringList tokens = nextTokens(in);
        if (tokens.isEmpty() || tokens[0].toUpper() != "OFF") {
            qCritical() << "Input file is not an OFF file";
            return false;
        }
    }

    // Counts line: nbVertices nbFaces nbEdges
    QStringList tokens = nextTokens(in);
    if (tokens.size() != 3) {
        qCritical() << "Second line should contain three numbers";
        return false;
    }

    bool countsAreOk = false;
    const int nbVertices = tokens[0].toInt(&countsAreOk);
    if (!countsAreOk || nbVertices < 0) {
        qCritical() << "Invalid number of vertices:" << tokens[0];
        return false;
    }
    const int nbFaces = tokens[1].toInt(&countsAreOk);
    if (!countsAreOk || nbFaces < 0) {
        qCritical() << "Invalid number of faces:" << tokens[1];
        return false;
    }

    // One line per vertex: x y z
    for (int iVertex = 0; iVertex < nbVertices; ++iVertex) {
        tokens = nextTokens(in);
        if (tokens.size() < 3) {
            qCritical() << "Vertex" << iVertex << "should contain three numbers";
            return false;
        }

        Vertex vertex;
        bool xIsOk = false;
        bool yIsOk = false;
        bool zIsOk = false;
        vertex.x = tokens[0].toDouble(&xIsOk);
        vertex.y = tokens[1].toDouble(&yIsOk);
        vertex.z = tokens[2].toDouble(&zIsOk);
        if (!xIsOk || !yIsOk || !zIsOk) {
            qCritical() << "Invalid coordinates for vertex" << iVertex;
            return false;
        }

        mesh.vertices.push_back(new Vertex(vertex));
    }

    // One line per face: nbCorners i0 i1 ... (only triangles are supported)
    for (int iFace = 0; iFace < nbFaces; ++iFace) {
        tokens = nextTokens(in);
        if (tokens.isEmpty()) {
            qCritical() << "Face" << iFace << "is missing";
            return false;
        }

        bool cornersAreOk = false;
        const int nbCorners = tokens[0].toInt(&cornersAreOk);
        if (!cornersAreOk || nbCorners != 3 || tokens.size() < 4) {
            qCritical() << "Face" << iFace << "is not a triangle";
            return false;
        }

        Triangle triangle;
        for (int iCorner = 0; iCorner < 3; ++iCorner) {
            bool indexIsOk = false;
            const int index = tokens[iCorner + 1].toInt(&indexIsOk);
            if (!indexIsOk || index < 0 ||
                index >= static_cast<int>(mesh.vertices.size())) {
                qCritical() << "Invalid vertex index in face" << iFace;
                return false;
            }
            triangle.vertices[iCorner] = mesh.vertices[index];
        }

        mesh.triangles.push_back(triangle);
    }

    return true;
}
