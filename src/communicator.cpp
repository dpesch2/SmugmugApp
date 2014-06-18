/*
 * Communicator.cpp
 *
 *  Created on: Apr 5, 2012
 *      Author: dpesch
 */

#include <qdebug.h>
#include <QDir>
#include <limits.h>

#include "communicator.h"
#include "picturecardpreview.h"

const QString API_KEY = "D7U8tZqdFT9XI6UvuPoyqpNAE2WwwBpP"; //per client
const QString LOGIN = "smugmug.login.anonymously";
const QString ALBUMS = "smugmug.albums.get";
const QString IMAGES = "smugmug.images.get";
const QString EXIF = "smugmug.images.getEXIF";

// Dynamic properties for Request object
const char* IS_EXTIF_REQEST = "isExifRequest";
const char* IS_IMAGE_REQEST = "isImageRequest";
const char* ALBUM_ID = "albumID";
const char* IMAGE_ID = "imageID";
const char* IS_THUMB = "isThumb";
const char* IMAGE_FULL_PATH = "imageFullPath";

// for debug purpose only
//const uint MAX_ALBUMS =  3;
//const uint MAX_IMAGES =  4;

const uint MAX_ALBUMS = UINT_MAX / 2; // divided to be compatible with int indexes!!!
const uint MAX_IMAGES = UINT_MAX / 2;

// where to save albums/images
const QString SAVE_LOCATION = "/../shared/photos/smugmugdemo/";

Communicator::Communicator(QObject* parent) :  QObject(parent) {

	m_nwam = new QNetworkAccessManager(this);
	m_pictureCardPreview = new PictureCardPreview(this);

	connect(m_nwam, SIGNAL(finished(QNetworkReply*)), this,
			SLOT(finishedSlot(QNetworkReply*)));

	connect(this, SIGNAL(jsonDataReceived(QScriptValue*,QNetworkReply*)), this,
			SLOT(processJSONResponse(QScriptValue*,QNetworkReply*)));

	connect(this, SIGNAL(loginResponseReceived(QScriptValue*)), this,
			SLOT(processLoginResponse(QScriptValue*)));

	connect(this, SIGNAL(albumsResponseReceived(QScriptValue*)), this,
			SLOT(processAlbumsResponse(QScriptValue*)));

	connect(this, SIGNAL(imagesResponseReceived(QScriptValue*)), this,
			SLOT(processImagesResponse(QScriptValue*)));

	connect(this, SIGNAL(exifResponseReceived(QScriptValue*,QNetworkReply*)),
			this, SLOT(processExifResponse(QScriptValue*,QNetworkReply*)));

	qDebug() << "Qt homedir " << QDir::homePath();
	qDebug() << "Qt currentdir " << QDir::currentPath();

	// populate categories with empty category representing non-assigned categories/subcategories
	const QString emptyKey("<empty>");
	m_emptyCategory = QSharedPointer<Category>(
			new Category(emptyKey, /*name*/""));
	m_categoryMap.insert(emptyKey, m_emptyCategory);
	m_subcategoryMap.insert(emptyKey, m_emptyCategory);
}

// cleanup temporary files
Communicator::~Communicator() {
	deleteLocalFiles();
}

// clean old user first
void Communicator::clear() {

	deleteLocalFiles();
	m_username = "";
	m_session = "";

	foreach(Album* album,  m_albumsMap.values() ) {
		delete album;
	}
	m_albumsMap.clear();
	m_categoryMap.clear();
	m_subcategoryMap.clear();
}

void Communicator::login(QString username) {

	// clean old user data first
	clear();

	m_username = username;
	requestData(LOGIN, API_KEY, NULL);
}

void Communicator::viewImage(QString albumId, QString imageId) {

	Album* album = m_albumsMap[albumId];

	if (album == NULL) {
		qWarning() << "Unknown album '" << albumId << "' at " << __FUNCTION__;
		return;
	}

	Image* image = findImage(albumId, imageId);
	if (image == NULL) {
		qWarning() << "Unknown image '" << imageId << "' at " << __FUNCTION__;
		return;
	}

	QDir home(QDir::homePath());
	QString dirPath = QDir::homePath() + SAVE_LOCATION + album->getTitle();
	dirPath.replace(" ", "_");
	home.mkpath(dirPath);

	QString fullPath = dirPath + QString("/") + image->getFilename();
	fullPath.replace(" ", "_");
	QFile file(fullPath);

	if (! file.exists()) {
		downloadImage(albumId, imageId);
	}
	m_pictureCardPreview->open(fullPath);

}

