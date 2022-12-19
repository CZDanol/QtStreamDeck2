#include "qstreamdeckevent.h"

#include <QMetaEnum>
#include <QJsonArray>

QStreamDeckEvent QStreamDeckEvent::fromMessage(const QJsonObject &json) {
	return QStreamDeckEvent{
		.json = json,
		.type = Type(QMetaEnum::fromType<Type>().keyToValue(json["event"].toString().toStdString().c_str())),
		.payload = json["payload"].toObject(),
	};
}