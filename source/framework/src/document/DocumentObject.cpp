#include "document/DocumentObject.h"
namespace SongYun {
	DocumentObject::DocumentObject(const TopoDS_Shape& shape, const std::string& name)
		: shape_(shape), name_(name)
	{}

	const TopoDS_Shape& DocumentObject::shape() const noexcept
	{
		return shape_;
	}

	const std::string& DocumentObject::name() const noexcept
	{
		return name_;
	}
}