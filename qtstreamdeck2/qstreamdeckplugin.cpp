#include "qstreamdeckplugin.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QJsonDocument>
#include <QMetaEnum>

#include "qstreamdeckdevice.h"
#include "qstreamdeckaction.h"

QStreamDeckPlugin::QStreamDeckPlugin() {
	connect(&websocket_, &QWebSocket::textMessageReceived, this, &QStreamDeckPlugin::onWebSocketTextMessageReceived);
	connect(&websocket_, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [this](QAbstractSocket::SocketError e) {
		qWarning() << "Websocket error: " << websocket_.errorString();
	});

	connect(this, &QStreamDeckPlugin::softwareMessageReceived, this, &QStreamDeckPlugin::onSoftwareMessageReceived);
	connect(this, &QStreamDeckPlugin::eventReceived, this, &QStreamDeckPlugin::onEventReceived);
}

void QStreamDeckPlugin::init(QCoreApplication &app) {
	// Load up command line parameters
	{
		QCommandLineParser cmdParser;
		cmdParser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
		cmdParser.addHelpOption();

		QCommandLineOption portOption("port", "", "port", "0");
		cmdParser.addOption(portOption);

		QCommandLineOption pluginuuidOption("pluginUUID", "", "pluginUUID", "");
		cmdParser.addOption(pluginuuidOption);

		QCommandLineOption registerEventOption("registerEvent", "", "registerEvent", "");
		cmdParser.addOption(registerEventOption);

		QCommandLineOption infoOption("info", "", "info", "");
		cmdParser.addOption(infoOption);

		cmdParser.process(app);

		port_ = cmdParser.value(portOption).toInt();
		pluginUUID_ = cmdParser.value(pluginuuidOption);
		registerEvent_ = cmdParser.value(registerEventOption);
		info_ = cmdParser.value(infoOption);
	}

	// Connect to the SW
	websocket_.open(QStringLiteral("ws://localhost:%1").arg(port_));
}

void QStreamDeckPlugin::onWebSocketTextMessageReceived(const QString &text) {
	const QJsonObject json = QJsonDocument::fromJson(text.toUtf8()).object();
	emit softwareMessageReceived(json);
}

void QStreamDeckPlugin::onSoftwareMessageReceived(const QJsonObject &message) {
	if(const QStreamDeckEvent e = QStreamDeckEvent::fromMessage(message); e.type != QStreamDeckEvent::Type::unknown)
		emit eventReceived(e);
}

void QStreamDeckPlugin::onEventReceived(const QStreamDeckEvent &e) {
	using ET = QStreamDeckEvent::Type;

	const QStreamDeckDeviceContext deviceContext = e.json["device"].toString();

	switch(e.type) {

		case ET::deviceDidConnect: {
			auto device = std::unique_ptr<QStreamDeckDevice>(createDevice());
			device->init(this, deviceContext, e.json["deviceInfo"].toObject());
			devices_.insert_or_assign(deviceContext, std::move(device));
			return;
		}

		case ET::deviceDidDisconnect: {
			devices_.erase(deviceContext);
			return;
		}

		default:
			break;

	}

	if(!deviceContext.isEmpty()) {
		const auto dit = devices_.find(deviceContext);
		if(dit != devices_.end())
			emit dit->second->eventReceived(e);
		else
			qWarning() << "QtStreamDeck error: Expected device " << deviceContext << "to exist.";
	}
}
