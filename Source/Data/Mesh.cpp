#include "Mesh.h"

#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

//---------------------------------------------------------------------------------
dm::Mesh::Mesh()
//---------------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
dm::Mesh::~Mesh()
//-----------------------------------------------------------------------------
{
    triangles.clear();
    for (size_t i = 0; i < vertices.size(); ++i) {
        delete vertices[i];
    }
    vertices.clear();
}

//-----------------------------------------------------------------------------
bool dm::Mesh::load(const QString &path)
//-----------------------------------------------------------------------------
{
    QFile file(path);
    if (!file.open(QFile::Text | QFile::ReadOnly)) {
        qCritical() << "Failed to open file " << path;
        return false;
    }
    QTextStream stream(&file);
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
        {
            QString currentLine = stream.readLine();
            if (currentLine.isEmpty()) {
                // end of file reached
                break;
            } else if (file.error() != QFile::NoError) {
                qCritical() << "An error ocurred while reading the file";
                return false;
            }

            QStringList tokens = currentLine.split(QRegularExpression("\\s+"),
                                                   Qt::SkipEmptyParts);
            if (tokens.empty() || tokens[0].toUpper() != "FACET") {
                if (tokens[0].toUpper() != "ENDSOLID") {
                    return false;
                }
                break;
            }

            if (tokens.size() >= 5 && tokens[1].toUpper() == "NORMAL") {
                bool normalIsOk = false;
                tri.normal[0] = tokens[2].toDouble(&normalIsOk);
                if (!normalIsOk) {
                    return false;
                }
                tri.normal[1] = tokens[3].toDouble(&normalIsOk);
                if (!normalIsOk) {
                    return false;
                }
                tri.normal[2] = tokens[4].toDouble(&normalIsOk);
                if (!normalIsOk) {
                    return false;
                }
            }
        }

        // 2nd line: 'outer loop'
        {
            QString currentLine = stream.readLine();
            if (!QString(currentLine)
                     .trimmed()
                     .toUpper()
                     .startsWith("OUTER LOOP")) {
                return false;
            }
        }

        // 3rd to 5th lines: 'vertex vix viy viz'
        for (unsigned i = 0; i < 3; ++i) {
            QString currentLine = stream.readLine();
            if (!QString(currentLine)
                     .trimmed()
                     .toUpper()
                     .startsWith("VERTEX")) {
                return false;
            }

            QStringList tokens =
                QString(currentLine)
                    .split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (tokens.size() < 4) {
                return false;
            }

            Vertex vertex;
            {
                bool vertexIsOk = false;
                vertex.x = tokens[1].toDouble(&vertexIsOk);
                if (!vertexIsOk) {
                    return false;
                }
                vertex.y = tokens[2].toDouble(&vertexIsOk);
                if (!vertexIsOk) {
                    return false;
                }
                vertex.z = tokens[3].toDouble(&vertexIsOk);
                if (!vertexIsOk) {
                    return false;
                }
            }

            tri.vertices[i] = new Vertex(vertex);
            vertices.push_back(tri.vertices[i]);
        }

        triangles.push_back(tri);

        // 6th line: 'endloop'
        {
            QString currentLine = stream.readLine();
            if (!QString(currentLine)
                     .trimmed()
                     .toUpper()
                     .startsWith("ENDLOOP")) {
                return false;
            }
        }

        // 7th and last line: 'endfacet'
        {
            QString currentLine = stream.readLine();
            if (!QString(currentLine)
                     .trimmed()
                     .toUpper()
                     .startsWith("ENDFACET")) {
                return false;
            }
        }
    }

    triangles.shrink_to_fit();
    vertices.shrink_to_fit();

    return true;
}
