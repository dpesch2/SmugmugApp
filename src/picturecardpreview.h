/*
 * picturecardpreview.h
 *
 *  Created on: 13.6.2014
 *      Author: Dan
 */

#ifndef PICTURECARDPREVIEW_H_
#define PICTURECARDPREVIEW_H_

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <bb/system/InvokeManager>

class PictureCardPreview : public QObject
{
    Q_OBJECT

    QTimer* m_timer;
    QString m_fullName;
    int m_tries;
    bb::system::InvokeManager* m_invokeManager;

    void openPicturePreview(QString fullPath);
    Q_SLOT void tick();

public:
    PictureCardPreview(QObject* parent);
    void open(QString fullName);
};




#endif /* PICTURECARDPREVIEW_H_ */
