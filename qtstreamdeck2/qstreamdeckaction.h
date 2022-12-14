#pragma once

#include <QObject>

#include "qstreamdeckdeclares.h"

class QStreamDeckAction : public QObject {
Q_OBJECT

public:
	inline QStreamDeckDevice *device() {
		return device_;
	}

	inline const QStreamDeckActionContext &actionContext() const {
		return actionContext_;
	}

protected:
	/**
	 * This function is called when a property inspector is requested for the action.
	 * This library allows easy property inspector
	 */
	virtual void buildPropertyInspector(QStreamDeckPropertyInspectorBuilder &b) {}

	/**
	 * Notifies the system that something about the property inspector has been changed and that the inspector should be rebuilt.
	 * Has no effect if a property inspector for the action is not open.
	 */
	void updatePropertyInspector();

private:
	QStreamDeckDevice *device_ = nullptr;
	QStreamDeckActionContext actionContext_;

};

template<typename Device>
class QStreamDeckActionT : public QStreamDeckAction {

public:
	inline Device *device() {
		return static_cast<Device *>(QStreamDeckAction::device());
	}

};