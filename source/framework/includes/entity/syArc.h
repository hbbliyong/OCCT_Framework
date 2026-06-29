#pragma once

#include "entity/syCurve.h"
#include "property/property.h"
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Pnt.hxx>

namespace SongYun {

class SyArc : public CurveFeature
{
public:
	Property<SyArc, gp_Pnt> start{this, gp_Pnt(0, 0, 0)};
	Property<SyArc, gp_Pnt> mid{this, gp_Pnt(5, 5, 0)};
	Property<SyArc, gp_Pnt> end{this, gp_Pnt(10, 0, 0)};

	void recompute() override
	{
		GC_MakeArcOfCircle arcMaker(start.get(), mid.get(), end.get());
		if (arcMaker.IsDone())
			m_shape = BRepBuilderAPI_MakeEdge(arcMaker.Value()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Arc; }
};

} // namespace SongYun
