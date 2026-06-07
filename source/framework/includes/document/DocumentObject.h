#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>
#include <string>

namespace SongYun {

class DocumentObject
{
public:
    SONGYUN_API DocumentObject(const TopoDS_Shape& shape, const std::string& name = "Object");

    SONGYUN_API const TopoDS_Shape& shape() const noexcept;
    SONGYUN_API const std::string& name() const noexcept;

private:
    TopoDS_Shape shape_;
    std::string name_;
};

} // namespace SongYun
