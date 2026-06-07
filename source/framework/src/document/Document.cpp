#include "document/Document.h"
namespace SongYun {
	Document::Document(const std::string& name)
		: name_(name)
	{}

	const std::string& Document::name() const noexcept
	{
		return name_;
	}

	void Document::setName(const std::string& name)
	{
		name_ = name;
	}

	bool Document::load(const std::string& /*path*/)
	{
		return false;
	}

	bool Document::save(const std::string& /*path*/) const
	{
		return false;
	}

	void Document::addObject(const std::shared_ptr<DocumentObject>& object)
	{
		if (object)
			objects_.push_back(object);
	}

	const std::vector<std::shared_ptr<DocumentObject>>& Document::objects() const noexcept
	{
		return objects_;
	}
}