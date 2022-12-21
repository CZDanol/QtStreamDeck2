#pragma once

#include <QObject>
#include <QMetaEnum>

namespace QStreamDeckCommand {
	Q_NAMESPACE

	enum Command {
		setSettings,
		getSettings,
		setGlobalSettings,
		getGlobalSettings,
		openUrl,
		logMessage,

		setTitle,
		setImage,
		showAlert,
		showOk,
		setState,
		switchToProfile,
		sendToPropertyInspector,

		_cnt
	};

	Q_ENUM_NS(Command);
}

inline QString operator +(QStreamDeckCommand::Command c) {
	static const QVector<QString> h = []() {
		QVector<QString> r;
		const int cnt = int(QStreamDeckCommand::Command::_cnt);
		r.resize(cnt);

		const auto me = QMetaEnum::fromType<QStreamDeckCommand::Command>();
		for(int i = 0; i < cnt; i++)
			r[i] = me.valueToKey(i);

		return r;
	}();
	return h.value(c);
}