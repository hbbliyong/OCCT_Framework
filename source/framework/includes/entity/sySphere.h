#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepPrimAPI_MakeSphere.hxx>
#include <gp_Pnt.hxx>

namespace SongYun {

/// @brief 球体特征
class SySphere : public CadObject
{
public:
	Property<SySphere, double> radius{this, 10.0};
	Property<SySphere, gp_Pnt> center{this, gp_Pnt(0,0,0)};

	void recompute() override
	{
		m_shape = BRepPrimAPI_MakeSphere(center.get(), radius.get()).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::Sphere; }
};

} // namespace SongYun
