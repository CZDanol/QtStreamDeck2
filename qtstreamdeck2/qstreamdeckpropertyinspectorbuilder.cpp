#include "qstreamdeckpropertyinspectorbuilder.h"

QStreamDeckPropertyInspectorBuilder::QStreamDeckPropertyInspectorBuilder(QStreamDeckAction *action) : action_(action) {

}

QString QStreamDeckPropertyInspectorBuilder::buildHTML() const {
	QString r;
	for(const auto &item: items_)
		item->buildHTML(r);
	return r;
}