void Communicator::loadImage(QString albumID, QString imageID, bool isThumb) {
	Image* image = findImage(albumID, imageID);
	if (image == NULL) {
		qWarning("Image is null");
		return;
	}
	QString url = (isThumb ? image->getThumbURL() : image->getLargeURL());
	requestImage(url, albumID, imageID, isThumb);
}

void Communicator::downloadAlbum( QString albumId, QString downloadDir) {
	Album* album = m_albumsMap[albumId];
	if (album == NULL) {
		qWarning() << "Unknown album '" << albumId << "' at " << __FUNCTION__;
		return;
	}

	QDir dir(downloadDir);
	QString dirPath = downloadDir + QDir::separator() + album->getTitle();
	dirPath.replace(" ", "_");
	dir.mkpath(dirPath);

	// check if all images are downloaded
	foreach (Image* image, album->getImages() ) {
		QString fullPath = dirPath + QDir::separator() + image->getFilename();
		fullPath.replace(" ", "_");
		requestImage(image->getLargeURL(), albumId, image->getId(), false,
				fullPath);
	}

}

void Communicator::downloadImage(QString albumId,
		 QString imageId, QString fileName) {
	Album* album = m_albumsMap[albumId];

	if (album == NULL) {
		qWarning() << "Unknown album '" << albumId << "' at " << __FUNCTION__;
		return;
	}

	Image* image = findImage(albumId, imageId);
	if (image == NULL) {
		qWarning() << "Unknown image '" << imageId << "' at " << __FUNCTION__;
		return;
	}

	if (fileName.isEmpty()) {
		QDir home(QDir::homePath());
		QString dirPath = QDir::homePath() + SAVE_LOCATION + album->getTitle();
		dirPath.replace(" ", "_");
		home.mkpath(dirPath);

		QString fullPath = dirPath + QString("/") + image->getFilename();
		fullPath.replace(" ", "_");
		fileName = fullPath;
	}


	requestImage(image->getLargeURL(), albumId, image->getId(), false,
			fileName);
}

QList<QObject*> Communicator::getAlbums() const {

	QList<QObject*> infoList;
	foreach(Album* album, m_albumsMap.values() ) {
		infoList.append(album);
	}
	return infoList;
}

QList<QObject*> Communicator::searchAlbums(QString re) const {

	QList<QObject*> infoList;
	QRegExp regexp(re);
	foreach(Album* album, m_albumsMap.values() ) {
		if (album->getTitle().contains(re)) {
			infoList.append(album);
		}
	}
	return infoList;
}

QObject* Communicator::getAlbum(QString albumID) const {
	return m_albumsMap[albumID];
}

QList<QObject*> Communicator::getImages(QString albumID) const {
	QList<QObject*> infoList;
	Album* album = m_albumsMap[albumID];
	if (album != NULL) {
		foreach (Image* image, album->getImages()) {
			infoList.append(image);
		}
	}
	return infoList;
}

QObject* Communicator::getImage(QString albumID, QString imageID) const {
	Image* image = findImage(albumID, imageID);
	if (image == NULL)
		return NULL;

	return image;
}

QVariantList Communicator::searchImages( QString re) const {

	QVariantList infoList;
	QRegExp regexp(re);

	for (AlbumMap_t::const_iterator it = m_albumsMap.begin();
			it != m_albumsMap.end(); it++) {
		Album* album = m_albumsMap[it.key()];

		foreach (const Image* image, album->getImages() ) {

			if (image->getKeywords().contains(re)
					|| image->getCaption().contains(re)) {
				infoList.append(image);
			}
		}
	}
	return infoList;
}

