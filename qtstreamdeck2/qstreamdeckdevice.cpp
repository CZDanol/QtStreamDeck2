#include "qstreamdeckdevice.h"

#include "qstreamdeckaction.h"
#include "qstreamdeckplugin.h"

QStreamDeckDevice::QStreamDeckDevice() {
	connect(this, &QStreamDeckDevice::eventReceived, this, &QStreamDeckDevice::onEventReceived);
}

QStreamDeckDevice::~QStreamDeckDevice() {

}

void QStreamDeckDevice::init(QStreamDeckPlugin *plugin, const QString &deviceContext, const QJsonObject &deviceInfo) {
	plugin_ = plugin;
	deviceContext_ = deviceContext;
	deviceInfo_ = deviceInfo;
	deviceType_ = DeviceType(deviceInfo["type"].toInt());

	const auto sizeJson = deviceInfo["size"];
	size_ = QPoint(sizeJson["columns"].toInt(), sizeJson["rows"].toInt());
}

void QStreamDeckDevice::onEventReceived(const QStreamDeckEvent &e) {
	using ET = QStreamDeckEvent::EventType;

	const QStreamDeckActionContext actionContext = e.json["context"].toString();

	switch(e.eventType) {

		case ET::willAppear: {
			const QString actionType = e.json["action"].toString();
			auto atit = plugin_->actionTypes_.find(actionType);
			if(atit == plugin_->actionTypes_.end()) {
				qWarning() << "QtStreamDeck error: Unknown action type" << actionType;
				return;
			}

			std::unique_ptr<QStreamDeckAction> action(atit.value()());
			action->init(this, e);
			actions_.insert_or_assign(actionContext, std::move(action));
			return;
		}

		case ET::willDisappear: {
			actions_.erase(actionContext);
			return;
		}

		default:
			break;

	}

	if(!actionContext.isEmpty()) {
		const auto ait = actions_.find(actionContext);
		if(ait != actions_.end())
			emit ait->second->eventReceived(e);
		else
			qWarning() << "QtStreamDeck error: Expected action " << actionContext << "to exist.";
	}
}
