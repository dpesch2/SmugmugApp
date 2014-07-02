#include "album.h"
#include <qdebug.h>

Album::Album(QString id, QString key, QString title, QSharedPointer<Category> category, QSharedPointer<Category> subcategory, QObject* parent)
    : QObject(parent), m_id(id), m_key(key), m_title(title), m_category(category), m_subcategory(subcategory),
      m_imageCount(0), m_thumbImage("")
{
}

Album::~Album() {
}

void Album::appendImage(Image* image){
    m_images.append(image);
    m_imagesMap.insert(image->getId(), image );
}

Image* Album::getImage(QString imageID ) {
    return m_imagesMap[imageID];
}

void Album::dump() {
    qDebug() << "id:" << m_id;
    qDebug() << "key:" << m_key;
    qDebug() << "title:" << m_title;
    qDebug() << "thumbImage:" << m_thumbImage;
    qDebug() << "category:" << m_category->getName();
    qDebug() << "subcategory:" << m_subcategory->getName();
    qDebug() << "images:";
    foreach(Image* item, m_images) {
        item->dump();
    }

}

