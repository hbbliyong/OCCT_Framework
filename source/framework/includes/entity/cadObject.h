#pragma once

#include "property/propertyBase.h"
#include <TopoDS_Shape.hxx>
#include <vector>
#include <functional>

// 前向声明 TDF
class TDF_Label;

namespace SongYun {

	class CadObject;

	/// @brief 变更集 — commit 后通过 EventBus 通知 View
	struct ChangeSet
	{
		std::vector<CadObject*> added;
		std::vector<CadObject*> removed;
		std::vector<CadObject*> modified;
		bool empty() const { return added.empty() && removed.empty() && modified.empty(); }
	};

	/// @brief 实体类型枚举
	enum class EntityType
	{
		Circle, Line, Arc, Polyline,
		// 曲面
		Plane, CylindricalSurface, ConicalSurface, SphericalSurface, ToroidalSurface,
		// 体
		Cylinder, Cone, Sphere, Torus
	};

	class CadObject
	{
	public:
		enum class State { Clean, Modified, New, Deleted };

		virtual ~CadObject() = default;

		// ---- 状态 ----
		const TopoDS_Shape& shape() const { return m_shape; }
		State state() const { return m_state; }

		// ---- dirty 管线 ----
		void markDirty()
		{
			if (m_state == State::Clean || m_state == State::New)
			{
				m_state = State::Modified;
				if (onDirty) onDirty(this);
			}
		}

		void clearDirty()
		{
			m_state = State::Clean;
			for (auto* p : m_properties) p->clearDirty();
		}

		void attach(PropertyBase* prop) { m_properties.push_back(prop); }

		/// 根据属性重建几何 → 写入 m_shape，调用 clearDirty
		virtual void recompute() = 0;

		/// 实体类型
		virtual EntityType entityType() const = 0;

		/// 序列化属性到 TDF（子类实现）
		virtual void saveToTDF(void* label) const {}
		/// 从 TDF 反序列化属性（子类实现）
		virtual void loadFromTDF(void* label) {}

	public:
		std::function<void(CadObject*)> onDirty;
		std::function<void()>           onPropertyChanged;

	protected:
		TopoDS_Shape m_shape;
		State m_state = State::New;

	private:
		std::vector<PropertyBase*> m_properties;
	};

} // namespace SongYun
