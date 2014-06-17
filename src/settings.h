/*
 * settings.h
 *
 *  Created on: 13.6.2014
 *      Author: Dan
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtCore/QObject>
#include <QtScript/QScriptEngine>

class Settings : public QObject
{
    Q_OBJECT
    QMap<QString,QString> m_settings;

public:
    Settings(QObject* parent = NULL) : QObject(parent) {load(); }
    Q_PROPERTY(QString user READ getUser WRITE setUser NOTIFY userChanged);
    QString getUser() const;
    void setUser(QString user);

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();
    Q_INVOKABLE QString get(QString settingName) const;
    Q_INVOKABLE void set(QString settingName, QString value );

    Q_SIGNAL void settingChanged(QString settingName);
    Q_SIGNAL void userChanged();
};



#endif /* SETTINGS_H_ */
