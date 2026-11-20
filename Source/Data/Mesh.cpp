#include "Mesh.h" // Header

// QT
#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

//------------------------------------------------------------------------------
// class Mesh
//------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
dm::Mesh::Mesh()
//---------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------
dm::Mesh::~Mesh()
//-----------------------------------------------------------------------------
{
    m_triangles.clear();
    for (size_t i=0; i < m_nodes.size(); ++i)
    {
        delete m_nodes[i];
    }
    m_nodes.clear();
}

//-----------------------------------------------------------------------------
bool dm::Mesh::load(const QString& p_file)
//-----------------------------------------------------------------------------
{
    QFile file(p_file);
    if (!file.open(QFile::Text | QFile::ReadOnly))
    {
        qCritical() << "Failed to open file " << p_file;
        return false;
    }
    QTextStream stream(&file);
    {
        QString firstLine = stream.readLine();
        if (!firstLine.toUpper().startsWith("SOLID"))
        {
            qCritical() << "Input file is not an ASCII STL file";
            return false;
        }
    }

    while (true)
    {
            Triangle tri;

            //1st line of a 'facet': "facet normal ni nj nk" / or 'endsolid' (i.e. end of file)
            {
                    QString currentLine = stream.readLine();
                    if (currentLine.isEmpty())
                    {
                            //end of file reached
                            break;
                    }
                    else if (file.error() != QFile::NoError)
                    {
                            qCritical() << "An error ocurred while reading the file";
                            return false;
                    }

                    QStringList tokens = currentLine.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                    if (tokens.empty() || tokens[0].toUpper() != "FACET")
                    {
                            if (tokens[0].toUpper() != "ENDSOLID")
                            {
                                    return false;
                            }
                            break;
                    }

                    if (tokens.size() >= 5 && tokens[1].toUpper() == "NORMAL")
                    {
                        //read the normal
                        bool normalIsOk = false;
                        tri.normal[0] = tokens[2].toDouble(&normalIsOk);
                        if (!normalIsOk)
                            return false;
                        tri.normal[1] = tokens[3].toDouble(&normalIsOk);
                        if (!normalIsOk)
                            return false;
                        tri.normal[2] = tokens[4].toDouble(&normalIsOk);
                        if (!normalIsOk)
                            return false;
                        //double norm = sqrt(tri.normal[0]*tri.normal[0] + tri.normal[1]*tri.normal[1] + tri.normal[2]*tri.normal[2]);
                        //qDebug() << norm << endl;
                        //if (norm != 0)
                        //{
                        //    tri.normal[0] /= norm;
                        //    tri.normal[1] /= norm;
                        //    tri.normal[2] /= norm;
                        //}
                    }
            }

            //2nd line: 'outer loop'
            {
                QString currentLine = stream.readLine();
                if (!QString(currentLine).trimmed().toUpper().startsWith("OUTER LOOP"))
                {
                    return false;
                }
            }

            //3rd to 5th lines: 'vertex vix viy viz'
            for (unsigned i = 0; i < 3; ++i)
            {
                    QString currentLine = stream.readLine();
                    if (!QString(currentLine).trimmed().toUpper().startsWith("VERTEX"))
                    {
                        return false;
                    }

                    QStringList tokens = QString(currentLine).split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                    if (tokens.size() < 4)
                    {
                        return false;
                    }

                    //read vertex
                    Node P;
                    {
                            bool vertexIsOk = false;
                            P.coo[0] = tokens[1].toDouble(&vertexIsOk);
                            if (!vertexIsOk)
                                return false;
                            P.coo[1] = tokens[2].toDouble(&vertexIsOk);
                            if (!vertexIsOk)
                                return false;
                            P.coo[2] = tokens[3].toDouble(&vertexIsOk);
                            if (!vertexIsOk)
                                return false;
                    }

                    tri.nodes[i] = new Node(P);
                    m_nodes.push_back(tri.nodes[i]);
            }

            //add the new triangle
            m_triangles.push_back(tri);

            //6th line: 'endloop'
            {
                    QString currentLine = stream.readLine();
                    if (!QString(currentLine).trimmed().toUpper().startsWith("ENDLOOP"))
                    {
                        return false;
                    }
            }

            //7th and last line: 'endfacet'
            {
                    QString currentLine = stream.readLine();
                    if (!QString(currentLine).trimmed().toUpper().startsWith("ENDFACET"))
                    {
                        return false;
                    }
            }
    }

    m_triangles.shrink_to_fit();
    m_nodes.shrink_to_fit();

    return true;
}
