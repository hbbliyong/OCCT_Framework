#include "geometry/BooleanBuilder.h"

TopoDS_Shape BooleanBuilder::cut(const TopoDS_Shape &left, const TopoDS_Shape &)
{
    return left;
}

TopoDS_Shape BooleanBuilder::fuse(const TopoDS_Shape &left, const TopoDS_Shape &)
{
    return left;
}

TopoDS_Shape BooleanBuilder::common(const TopoDS_Shape &left, const TopoDS_Shape &)
{
    return left;
}
