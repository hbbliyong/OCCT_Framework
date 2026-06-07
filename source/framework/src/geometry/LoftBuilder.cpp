#include "geometry/LoftBuilder.h"
namespace SongYun
{
	TopoDS_Shape LoftBuilder::build(const std::vector<TopoDS_Shape>& profiles)
	{
		if (profiles.empty())
			return TopoDS_Shape();
		return profiles.front();
	}
}