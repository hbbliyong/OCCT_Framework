#pragma once

#include "entity/cadObject.h"
#include "property/property.h"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Circ.hxx>

namespace SongYun{

class SyCircle : public CadObject
{
public:
    Property<SyCircle, double> radius{this, 10.0};
    Property<SyCircle, gp_Pnt> center{this, gp_Pnt(0,0,0)};
    Property<SyCircle, gp_Dir> normal{this, gp_Dir(0,0,1)};

    void recompute() override
    {
        gp_Ax2 ax(center.get(), normal.get());
        gp_Circ c(ax, radius.get());
        m_shape = BRepBuilderAPI_MakeEdge(c).Shape();
        clearDirty();
    }

    EntityType entityType() const override { return EntityType::Circle; }
};

} // namespace SongYun