void Communicator::requestData( QString method, QString APIKey,
		const QMap<QString, QString> *queryItems,
		const QMap<QString, QVariant> * responseProps) {

	QNetworkRequest request(
			QUrl("http://api.smugmug.com/services/api/json/1.2.2/"));
	request.setHeader(QNetworkRequest::ContentTypeHeader,
			"application/x-www-form-urlencoded");

	QByteArray data;
	QUrl params;

	params.addQueryItem("method", method); //always here
	params.addQueryItem("APIKey", APIKey);
	if (queryItems != NULL) {
		QMapIterator<QString, QString> iter(*queryItems);
		while (iter.hasNext()) {
			iter.next();
			params.addQueryItem(iter.key(), iter.value());
		}
	}
	data = params.encodedQuery();

	qDebug("Requesting");

	QNetworkReply * response = m_nwam->post(request, data);
	if (responseProps != NULL) {
		QMapIterator<QString, QVariant> iter(*responseProps);
		while (iter.hasNext()) {
			iter.next();
			response->setProperty(iter.key().toStdString().c_str(),
					iter.value());
		}
	}
}

void Communicator::requestImage(QString jpegUrl, QString albumID,
		QString imageID, bool isThumb, QString imageFullPath) {
	QUrl url(jpegUrl);
	if (url.isValid()) {
		qDebug() << "load " << url << " thumb " << isThumb;
		QNetworkRequest request;
		request.setUrl(url);
		QNetworkReply* reply = m_nwam->get(request);

		reply->setProperty(IS_IMAGE_REQEST, true);
		reply->setProperty(ALBUM_ID, albumID);
		reply->setProperty(IMAGE_ID, imageID);
		reply->setProperty(IS_THUMB, isThumb);

		if (!imageFullPath.isNull())
			reply->setProperty(IMAGE_FULL_PATH, imageFullPath);

	} else {
		qWarning() << "Invalid URL" << jpegUrl;
	}
}

void Communicator::loadExif(QString albumId, QString imageId) {

	Image* image = findImage(albumId, imageId);
	if (image == NULL)
		return;

	// request image Exif
	QMap<QString, QString> params;
	params["SessionID"] = m_session;
	params["ImageID"] = image->getId();
	params["ImageKey"] = image->getKey();

	QMap<QString, QVariant> responseParams;
	responseParams[IS_EXTIF_REQEST] = true;
	responseParams[ALBUM_ID] = albumId;
	responseParams[IMAGE_ID] = imageId;

	requestData(EXIF, API_KEY, &params, &responseParams);
}

void Communicator::finishedSlot(QNetworkReply* reply) {
	qDebug() << "Got reply";
	// Reading attributes of the reply
	// e.g. the HTTP status code
	QVariant statusCodeV = reply->attribute(
			QNetworkRequest::HttpStatusCodeAttribute);
	// Or the target URL if it was a redirect:
	QVariant redirectionTargetUrl = reply->attribute(
			QNetworkRequest::RedirectionTargetAttribute);
	// see CS001432 on how to handle this

	// no error received?
	if (reply->error() == QNetworkReply::NoError) {
		// handle based on response
		if (isImageResponse(reply))
			processImageResponse(reply);
		else
			processJsonResponse(reply);

	}
	// Some http error received
	else {
		qWarning() << "Network Error " << reply->errorString();
		emit error(reply->errorString());
	}

	reply->deleteLater();
}

bool Communicator::isImageResponse(QNetworkReply* reply) {

	// Image request has albumID, imageID and isThumb attached as dynamic properties
	QVariant prop = reply->property(IS_IMAGE_REQEST);
	return prop.isValid();
}

bool Communicator::storeImage(QString fullPath, QNetworkReply* reply) {
	QByteArray bytes = reply->readAll(); // bytes

	QFile file(fullPath);
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Could not open file " << fullPath << " for writing";
		emit error(tr("Could not open file %1 for writing").arg(fullPath));
		return false;
	}

	file.write(bytes);
	file.close();
	qDebug() << "jpeg downloaded" << fullPath;
	return true;
}

