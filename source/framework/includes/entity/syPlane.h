#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 平面特征
class SyPlane : public CadObject
{
public:
	Property<SyPlane, gp_Pnt> origin{this, gp_Pnt(0,0,0)};
	Property<SyPlane, gp_Dir> normal{this, gp_Dir(0,0,1)};

	void recompute() override
	{
		gp_Pln pln(origin.get(), normal.get());
		m_shape = BRepBuilderAPI_MakeFace(pln).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Plane; }
};

} // namespace SongYun
