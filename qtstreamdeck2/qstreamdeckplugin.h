#pragma once

#include <QCoreApplication>
#include <QWebSocket>
#include <QJsonObject>

#include "qstreamdeckdeclares.h"
#include "qstreamdeckevent.h"

class QStreamDeckPlugin : public QObject {
Q_OBJECT
	friend class QStreamDeckDevice;

public:
	QStreamDeckPlugin();
	virtual ~QStreamDeckPlugin() = default;

protected:
	/**
 * Initializes the plugin from the command line arguments
 */
	virtual void init(QCoreApplication &app);

signals:
	/**
	 * Emitted whenever any message is received from the software.
	 * The messages are processed internally by this library.
	 * This signal is emitted always, regardless if the message is processed or not.
	 * @param message
	 */
	void softwareMessageReceived(const QJsonObject &message);

	/// Emited when any known event is received.
	void eventReceived(const QStreamDeckEvent &e);

protected:
	template<typename Action>
	void registerActionType(const QStreamDeckActionUID &actionUID) {
		Q_ASSERT(actionTypes_.contains(actionUID));
		actionTypes_[actionUID] = []() { return new Action(); };
	}

protected:
	virtual QStreamDeckDevice *createDevice() = 0;

private slots:
	void onWebSocketTextMessageReceived(const QString &text);
	void onSoftwareMessageReceived(const QJsonObject &message);
	void onEventReceived(const QStreamDeckEvent &e);

private:
	int port_ = -1;
	QString pluginUUID_;
	QString registerEvent_;
	QString info_;

private:
	QWebSocket websocket_;

private:
	std::unordered_map<QString, std::unique_ptr<QStreamDeckDevice>> devices_;
	QHash<QStreamDeckActionUID, std::function<QStreamDeckAction *()>> actionTypes_;

};

template<typename Device_>
class QStreamDeckPluginT : public QStreamDeckPlugin {

public:
	using Device = Device_;

protected:
	virtual QStreamDeckDevice *createDevice() final override {
		return new Device();
	}

};