void Communicator::processImageResponse(QNetworkReply* reply) {

	// get dynamic properties associated with request
	QString albumID = reply->property(ALBUM_ID).toString();

	QVariant fullPathProp = reply->property(IMAGE_FULL_PATH);
	if (fullPathProp.isValid()) {
		// if full path is part of response - download only
		QString fullPath = reply->property(IMAGE_FULL_PATH).toString();
		if (!storeImage(fullPath, reply))
			return;
		emit imageSavedToShared(fullPath);

	} else {
		// otherwise download, update model and notify
		QString imageID = reply->property(IMAGE_ID).toString();
		bool isThumb = reply->property(IS_THUMB).toBool();

		QString filename = QString("image_%1_%2_%3.jpg").arg(albumID).arg(
				imageID).arg((isThumb ? "thumb" : "full"));

		QString fullPath = QDir::currentPath() + QString("/tmp/") + filename;
		QString localAsset = QUrl::fromLocalFile(fullPath).toString(QUrl::None);
		qDebug("downloaded image localAsset %s", qPrintable(localAsset));

		if (!storeImage(fullPath, reply))
			return;

		if (isThumb) {
			// try to save the image as galery thumb
			QString galeryFilename = QString("image_%1.jpg").arg(albumID);
			QFile galeryFile(galeryFilename);
			if (!galeryFile.exists()) {

			}
		}

		// update image data
		updateImage(albumID, imageID, localAsset, isThumb);

		// remember all filenames for cleanup
		m_localFilenames << fullPath;

		// finished - we are loaded
		emit imageLoaded(albumID, imageID, localAsset, isThumb);
	}
}

void Communicator::processJsonResponse(QNetworkReply* reply) {

	QByteArray bytes = reply->readAll(); // bytes
	QString urlString = reply->request().url().toString();

	QString string(bytes); // string
	qDebug() << "JsonResponse: " << string;
	QScriptValue sc(QString::null); //default null
	QScriptEngine engine;
	sc = engine.evaluate("( " + string + ")");

#ifdef DEBUG
	dumpJSONData(sc);
	qDebug() << queryProperty(sc, QString("method")).toString();
	QString session = queryProperty(sc, QString("Login/Session/id")).toString();
	qDebug() << session;
#endif
	emit jsonDataReceived(&sc, reply);
}

void Communicator::processJSONResponse(QScriptValue* sc, QNetworkReply* reply) {
	QString method = queryProperty(*sc, QString("method")).toString();
	if (method == LOGIN) {
		emit loginResponseReceived(sc);
	} else if (method == ALBUMS) {
		emit albumsResponseReceived(sc);
	} else if (method == IMAGES) {
		emit imagesResponseReceived(sc);
	} else if (method == EXIF) {
		emit exifResponseReceived(sc, reply);
	} else {
		qWarning() << "Unexpected JSON response for method " << method;
	}
}

bool Communicator::processResponseStat(QScriptValue *sc) {
	QString stat = queryProperty(*sc, QString("stat")).toString();
	if (stat == QString::null || stat != "ok") {
			QString message = queryProperty(*sc, QString("message")).toString();
			if ( message == QString::null ) {
				message = "Unknown error.";
			}
			emit error( message );
			return false;
	}
	return true;
}

void Communicator::processLoginResponse(QScriptValue *sc) {
	if (! processResponseStat(sc)) {
		return;
	}

	m_session = queryProperty(*sc, QString("Login/Session/id")).toString();
	if (m_session != QString::null) {
		QMap<QString, QString> params;
		params["NickName"] = m_username;
		params["SessionID"] = m_session;
		requestData(ALBUMS, API_KEY, &params);
	}
}

