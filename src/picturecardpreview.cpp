/*
 * picturecardpreview.cpp
 *
 *  Created on: 13.6.2014
 *      Author: Dan
 */

#include <bb/system/InvokeRequest>
#include "picturecardpreview.h"

const int TRIES = 25; // 5 s total
const int INTERVAL = 200; // ms

PictureCardPreview::PictureCardPreview(QObject* parent ) : QObject(parent) {
	m_tries = 0;
	m_timer = new QTimer(this);
	m_timer->setInterval(INTERVAL);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(tick()));
	m_invokeManager = new bb::system::InvokeManager(this);

}

void PictureCardPreview::tick() {
	m_tries--;
	if (m_tries <= 0) {
		m_timer->stop();
	}
	QFile file(m_fullName);
	if (file.exists()) {
		openPicturePreview(m_fullName);
	}
}

void PictureCardPreview::open(QString fullPath) {
	m_fullName = fullPath;
	QFile file(fullPath);
	if (file.exists()) {
		openPicturePreview(fullPath);
		return;
	}
	m_fullName = fullPath;
	m_tries = TRIES;
	if (! m_timer->isActive()) {
		m_timer->start();
	}
}

void PictureCardPreview::openPicturePreview(QString fullPath) {
	 // Create and send an invocation for the card target
	bb::system::InvokeRequest cardRequest;
	cardRequest.setTarget("sys.pictures.card.previewer");
	cardRequest.setAction("bb.action.VIEW");
	cardRequest.setMimeType("image/jpeg");
	cardRequest.setUri(QUrl::fromLocalFile(fullPath));
	m_invokeManager->invoke(cardRequest);
}



