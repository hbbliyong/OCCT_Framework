#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Pnt.hxx>
#include <vector>

namespace SongYun {

class SyPolyline : public CadObject
{
public:
	Property<SyPolyline, bool> closed{this, false};

	void addPoint(const gp_Pnt& pt) { m_points.push_back(pt); markDirty(); }
	void clearPoints()              { m_points.clear(); markDirty(); }
	size_t pointCount() const                { return m_points.size(); }
	const std::vector<gp_Pnt>& points() const { return m_points; }

	void recompute() override
	{
		if (m_points.size() < 2) { clearDirty(); return; }
		BRepBuilderAPI_MakeWire wm;
		for (size_t i = 1; i < m_points.size(); ++i)
			wm.Add(BRepBuilderAPI_MakeEdge(m_points[i - 1], m_points[i]));
		if (closed.get() && m_points.size() >= 3)
			wm.Add(BRepBuilderAPI_MakeEdge(m_points.back(), m_points.front()));
		m_shape = wm.Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Polyline; }

private:
	std::vector<gp_Pnt> m_points;
};

} // namespace SongYun