void Communicator::processAlbumsResponse(QScriptValue *sc) {
	if (! processResponseStat(sc)) {
			return;
	}

	QScriptValue albums = queryProperty(*sc, QString("Albums"));
	QScriptValueIterator it(albums);

	uint i = 0;
	while (it.hasNext()) {
		it.next();
		QScriptValue albumData = it.value();
		QString id;

		// skip items for which we could not obtain valid id
		QScriptValue idData = queryProperty(albumData, QString("id"));
		if (!idData.isValid())
			continue;

		QScriptValue categoryData = queryProperty(albumData,
				QString("Category"));
		QScriptValue subCategoryData = queryProperty(albumData,
				QString("SubCategory"));

		QSharedPointer<Category> category;
		if (categoryData.isValid()) {
			id = queryProperty(categoryData, QString("id")).toString();

			if (m_categoryMap.contains(id)) {
				category = m_categoryMap.value(id);
				//qDebug() << "found category " << id << " name " << category->getName();
			} else {
				category =
						QSharedPointer<Category>(
								new Category(id,
										queryProperty(categoryData,
												QString("Name")).toString()));
				//qDebug() << "added found category " << id << " name " << category->getName();
				m_categoryMap.insert(id, category);
			}
		} else {
			category = m_emptyCategory;
		}

		QSharedPointer<Category> subCategory;
		if (subCategoryData.isValid()) {
			id = queryProperty(subCategoryData, QString("id")).toString();
			if (m_subcategoryMap.contains(id)) {
				qDebug() << "found category " << id << " name "
						<< category->getName();
				subCategory = m_subcategoryMap.value(id);
			} else {
				subCategory =
						QSharedPointer<Category>(
								new Category(id,
										queryProperty(subCategoryData,
												QString("Name")).toString()));
				qDebug() << "added found category " << id << " name "
						<< category->getName();
				m_subcategoryMap.insert(id, subCategory);
			}
		} else {
			subCategory = m_emptyCategory;
		}

		id = idData.toString();
		Album*  album =	new Album(id,
						queryProperty(albumData, QString("Key")).toString(),
						queryProperty(albumData, QString("Title")).toString(),
						category, subCategory, this);
		m_albumsMap.insert(id, album);

		if (i++ > MAX_ALBUMS)
			break;
	}
	qDebug() << "Albums count = " << m_albumsMap.size();
	qDebug() << "Category count = " << m_categoryMap.size();
	qDebug() << "Subcategory count = " << m_subcategoryMap.size();

	// request images for known albums
	QMapIterator<QString, Album* > it_albums(m_albumsMap);
	while (it_albums.hasNext()) {
		it_albums.next();
		Album* a = it_albums.value();

		// request images
		QMap<QString, QString> params;
		params["SessionID"] = m_session;
		params["AlbumID"] = a->getId();
		params["AlbumKey"] = a->getKey();
		params["Heavy"] = "true";
		requestData(IMAGES, API_KEY, &params);
	}

	emit albumsLoaded();
}

void Communicator::processImagesResponse(QScriptValue *sc) {

	QString albumId = queryProperty(*sc, QString("Album/id")).toString();
	Album* album = m_albumsMap.value(albumId);
	if (! album ) {
		return;
	}

	QScriptValue images = queryProperty(*sc, QString("Album/Images"));
	int imageCount = queryProperty(*sc, QString("Album/ImageCount")).toInt32();
	album->setImageCount(imageCount);

	QScriptValueIterator it(images);

	int i;
	for (i = 0; it.hasNext() && i < imageCount; i++) {
		it.next();
		QScriptValue val = it.value();

		// skip items without id
		QScriptValue idData = queryProperty(val, QString("id"));
		if (!idData.isValid()) {
			qWarning("Album ID is missing. Skipping");
			continue;
		}

		QScriptValue keyData = queryProperty(val, QString("Key"));
		QScriptValue captionData = queryProperty(val, QString("Caption"));
		QScriptValue keywordsData = queryProperty(val, QString("Keywords"));

		QScriptValue thumbUrlData = queryProperty(val, QString("ThumbURL"));

		// fall back to middle or thumb data
		QScriptValue largeUrlData = queryProperty(val, QString("LargeURL"));
		if (!largeUrlData.isValid()) {
			largeUrlData = queryProperty(val, QString("MediumURL"));
		}
		if (!largeUrlData.isValid()) {
			largeUrlData = thumbUrlData;
		}

		QScriptValue filename = queryProperty(val, QString("FileName"));
		QScriptValue date = queryProperty(val, QString("Date"));

		Image* image = new Image(idData.toString(), keyData.toString(),
				captionData.toString(), keywordsData.toString(),
				thumbUrlData.toString(), largeUrlData.toString(),
				filename.toString(), date.toString(), album);

		album->appendImage(image);

		qDebug("Image %s (%s) appended", qPrintable(filename.toString()),
				qPrintable(thumbUrlData.toString()));

		// load thumbs for category
		if (i < 3) {
			loadImage(albumId, idData.toString(), true );
		}

		if (i > (int) MAX_IMAGES)
			break;
	}

	qDebug() << "album:" << albumId << " has " << i << " images";
}

