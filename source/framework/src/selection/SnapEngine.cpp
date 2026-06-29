#include "selection/SnapEngine.h"
#include "view/View.h"

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_Shape.hxx>
#include <NCollection_List.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <V3d_View.hxx>
#include <limits>
#include <cmath>

namespace SongYun {

	SnapEngine::SnapEngine() = default;

	// ================================================================
	// 从所有已显示对象中提取顶点 + 中点
	// ================================================================
	std::vector<gp_Pnt> SnapEngine::extractSnapPoints(void* ctxPtr) const
	{
		std::vector<gp_Pnt> pts;
		auto* ctx = static_cast<AIS_InteractiveContext*>(ctxPtr);
		if (!ctx) return pts;

		NCollection_List<Handle(AIS_InteractiveObject)> dispList;
		ctx->DisplayedObjects(dispList);

		for (auto it = dispList.cbegin(); it != dispList.cend(); ++it)
		{
			Handle(AIS_InteractiveObject) aisObj = *it;
			if (aisObj.IsNull()) continue;

			// 排除辅助对象
			if (!aisObj->IsKind(STANDARD_TYPE(AIS_Shape))) continue;

			auto* shapeObj = dynamic_cast<AIS_Shape*>(aisObj.get());
			if (!shapeObj) continue;

			TopoDS_Shape shape = shapeObj->Shape();

			// 顶点
			for (TopExp_Explorer exp(shape, TopAbs_VERTEX); exp.More(); exp.Next())
			{
				pts.push_back(BRep_Tool::Pnt(TopoDS::Vertex(exp.Current())));
			}

			// 边中点
			for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next())
			{
				BRepAdaptor_Curve curve(TopoDS::Edge(exp.Current()));
				double um = (curve.FirstParameter() + curve.LastParameter()) * 0.5;
				gp_Pnt mid;
				curve.D0(um, mid);
				pts.push_back(mid);
			}
		}

		return pts;
	}

	// ================================================================
	// 查找最近捕捉点
	// ================================================================
	SnapResult SnapEngine::findSnap(View* view,
		const std::vector<int>& screenPos, int tolerance) const
	{
		SnapResult result;
		if (!view) return result;

		auto ctx = view->getContext();
		if (ctx.IsNull()) return result;

		int sx = screenPos[0];
		int sy = screenPos[1];

		auto candPoints = extractSnapPoints(ctx.operator->());

		double bestDist = std::numeric_limits<double>::max();
		gp_Pnt bestPnt;

		for (const auto& pt : candPoints)
		{
			int px = 0, py = 0;
			view->getV3dView()->Convert(pt.X(), pt.Y(), pt.Z(), px, py);

			double dx = static_cast<double>(px) - static_cast<double>(sx);
			double dy = static_cast<double>(py) - static_cast<double>(sy);
			double dist = std::sqrt(dx * dx + dy * dy);

			if (dist < tolerance && dist < bestDist)
			{
				bestDist = dist;
				bestPnt = pt;
			}
		}

		if (bestDist < tolerance)
		{
			result.point = bestPnt;
			result.valid = true;
		}

		return result;
	}

} // namespace SongYun
