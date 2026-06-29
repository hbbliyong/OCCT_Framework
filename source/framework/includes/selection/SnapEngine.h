#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <vector>

namespace SongYun {

	class View;

	/// @brief 捕捉点类型
	enum class SnapType { Vertex, Midpoint, Center };

	/// @brief 捕捉结果
	struct SnapResult
	{
		gp_Pnt   point;
		SnapType type = SnapType::Vertex;
		bool     valid = false;
	};

	/// @brief 特征点捕捉引擎 — 从当前可见的 AIS 对象中提取特征点
	class SnapEngine
	{
	public:
		SONGYUN_API SnapEngine();

		/// 查找距离鼠标最近的特征点（在屏幕坐标容差内）
		SONGYUN_API SnapResult findSnap(View* view,
			const std::vector<int>& screenPos, int tolerance = 12) const;

	private:
		std::vector<gp_Pnt> extractSnapPoints(void* ctx) const;
	};

} // namespace SongYun
