/*
 * Communicator.h
 *
 *  Created on: Apr 5, 2012
 *
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include <QtCore/QObject>
#include <QtNetwork/qhttp.h>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValueIterator>

#include "album.h"
#include "category.h"

class PictureCardPreview;
class Settings;


class Communicator : public QObject
{
    Q_OBJECT

public:
    Communicator(QObject* parent = NULL);
    ~Communicator();

// script interface
    Q_INVOKABLE void login(QString username );
    Q_INVOKABLE void loadImage(QString albumID, QString imageID, bool isThumb );
    Q_INVOKABLE void loadExif(QString albumID, QString imageID );
    Q_INVOKABLE QList<QObject*> getAlbums() const;
    Q_INVOKABLE QObject* getAlbum(QString albumID) const;
    Q_INVOKABLE QList<QObject*> getImages(QString albumID) const;
    Q_INVOKABLE QObject* getImage(QString albumID, QString imageID ) const;
    Q_INVOKABLE QList<QObject*> searchAlbums( QString re) const;
    Q_INVOKABLE QVariantList searchImages( QString re) const;
    Q_INVOKABLE void downloadAlbum( QString albumID, QString dirName);
    Q_INVOKABLE void downloadImage( QString albumId, QString imageId, QString fileName = "");
    Q_INVOKABLE void viewImage( QString albumId, QString imageId);
    Q_INVOKABLE void dump();

// public signals
signals:
    void albumsLoaded();
    void imageLoaded( QString albumId, QString imageId, QString localPath, bool isThumb );
    void exifLoaded( QString albumId, QString imageId);
    void imageSavedToShared( QString fullPath);
    void error( QString errorMsg);

private:
    void requestData( QString,  QString, const QMap<QString, QString>* param = NULL, const QMap<QString, QVariant> *responseProps = NULL);
    static QScriptValue queryProperty(QScriptValue&, QString);
    void dumpJSONData(QScriptValue);
    void updateImage( QString albumId, QString imageId, QString localPath, bool isThumb );
    void requestImage( QString jpegUrl, QString albumId, QString imageId, bool isThumb, QString imageFullPath = QString() );
    void requestExif( QString albumId,  QString imageId );
    void deleteLocalFiles();
    static bool isImageResponse(QNetworkReply* reply);
    void processImageResponse(QNetworkReply* reply);
    void processJsonResponse(QNetworkReply* reply);
    void clear();
    Image* findImage(QString albumId, QString imageId ) const;
    bool storeImage( QString fullPath, QNetworkReply* reply );

    QNetworkAccessManager* m_nwam;
    PictureCardPreview* m_pictureCardPreview;

//session
    QString m_username;
    QString m_session;

//caches
    typedef QMap<QString, Album* >  AlbumMap_t;
    AlbumMap_t m_albumsMap;

    typedef QMap<QString, QSharedPointer<Category> >  CategoryMap_t;
    CategoryMap_t m_categoryMap;
    CategoryMap_t m_subcategoryMap;

    QSharedPointer<Category> m_emptyCategory; // empty category with name ""

// downloaded files
    QStringList m_localFilenames;


private slots:
    void finishedSlot(QNetworkReply*);
    void processJSONResponse(QScriptValue*, QNetworkReply*);
    void processLoginResponse(QScriptValue*);
    void processAlbumsResponse(QScriptValue*);
    void processImagesResponse(QScriptValue*);
    void processExifResponse(QScriptValue*, QNetworkReply* reply);

// private signals
signals:
    void jsonDataReceived(QScriptValue*,QNetworkReply* reply);
    void loginResponseReceived(QScriptValue*);
    void albumsResponseReceived(QScriptValue*);
    void imagesResponseReceived(QScriptValue*);
    void exifResponseReceived(QScriptValue*,QNetworkReply* reply);
};

#endif /* COMMUNICATOR_H_ */
