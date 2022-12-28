#pragma once

#include <QString>
#include <QMap>

#include "qstreamdeckdeclares.h"

struct QStreamDeckPropertyInspectorItems {
	friend class QStreamDeckPropertyInspectorBuilder;

protected:
	static inline QString escapeQuotes(const QString &str) {
		QString s = str;
		s.replace("\"", "\\\"");
		return s;
	}

	struct Item {

	public:
		virtual ~Item() = default;
		virtual void buildHTML(QString &sink) const = 0;

		virtual QStreamDeckPropertyInspectorCallback callback() const { return {}; }

	public:
		QString id;
	};
	using ItemUPtr = std::unique_ptr<Item>;

	template<typename T_>
	struct ItemT : public Item {
		using T = T_;

		inline T &setID(const QString &set) {
			id = set;
			return ref();
		}

	protected:
		inline T &ref() {
			return static_cast<T &>(*this);
		}

	};
	template<typename T>
	struct ElementItemT : public ItemT<T> {
		QString label;
		QMap<QString, QString> properties;

		inline T &setLabel(const QString &set) {
			label = set;
			return this->ref();
		}

		inline T &setProperty(const QString &property, const QString &value) {
			properties.insert(property, value);
			return this->ref();
		}

	protected:
		void buildPre(QString &sink) const {
			sink += QStringLiteral("<div class=\"sdpi-item\"><div class=\"sdpi-item-label\">%1</div>").arg(label);
		}

		void buildPost(QString &sink) const {
			sink += "</div>";
		}

		void buildProperties(QString &sink) const {
			sink += QStringLiteral(" id=\"pitem_%1\"").arg(escapeQuotes(this->id));

			for(auto it = properties.begin(), e = properties.end(); it != e; it++)
				sink += QStringLiteral(" %1=\"%2\"").arg(it.key(), escapeQuotes(it.value()));
		}
	};

public:
	struct Item_HTML final : public ItemT<Item_HTML> {
		QString html;

		inline T &setHTML(const QString &set) {
			html = set;
			return *this;
		}

		void buildHTML(QString &sink) const override {
			sink += html;
		}
	};
	struct Item_LineEdit final : public ElementItemT<Item_LineEdit> {
		inline T &setDefaultValue(const QString &set) {
			setProperty("value", set);
			return this->ref();
		}

		inline T &setPlaceholder(const QString &set) {
			setProperty("placeholder", set);
			return this->ref();
		}

		void buildHTML(QString &sink) const override {
			sink += QStringLiteral("<input type=\"text\" class=\"spdi-item-value\"");
			this->buildProperties(sink);
			sink += ">";
		}
	};
};