void Communicator::processExifResponse(QScriptValue *sc, QNetworkReply* reply) {

	// get dynamic properties associated with request
	QString albumId = reply->property(ALBUM_ID).toString();
	QString imageId = reply->property(IMAGE_ID).toString();

	Image* image = findImage(albumId, imageId);
	if (image != NULL)
		return;

	image->setAperture(queryProperty(*sc, QString("Image/Aperture")).toString());
	image->setDateTime(queryProperty(*sc, QString("Image/DateTime")).toString());
	image->setExposureTime(queryProperty(*sc, QString("Image/ExposureTime")).toString());
	image->setFocalLength(queryProperty(*sc, QString("Image/FocalLength")).toString());
	image->setIso(queryProperty(*sc, QString("Image/ISO")).toString());

	emit exifLoaded(albumId, imageId);
}

void Communicator::dumpJSONData(QScriptValue value) {
	QScriptValueIterator it(value);
	while (it.hasNext()) {
		it.next();
		if (it.value().isObject()) {
			dumpJSONData(it.value());
			continue;
		}
		qDebug() << it.name() << ": " << it.value().toString();
	}
}

Image* Communicator::findImage(QString albumId, QString imageId) const {
	Album* album = m_albumsMap[albumId];
	if (album == NULL) {
		qWarning() << "Unknown album '" << albumId << "' at " << __FUNCTION__;
		return NULL;
	}

	Image* image = album->getImage(imageId);
	if (image == NULL) {
		qWarning() << "Unknown image '" << imageId << "' at " << __FUNCTION__;
		return NULL;
	}

	return image;
}

void Communicator::updateImage(QString albumId, QString imageId,
		QString assetPath, bool isThumb) {
	Image* image = findImage(albumId, imageId);
	if (image == NULL) {
		return;
	}

	image->setImage(assetPath, isThumb);

	if (isThumb) {
		Album* album = m_albumsMap[albumId];
		if (album->getThumbImage().isEmpty()) {
			album->setThumbImage(assetPath);
		}
	}
}

// delete temporary files
void Communicator::deleteLocalFiles() {

	QStringListIterator iter(m_localFilenames);
	while (iter.hasNext()) {

		// delete the file
		QString filename = iter.next();
		bool result = QFile(filename).remove();
		if (!result) {
			qWarning() << "Cannot delete temporary file: " << filename;
		} else {
			qDebug() << "Deleted temporary file: " << filename;
		}
	}

	m_localFilenames.clear();
}

QScriptValue Communicator::queryProperty(QScriptValue& value, QString path) {
	QStringList pathList = path.split(QRegExp("[/]"));
	QScriptValueIterator *it;
	QScriptValue val = value;

	for (int i = 0; i < pathList.length(); i++) {
		QString pathElement = pathList.at(i);
		it = new QScriptValueIterator(val);

		while (it->hasNext()) {
			it->next();
			QScriptValue structObject = it->value();
#ifdef DEBUG
			qDebug() << "Path element " << pathElement << " struct NAME: " << it->name();
#endif
			if (pathElement == it->name()) {
				if (i == pathList.length() - 1) {
					return structObject; //we are at the end of path, return value
				}
				if (structObject.isObject()) { //it has to be Object!
					val = structObject;
					break; //we are not at the end, found another part of path, let's search for next one
				} else {
					return QScriptValue(); //assert, we can not get here under normal conditions??
				}
			}
		}
		delete it;
	}
	qWarning() << "Could not found property at path " << path;
	return QScriptValue(); //not found
}

void Communicator::dump() {
	qDebug() << "Albums:";
	for (AlbumMap_t::const_iterator it = m_albumsMap.begin();
			it != m_albumsMap.end(); it++) {
		Album* album = m_albumsMap[it.key()];
		album->dump();
	}
}
