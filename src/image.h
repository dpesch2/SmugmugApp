#ifndef IMAGE_H
#define IMAGE_H

#include <QtCore/QObject>

class Image : public QObject
{
	Q_OBJECT

public:
    Image(QString id,
    	  QString key,
    	  QString caption,
    	  QString keywords,
    	  QString thumbURL,
    	  QString largeURL,
    	  QString filename,
    	  QString date,
    	  QObject* parent = NULL);

    Q_PROPERTY(QString id READ getId NOTIFY idChanged);
   // Q_PROPERTY(QString albumId READ getAlbumId NOTIFY albumIdChanged);
    Q_PROPERTY(QString caption READ getCaption NOTIFY captionChanged);
    Q_PROPERTY(QString keywords READ getKeywords NOTIFY keywordsChanged);
    Q_PROPERTY(QString dateProp READ getDate NOTIFY dateChanged);
    Q_PROPERTY(QString filename READ getFilename NOTIFY filenameChanged);
    Q_PROPERTY(QString thumbImage READ getThumbImage NOTIFY thumbImageChanged);
    Q_PROPERTY(QString thumbURL READ getThumbURL NOTIFY thumbURLChanged);
    Q_PROPERTY(QString largeImage READ getLargeImage NOTIFY largeImageChanged);
    Q_PROPERTY(QString largeURL READ getLargeURL NOTIFY largeURLChanged);

    Q_PROPERTY(QString aperture READ getAperture WRITE setAperture NOTIFY apertureChanged );
    Q_PROPERTY(QString exposureTime READ getExposureTime WRITE setExposureTime NOTIFY exposureTimeChanged );
    Q_PROPERTY(QString dateTime READ getDateTime WRITE setDateTime NOTIFY dateTimeChanged );
    Q_PROPERTY(QString focalLength READ getFocalLength WRITE setFocalLength NOTIFY focalLengthChanged );
    Q_PROPERTY(QString iso READ getIso WRITE setIso NOTIFY isoChanged );

    QString getId() const {return m_id;}
    QString getKey() const {return m_key;}
    QString getCaption() const {return m_caption;}
    QString getKeywords() const {return m_keywords;}
    QString getFilename() const {return m_filename;}
    QString getDate() const {return m_date;}
    QString getThumbURL() const {return m_thumbURL; }
    QString getLargeURL() const {return m_largeURL; }
    QString getThumbImage() const {return m_thumbImage; }
    QString getLargeImage() const {return m_largeImage; }
    QString getAperture() const { return m_aperture; }
    QString getExposureTime() const { return m_exposureTime; }
    QString getDateTime() const { return m_dateTime; }
    QString getFocalLength() const { return m_focalLength; }
    QString getIso() const { return m_iso; }

    Q_SIGNAL void idChanged();
    Q_SIGNAL void albumIdChanged();
    Q_SIGNAL void captionChanged();
    Q_SIGNAL void keywordsChanged();
    Q_SIGNAL void dateChanged();
    Q_SIGNAL void filenameChanged();
    Q_SIGNAL void thumbImageChanged();
    Q_SIGNAL void thumbURLChanged();
    Q_SIGNAL void largeImageChanged();
    Q_SIGNAL void largeURLChanged();

    Q_SIGNAL void apertureChanged();
    Q_SIGNAL void exposureTimeChanged();
    Q_SIGNAL void dateTimeChanged();
    Q_SIGNAL void focalLengthChanged();
    Q_SIGNAL void isoChanged();

    void setImage(QString fullPath, bool isThumb );
    void setAperture(QString val) { m_aperture = val; emit apertureChanged(); }
    void setExposureTime(QString val) { m_exposureTime = val; emit exposureTimeChanged(); }
    void setDateTime(QString val) { m_dateTime = val; emit dateTimeChanged(); }
    void setFocalLength(QString val) { m_focalLength = val; emit focalLengthChanged(); }
    void setIso(QString val) { m_iso = val; emit isoChanged(); }

    void dump() const;

private:
    QString m_id;
    QString m_key;
    QString m_caption;
    QString m_keywords;
    QString m_thumbURL;
    QString m_largeURL;
    QString m_thumbImage;
    QString m_largeImage;
    QString m_filename;
    QString m_date;

    // exif
    QString m_aperture;
    QString m_dateTime;
    QString m_exposureTime;
    QString m_focalLength;
    QString m_iso;
};

#endif // IMAGE_H
