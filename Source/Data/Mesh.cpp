#include "Mesh.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "MeshReader/MeshReaderFactory.h"

//---------------------------------------------------------------------------------
dm::Mesh::Mesh()
//---------------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
dm::Mesh::~Mesh()
//-----------------------------------------------------------------------------
{
    clear();
}

//-----------------------------------------------------------------------------
void dm::Mesh::clear()
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
    clear();

    std::unique_ptr<MeshReader> reader = makeMeshReader(path);
    if (!reader) {
        qCritical() << "Unsupported mesh format:" << path;
        return false;
    }

    QFile file(path);
    if (!file.open(QFile::Text | QFile::ReadOnly)) {
        qCritical() << "Failed to open file " << path;
        return false;
    }
    QTextStream stream(&file);

    if (!reader->read(stream, *this)) {
        clear();
        return false;
    }

    triangles.shrink_to_fit();
    vertices.shrink_to_fit();

    return true;
}
