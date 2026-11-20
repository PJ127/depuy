#include "Data.h" // Header

//Qt
#include <QSharedPointer>

#include "../Data/Mesh.h"

QSharedPointer<dm::Data> s_instance = 0;

dm::Data::Data()
    : m_mesh(0)
{
	m_mesh = new dm::Mesh();
}

dm::Data* dm::Data::instance()
{
        if (!s_instance)
        {
                s_instance.reset(new Data);
        }
        return s_instance.data();
}

