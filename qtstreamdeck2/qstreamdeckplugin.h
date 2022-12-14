#pragma once

#include <QCoreApplication>
#include <QWebSocket>
#include <QJsonObject>

#include "qstreamdeckdeclares.h"

class QStreamDeckPlugin : public QObject {
Q_OBJECT

public:
	QStreamDeckPlugin();
	virtual ~QStreamDeckPlugin() = default;

public:
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

protected:
	template<typename Action>
	void registerActionType(const QString &actionUID) {
		Q_ASSERT(actionTypes_.contains(actionUID));
		actionTypes_[actionUID] = []() { return new Action(); };
	}

protected:
	virtual QStreamDeckDevice *createDevice() = 0;

private slots:
	void onWebSocketTextMessageReceived(const QString &text);
	void onSoftwareMessageReceived(const QJsonObject &message);

private:
	int port_ = -1;
	QString pluginUUID_;
	QString registerEvent_;
	QString info_;

private:
	QWebSocket websocket_;

private:
	std::unordered_map<QString, std::unique_ptr<QStreamDeckDevice>> devices_;
	QHash<QString, std::function<QStreamDeckAction *()>> actionTypes_;

};

template<typename Device>
class QStreamDeckPluginT : public QStreamDeckPlugin {

protected:
	virtual QStreamDeckDevice *createDevice() final override {
		return new Device();
	}

};