#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <memory>

namespace SongYun {

	class Document;

	class Project
	{
	public:
		SONGYUN_API explicit Project(const QString& name);
		SONGYUN_API ~Project();

		SONGYUN_API QString   name() const;
		SONGYUN_API Document* document() const;

	private:
		QString m_name;
		std::unique_ptr<Document> m_doc;
	};

} // namespace SongYun
