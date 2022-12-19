#pragma once

#include <QObject>
#include <QJsonObject>
#include <QPoint>

#include "qstreamdeckdeclares.h"

struct QStreamDeckEvent {
Q_GADGET

public:
	static QStreamDeckEvent fromMessage(const QJsonObject &json);

public:
	enum class Type {
		unknown = -1,
		didReceiveSettings,
		didReceiveGlobalSettings,
		keyDown,
		keyUp,
		touchTap,
		dialPress,
		dialRotate,
		willAppear,
		willDisappear,
		titleParametersDidChange,
		deviceDidConnect,
		deviceDidDisconnect,
		applicationDidLaunch,
		applicationDidTerminate,
		systemDidWakeUp,
		propertyInspectorDidAppear,
		propertyInspectorDidDisappear,
		sendToPlugin
	};

	Q_ENUM(Type);

public:
	/// Original json object of the event
	QJsonObject json;

	Type type = Type::unknown;
	QJsonObject payload;

};