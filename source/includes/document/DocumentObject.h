#pragma once

#include <TopoDS_Shape.hxx>
#include <string>

class DocumentObject
{
public:
    DocumentObject(const TopoDS_Shape& shape, const std::string& name = "Object");

    const TopoDS_Shape& shape() const noexcept;
    const std::string& name() const noexcept;

private:
    TopoDS_Shape shape_;
    std::string name_;
};
