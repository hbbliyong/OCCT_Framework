#pragma once

#include "core/CoreMacro.hpp"
#include <gp_Pnt.hxx>
#include <vector>
#include <array>

namespace SongYun {
namespace Math {

	/// 三角形：三个顶点在 points 数组中的索引
	using Triangle = std::array<int, 3>;

	/// @brief 耳切法三角剖分
	///
	/// 输入：闭合简单多边形的顶点列表（首尾不重复，即 points[0] == points[n-1] 无需传入）
	/// 输出：三角形索引列表
	///
	/// 调用前会先将 3D 点投影到多边形平面（2D），内部执行耳切法，返回三角形索引。
	///
	/// TODO: 用户实现耳切法算法
	SONGYUN_API std::vector<Triangle> triangulate(const std::vector<gp_Pnt>& polygon);

} // namespace Math
} // namespace SongYun
