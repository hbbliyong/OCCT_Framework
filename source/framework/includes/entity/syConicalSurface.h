#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <Geom_ConicalSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆锥曲面特征
class SyConicalSurface : public CadObject
{
public:
	Property<SyConicalSurface, double> radius{this, 10.0};
	Property<SyConicalSurface, double> semiAngle{this, 0.5};    // 半顶角（弧度）
	Property<SyConicalSurface, gp_Pnt> apex{this, gp_Pnt(0,0,0)};
	Property<SyConicalSurface, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax3 ax(apex.get(), axis.get());
		Handle(Geom_ConicalSurface) surf = new Geom_ConicalSurface(ax, semiAngle.get(), radius.get());
		m_shape = BRepBuilderAPI_MakeFace(surf).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::ConicalSurface; }
};

} // namespace SongYun
