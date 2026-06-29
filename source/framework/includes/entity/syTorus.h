#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepPrimAPI_MakeTorus.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 圆环体特征
class SyTorus : public CadObject
{
public:
	Property<SyTorus, double> majorRadius{this, 20.0};
	Property<SyTorus, double> minorRadius{this, 5.0};
	Property<SyTorus, gp_Pnt> center{this, gp_Pnt(0,0,0)};
	Property<SyTorus, gp_Dir> axis{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Ax2 ax(center.get(), axis.get());
		m_shape = BRepPrimAPI_MakeTorus(ax, majorRadius.get(), minorRadius.get()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Torus; }
};

} // namespace SongYun
