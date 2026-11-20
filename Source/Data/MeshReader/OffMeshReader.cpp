#include "OffMeshReader.h"

#include <QDebug>
#include <QStringList>
#include <QTextStream>

#include "../Mesh.h"

//------------------------------------------------------------------------------
bool dm::OffMeshReader::read(QTextStream &stream, Mesh &mesh)
//------------------------------------------------------------------------------
{
    {
        QStringList tokens = readTokens(stream);
        if (tokens.isEmpty() || tokens[0].toUpper() != "OFF") {
            qCritical() << "Input file is not an OFF file";
            return false;
        }
    }

    // Counts line: nbVertices nbFaces nbEdges
    QStringList tokens = readTokens(stream);
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
        Vertex vertex;
        if (!readVertex(stream, vertex)) {
            qCritical() << "Invalid vertex" << iVertex;
            return false;
        }
        mesh.vertices.push_back(new Vertex(vertex));
    }

    // One line per face: nbCorners i0 i1 ... (only triangles are supported)
    for (int iFace = 0; iFace < nbFaces; ++iFace) {
        tokens = readTokens(stream);
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
