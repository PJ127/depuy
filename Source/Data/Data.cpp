#include "Data.h" // Header

// Qt
#include <QSharedPointer>

#include "../Data/Mesh.h"

QSharedPointer<dm::Data> sInstance = 0;

dm::Data::Data() : mMesh(0) { mMesh = new dm::Mesh(); }

dm::Data *dm::Data::instance() {
    if (!sInstance) {
        sInstance.reset(new Data);
    }
    return sInstance.data();
}
