#include "cmds/TriangulatePolylineCommand.h"

#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopoDS_Edge.hxx>
#include "command/CommandContext.h"
#include "selection/SelectionManager.h"
#include "view/ViewManager.h"
#include "view/View.h"
#include "document/DocumentManager.h"
#include "common/StatusService.h"
#include "math/EarClipping.h"

namespace Samples
{
	static std::vector<gp_Pnt> extractPoints(const TopoDS_Wire& wire)
	{
		std::vector<gp_Pnt> pts;
		for (TopExp_Explorer exp(wire, TopAbs_EDGE); exp.More(); exp.Next())
		{
			const auto& edge = TopoDS::Edge(exp.Current());
			TopoDS_Vertex v1, v2;
			TopExp::Vertices(edge, v1, v2);
			pts.push_back(BRep_Tool::Pnt(v1));
		}
		return pts;
	}

	static bool isClosedWire(const TopoDS_Wire& wire)
	{
		return BRep_Tool::IsClosed(wire);
	}

	/// 判断两个索引是否在原多边形中相邻（包括首尾）
	static bool isAdjacent(int i, int j, int n)
	{
		int diff = std::abs(i - j);
		return diff == 1 || diff == n - 1;
	}

	/// 从三角形中提取不在原多边形边界上的「新边」
	static std::vector<std::pair<int, int>> newEdges(
		const std::vector<SongYun::Math::Triangle>& tris, int n)
	{
		std::vector<std::pair<int, int>> edges;
		for (auto& t : tris)
		{
			for (int k = 0; k < 3; ++k)
			{
				int a = t[k], b = t[(k + 1) % 3];
				if (!isAdjacent(a, b, n))
					edges.push_back({ a, b });
			}
		}
		return edges;
	}

	bool TriangulatePolylineCommand::execute()
	{
		auto& svc = this->context().statusService();
		auto* view = this->context().viewManager().activeView();

		// 1. 拾取闭合 polyline
		auto pick = this->context().selectionManager().PickObject("Select a closed polyline.");
		if (!pick.has_value() || pick->shape.IsNull())
		{
			svc.showMessage("No object selected. Cancelled.");
			return false;
		}

		TopoDS_Wire wire;
		if (pick->shape.ShapeType() == TopAbs_WIRE)
			wire = TopoDS::Wire(pick->shape);
		else
		{
			svc.showMessage("Selected object is not a wire. Cancelled.");
			return false;
		}

		if (!isClosedWire(wire))
		{
			svc.showMessage("Selected polyline is not closed. Cancelled.");
			return false;
		}

		// 2. 提取顶点
		auto pts = extractPoints(wire);
		if (pts.size() < 3)
		{
			svc.showMessage("Polyline has fewer than 3 vertices.");
			return false;
		}

		// 3. 耳切法三角剖分
		auto tris = SongYun::Math::triangulate(pts);
		if (tris.empty())
		{
			svc.showMessage("Triangulation returned no triangles.");
			return false;
		}

		// 4. 构建新增边（对角线），显示为红色
		int n = static_cast<int>(pts.size());
		auto diagonals = newEdges(tris, n);

		if (diagonals.empty())
		{
			svc.showMessage("No new edges to display (polygon is already a triangle?).");
			return true;
		}

		// 构建所有新边为 TopoDS 并放入临时 Compound
		BRep_Builder builder;
		TopoDS_Compound compound;
		builder.MakeCompound(compound);
		for (auto& [i, j] : diagonals)
		{
			auto edge = BRepBuilderAPI_MakeEdge(pts[i], pts[j]).Edge();
			builder.Add(compound, edge);
		}

		// 红色临时预览
		if (view)
			view->showTemporaryShape(compound);

		svc.showMessage(QString("Triangulated: %1 triangles, %2 new edges (red).")
			.arg(tris.size()).arg(diagonals.size()));
		return true;
	}

	REGISTER_COMMAND(TriangulatePolylineCommand, "Samples.TriangulatePolyline");

} // namespace Samples
