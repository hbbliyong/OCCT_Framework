#pragma once

#include "entity/syCurve.h"
#include "property/property.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Pnt.hxx>

namespace SongYun {

class SyLine : public CurveFeature
{
public:
	Property<SyLine, gp_Pnt> start{this, gp_Pnt(0, 0, 0)};
	Property<SyLine, gp_Pnt> end{this, gp_Pnt(10, 0, 0)};

	void recompute() override
	{
		m_shape = BRepBuilderAPI_MakeEdge(start.get(), end.get()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Line; }
};

} // namespace SongYun
