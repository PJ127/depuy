#include "MeshReaderFactory.h"

#include <QFileInfo>

#include "OffMeshReader.h"
#include "StlMeshReader.h"

//------------------------------------------------------------------------------
std::unique_ptr<dm::MeshReader> dm::makeMeshReader(const QString &path)
//------------------------------------------------------------------------------
{
    const QString extension = QFileInfo(path).suffix().toLower();

    if (extension == "stl") {
        return std::make_unique<StlMeshReader>();
    }
    if (extension == "off") {
        return std::make_unique<OffMeshReader>();
    }
    return nullptr;
}
