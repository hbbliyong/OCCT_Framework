#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆柱体特征
class SyCylinder : public CadObject
{
public:
	Property<SyCylinder, double> radius{this, 10.0};
	Property<SyCylinder, double> height{this, 30.0};
	Property<SyCylinder, gp_Pnt> center{this, gp_Pnt(0,0,0)};
	Property<SyCylinder, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax2 ax(center.get(), axis.get());
		m_shape = BRepPrimAPI_MakeCylinder(ax, radius.get(), height.get()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Cylinder; }
};

} // namespace SongYun
