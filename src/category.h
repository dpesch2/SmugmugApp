#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtCore/QObject>

class Category
{
public:
    Category(QString id, QString name);

    QString getName() const { return m_name; }


private:
    QString m_id;
    QString m_name;
};

#endif // CATEGORY_H
