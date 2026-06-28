#include "math/EarClipping.h"

#include <Geom_Plane.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>

namespace SongYun {
namespace Math {

	std::vector<Triangle> triangulate(const std::vector<gp_Pnt>& polygon)
	{
		// ================================================================
		// 耳切法三角剖分 — STUB（用户填写算法实现）
		//
		// 输入 polygon: 闭合多边形的顶点（已去掉首尾重复）
		// 返回: 三角形列表，每个 Triangle = {i0, i1, i2} 为 polygon 下标
		//
		// 典型步骤：
		// 1. 用 polygon 前三个不共线点求平面法向量
		// 2. 将所有点投影到 2D
		// 3. 耳切法找出所有耳朵，逐个切出三角形
		// 4. 返回三角形索引
		// ================================================================

		std::vector<Triangle> result;

		// TODO: 实现耳切法
		// 临时：简单生成三角形（仅用于框架测试，实际替换）
		if (polygon.size() >= 3)
		{
			for (size_t i = 2; i < polygon.size(); ++i)
				result.push_back({ 0, static_cast<int>(i - 1), static_cast<int>(i) });
		}

		return result;
	}

} // namespace Math
} // namespace SongYun
