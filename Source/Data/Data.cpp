#include "Data.h"

#include <QSharedPointer>

#include "../Data/Mesh.h"

QSharedPointer<dm::Data> sharedInstance = 0;

dm::Data::Data() : currentMesh(0) { currentMesh = new dm::Mesh(); }

dm::Data *dm::Data::instance() {
    if (!sharedInstance) {
        sharedInstance.reset(new Data);
    }
    return sharedInstance.data();
}
