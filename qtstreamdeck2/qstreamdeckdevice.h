#pragma once

#include <QObject>
#include <QJsonObject>

#include "qstreamdeckdeclares.h"

class QStreamDeckDevice : public QObject {
Q_OBJECT
	friend class QStreamDeckPlugin;

public:
	virtual ~QStreamDeckDevice() = default;

public:
	inline QStreamDeckPlugin *plugin() {
		return plugin_;
	}

	inline const QStreamDeckDeviceContext &deviceContext() const {
		return deviceContext_;
	}

	inline const QJsonObject &deviceInfo() const {
		return deviceInfo_;
	}

protected:
	virtual void init(QStreamDeckPlugin *plugin, const QStreamDeckDeviceContext &deviceContext, const QJsonObject &deviceInfo);

private:
	QStreamDeckPlugin *plugin_ = nullptr;
	QStreamDeckDeviceContext deviceContext_;
	QJsonObject deviceInfo_;

private:
	QHash<QString

};

template<typename Plugin>
class QStreamDeckDeviceT : public QStreamDeckDevice {

public:
	inline Plugin *plugin() {
		return static_cast<Plugin *>(QStreamDeckDevice::plugin());
	}

};