#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <Geom_CylindricalSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆柱曲面特征
class SyCylindricalSurface : public CadObject
{
public:
	Property<SyCylindricalSurface, double> radius{this, 10.0};
	Property<SyCylindricalSurface, gp_Pnt> origin{this, gp_Pnt(0,0,0)};
	Property<SyCylindricalSurface, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax3 ax(origin.get(), axis.get());
		Handle(Geom_CylindricalSurface) surf = new Geom_CylindricalSurface(ax, radius.get());
		m_shape = BRepBuilderAPI_MakeFace(surf).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::CylindricalSurface; }
};

} // namespace SongYun
