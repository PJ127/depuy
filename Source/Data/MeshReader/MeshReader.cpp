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
bool dm::MeshReader::readVertex(QTextStream &stream, Vertex &vertex,
                                const QString &prefix)
//------------------------------------------------------------------------------
{
    return parseVertex(readTokens(stream), vertex, prefix);
}

//------------------------------------------------------------------------------
bool dm::MeshReader::parseVertex(const QStringList &tokens, Vertex &vertex,
                                 const QString &prefix)
//------------------------------------------------------------------------------
{
    const QStringList prefixWords = prefix.split(' ', Qt::SkipEmptyParts);
    const int nbPrefixWords = prefixWords.size();
    if (tokens.size() < nbPrefixWords + 3) {
        return false;
    }

    for (int i = 0; i < nbPrefixWords; ++i) {
        if (tokens[i].toLower() != prefixWords[i].toLower()) {
            return false;
        }
    }

    bool xIsOk = false;
    bool yIsOk = false;
    bool zIsOk = false;
    vertex.x = tokens[nbPrefixWords].toDouble(&xIsOk);
    vertex.y = tokens[nbPrefixWords + 1].toDouble(&yIsOk);
    vertex.z = tokens[nbPrefixWords + 2].toDouble(&zIsOk);
    return xIsOk && yIsOk && zIsOk;
}
