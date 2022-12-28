#pragma once

#include <QString>
#include <QList>
#include <QMap>

#include "qstreamdeckdeclares.h"
#include "qstreamdeckpropertyinspectoritems.h"

class QStreamDeckPropertyInspectorBuilder {
	using I = QStreamDeckPropertyInspectorItems;

public:
	inline I::Item_HTML &addHTML(const QString &html) {
		auto e = new I::Item_HTML;
		items_.emplace_back(e);
		return e->setHTML(html);
	}

	inline I::Item_LineEdit &addLineEdit(const QString &id, const QString &label) {
		auto e = new I::Item_LineEdit;
		items_.emplace_back(e);
		return e->setID(id).setLabel(label);
	}

public:
	QStreamDeckPropertyInspectorBuilder(QStreamDeckAction *action);

public:
	QString buildHTML() const;

private:
	QStreamDeckAction *action_ = nullptr;
	std::vector<ItemUPtr> items_;

};
