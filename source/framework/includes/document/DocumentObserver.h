#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>

namespace SongYun {

	/// @brief 文档事件观察者 — View 实现此接口以响应数据变更
	class DocumentObserver
	{
	public:
		virtual SONGYUN_API ~DocumentObserver() = default;

		virtual SONGYUN_API void onObjectAdded(int id, const TopoDS_Shape& shape) {}
		virtual SONGYUN_API void onObjectRemoved(int id) {}
	};

} // namespace SongYun
