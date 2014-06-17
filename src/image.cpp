#include "image.h"
#include <qdebug.h>

Image::Image(QString id,
			 QString key,
			 QString caption,
			 QString keywords,
			 QString thumbURL,
			 QString largeURL,
			 QString filename,
			 QString date,
			 QObject* parent )
           :
			 QObject(parent),
			 m_id(id),
             m_key(key),
             m_caption(caption),
             m_keywords(keywords),
             m_thumbURL(thumbURL),
             m_largeURL(largeURL),
             m_thumbImage(""),
             m_largeImage(""),
             m_filename(filename),
             m_date(date)
{
}

void Image::setImage(QString fullPath, bool isThumb ) {
    if (isThumb) {
        m_thumbImage = fullPath;
        emit thumbImageChanged();
    } else {
        m_largeImage = fullPath;
        emit largeImageChanged();
    }
}


void Image::dump() const {
    qDebug() << "Image:";
    qDebug() << "  id: " << m_id;
    qDebug() << "  key:" << m_key;
    qDebug() << "  caption:" << m_caption;
    qDebug() << "  filename:" << m_filename;
    qDebug() << "  date:" << m_date;
    qDebug() << "  keywords:" << m_keywords;
    qDebug() << "  thumbURL:" << m_thumbURL;
    qDebug() << "  largeURL:" << m_largeURL;
    qDebug() << "  thumbImage:" << m_thumbImage;
    qDebug() << "  largeImage" << m_largeImage;
}
