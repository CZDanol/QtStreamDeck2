#include "qstreamdeckdevice.h"

void QStreamDeckDevice::init(QStreamDeckPlugin *plugin, const QString &deviceContext, const QJsonObject &deviceInfo) {
	plugin_ = plugin;
	deviceContext_ = deviceContext;
	deviceInfo_ = deviceInfo;
}
