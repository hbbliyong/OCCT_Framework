#pragma once

#include "core/CoreMacro.hpp"
#include "UIItems.h"
#include <QString>
#include <vector>
#include <memory>

namespace SongYun {

	/// @brief UI 分组构建器（链式 DSL）
	class UISection
	{
	public:
		explicit UISection(const QString& name) : m_name(name) {}

		const QString& name() const { return m_name; }
		const std::vector<std::unique_ptr<UIItemBase>>& items() const { return m_items; }

		UISection& checkBox(const char* key)
		{
			auto item = std::make_unique<BoolItem>();
			item->key = key;
			m_items.push_back(std::move(item));
			return *this;
		}

		UISection& doubleSpin(const char* key)
		{
			auto item = std::make_unique<DoubleItem>();
			item->key = key;
			m_items.push_back(std::move(item));
			return *this;
		}

		UISection& combo(const char* key)
		{
			auto item = std::make_unique<EnumItem>();
			item->key = key;
			m_items.push_back(std::move(item));
			return *this;
		}

		UISection& button(const char* key)
		{
			auto item = std::make_unique<ButtonItem>();
			item->key = key;
			m_items.push_back(std::move(item));
			return *this;
		}

		// 配置项（操作最后添加的 item）
		UISection& label(const QString& text)  { lastItem()->label = text; return *this; }
		UISection& range(double min, double max) {
			if (auto* d = dynamic_cast<DoubleItem*>(lastItem())) { d->min = min; d->max = max; }
			return *this;
		}
		UISection& step(double s) {
			if (auto* d = dynamic_cast<DoubleItem*>(lastItem())) { d->step = s; }
			return *this;
		}
		UISection& items(const QStringList& list) {
			if (auto* e = dynamic_cast<EnumItem*>(lastItem())) { e->values = list; }
			return *this;
		}
		UISection& text(const QString& t) { lastItem()->label = t; return *this; }

		template<typename Obj, typename Val>
		UISection& bind(Obj* obj, Val Obj::*member)
		{
			if (auto* d = dynamic_cast<DoubleItem*>(lastItem()))
			{
				d->getter = [obj, member]() { return static_cast<double>(obj->*member); };
				d->setter = [obj, member](double v) { obj->*member = static_cast<Val>(v); };
			}
			else if (auto* b = dynamic_cast<BoolItem*>(lastItem()))
			{
				b->getter = [obj, member]() { return obj->*member; };
				b->setter = [obj, member](bool v) { obj->*member = v; };
			}
			else if (auto* e = dynamic_cast<EnumItem*>(lastItem()))
			{
				e->getter = [obj, member]() { return QString::number(static_cast<int>(obj->*member)); };
				e->setter = [obj, member](const QString& v) { obj->*member = static_cast<Val>(v.toInt()); };
			}
			return *this;
		}

		UISection& onClick(std::function<void()> cb) {
			if (auto* b = dynamic_cast<ButtonItem*>(lastItem())) { b->onClick = std::move(cb); }
			return *this;
		}

	private:
		UIItemBase* lastItem() { return m_items.back().get(); }

		QString m_name;
		std::vector<std::unique_ptr<UIItemBase>> m_items;
	};

} // namespace SongYun
