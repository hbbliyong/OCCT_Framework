#include "geometry/SweepBuilder.h"
namespace SongYun
{
	TopoDS_Shape SweepBuilder::build(const TopoDS_Shape& profile, const TopoDS_Shape&)
	{
		return profile;
	}
}