#pragma once

#include "property/propertyBase.h"

namespace SongYun {

template<class Owner, class T>
class Property : public PropertyBase
{
public:
	Property(Owner* owner, T v = {})
		: m_owner(owner), m_value(std::move(v))
	{
		m_owner->attach(this);
	}

	const T& get() const { return m_value; }

	void set(const T& v)
	{
		m_value = v;
		m_dirty = true;

		// 1. 先标记 owner 脏（markDirty 内部去重）
		m_owner->markDirty();

		// 2. 通知属性变更（方案A：无参回调，外部 lambda 自行捕获旧/新值）
		if (m_owner->onPropertyChanged)
			m_owner->onPropertyChanged();
	}

	bool isDirty() const override { return m_dirty; }
	void clearDirty() override { m_dirty = false; }

private:
	Owner* m_owner;
	T m_value{};
	bool m_dirty = false;
};

} // namespace SongYun
