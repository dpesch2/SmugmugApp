#include "category.h"

Category::Category()
    :m_id(QString::null), m_name(QString::null)
{
}

Category::Category(QString id, QString name)
    : m_id(id), m_name(name)
{
}
