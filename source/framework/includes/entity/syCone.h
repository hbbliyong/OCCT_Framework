#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepPrimAPI_MakeCone.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆锥体特征
class SyCone : public CadObject
{
public:
	Property<SyCone, double> radius1{this, 10.0};
	Property<SyCone, double> radius2{this, 5.0};
	Property<SyCone, double> height{this, 30.0};
	Property<SyCone, gp_Pnt> center{this, gp_Pnt(0,0,0)};
	Property<SyCone, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax2 ax(center.get(), axis.get());
		m_shape = BRepPrimAPI_MakeCone(ax, radius1.get(), radius2.get(), height.get()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Cone; }
};

} // namespace SongYun
