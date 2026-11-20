#include "MeshReader.h"

#include <QRegularExpression>
#include <QTextStream>

#include "../Mesh.h"

//------------------------------------------------------------------------------
QStringList dm::MeshReader::readTokens(QTextStream &stream)
//------------------------------------------------------------------------------
{
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) {
            continue;
        }
        return line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    }
    return {};
}

//------------------------------------------------------------------------------
bool dm::MeshReader::parseLine(const QStringList &tokens,
                               const QStringList &prefix, int nbFloats,
                               QList<double> *values)
//------------------------------------------------------------------------------
{
    const int nbPrefixWords = prefix.size();
    if (tokens.size() < nbPrefixWords + nbFloats) {
        return false;
    }

    for (int i = 0; i < nbPrefixWords; ++i) {
        if (tokens[i].compare(prefix[i], Qt::CaseInsensitive) != 0) {
            return false;
        }
    }

    QList<double> result;
    for (int i = 0; i < nbFloats; ++i) {
        bool isOk = false;
        result.push_back(tokens[nbPrefixWords + i].toDouble(&isOk));
        if (!isOk) {
            return false;
        }
    }

    if (values) {
        *values = result;
    }
    return true;
}

//------------------------------------------------------------------------------
bool dm::MeshReader::readLine(QTextStream &stream, const QStringList &prefix,
                              int nbFloats, QList<double> *values)
//------------------------------------------------------------------------------
{
    return parseLine(readTokens(stream), prefix, nbFloats, values);
}

//------------------------------------------------------------------------------
bool dm::MeshReader::readVertex(QTextStream &stream, Vertex &vertex,
                                const QStringList &prefix)
//------------------------------------------------------------------------------
{
    QList<double> coordinates;
    if (!readLine(stream, prefix, 3, &coordinates)) {
        return false;
    }
    vertex.x = coordinates[0];
    vertex.y = coordinates[1];
    vertex.z = coordinates[2];
    return true;
}
