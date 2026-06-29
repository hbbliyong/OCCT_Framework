#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <Geom_SphericalSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace SongYun {

/// @brief 球面特征
class SySphericalSurface : public CadObject
{
public:
	Property<SySphericalSurface, double> radius{this, 10.0};
	Property<SySphericalSurface, gp_Pnt> center{this, gp_Pnt(0,0,0)};

	void recompute() override
	{
		gp_Ax3 ax(center.get(), gp_Dir(0,0,1));
		Handle(Geom_SphericalSurface) surf = new Geom_SphericalSurface(ax, radius.get());
		m_shape = BRepBuilderAPI_MakeFace(surf).Shape();
		clearDirty();
	}

	EntityType entityType() const override { return EntityType::SphericalSurface; }
};

} // namespace SongYun
