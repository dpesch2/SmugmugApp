/*
 * settings.cpp

 *
 *  Created on: 13.6.2014
 *      Author: Dan
 */

#include <QDir>
#include <QTextStream>

#include "settings.h"


// where to store preferences
const QString PREFERENCES_PATH = "/preferences.conf";
const QString SEPARATOR = ":";
const QString USER = "user";

void Settings::load() {
	QString filename = QDir::homePath() + PREFERENCES_PATH;
	QFile myFile(filename);
	QString line;

	if (!myFile.open(QIODevice::ReadOnly)) {
		qWarning("Could not open file %s", qPrintable(filename));
		return;
	}

	m_settings.clear();
	QTextStream stream(&myFile);
	for(;;) {
		line = stream.readLine();
		if (line.isNull()) {
			break;
		}
		QStringList elements = line.split(SEPARATOR);
		if (elements.size() >= 2) {
			QString name = elements.first();
			elements.erase(elements.begin());
			QString value = elements.join(SEPARATOR);
			m_settings[name] = value;
		}
	}
}

Q_INVOKABLE void Settings::save() {
	QString filename = QDir::homePath() + PREFERENCES_PATH;
	QFile myFile(filename);

	if (!myFile.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
		qWarning("Could not open file %s", qPrintable(filename));
	}

	QTextStream stream(&myFile);
	foreach (const QString& setting, m_settings.keys()) {
		stream << setting << SEPARATOR << m_settings[setting] << endl;
	}
}

QString Settings::get(QString setting) const {
	return m_settings[setting];
}

void Settings::set(QString setting, QString value) {
	m_settings[setting] = value;
	emit settingChanged(setting);
}

QString Settings::getUser() const {
	return get(USER);
}

void Settings::setUser(QString user)  {
	set(USER, user);
	emit userChanged();
}





