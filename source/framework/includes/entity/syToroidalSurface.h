#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <Geom_ToroidalSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆环面特征
class SyToroidalSurface : public CadObject
{
public:
	Property<SyToroidalSurface, double> majorRadius{this, 20.0};
	Property<SyToroidalSurface, double> minorRadius{this, 5.0};
	Property<SyToroidalSurface, gp_Pnt> center{this, gp_Pnt(0,0,0)};
	Property<SyToroidalSurface, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax3 ax(center.get(), axis.get());
		Handle(Geom_ToroidalSurface) surf = new Geom_ToroidalSurface(ax, majorRadius.get(), minorRadius.get());
		m_shape = BRepBuilderAPI_MakeFace(surf).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::ToroidalSurface; }
};

} // namespace SongYun
