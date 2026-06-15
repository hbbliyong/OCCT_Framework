#pragma once

#include "core/CoreMacro.hpp"
#include <string>
#include <vector>
#include <memory>
#include <TopoDS_Shape.hxx>
namespace SongYun {
	class Document
	{
	public:
		SONGYUN_API explicit Document(const std::string& name = "Untitled");
		SONGYUN_API virtual ~Document();

		SONGYUN_API const std::string& name() const noexcept;
		SONGYUN_API void setName(const std::string& name);

		SONGYUN_API virtual bool load(const std::string& path);
		SONGYUN_API virtual bool save(const std::string& path) const;

		SONGYUN_API int addObject(const TopoDS_Shape& object);
		SONGYUN_API  int createAssembly(const std::vector<int>& children);
		SONGYUN_API void removeObject(const int id);
		SONGYUN_API const std::vector<TopoDS_Shape>& objects() const noexcept;
	private:
		bool createNew();
	private:
		std::string name_;
		std::vector<std::shared_ptr<TopoDS_Shape>> objects_;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
} // namespace SongYun
