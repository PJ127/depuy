#pragma once

#include <memory>

#include <QString>

#include "MeshReader.h"

//------------------------------------------------------------------------------
namespace dm {
//------------------------------------------------------------------------------

//! Creates the reader matching the file extension (".stl", ".off").
//! Returns nullptr if the format is not supported.
std::unique_ptr<MeshReader> makeMeshReader(const QString &path);

//------------------------------------------------------------------------------
} // namespace dm
//------------------------------------------------------------------------------
