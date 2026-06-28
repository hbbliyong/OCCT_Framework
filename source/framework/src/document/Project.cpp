#include "document/Project.h"
#include "document/Document.h"

namespace SongYun {

	Project::Project(const QString& name)
		: m_name(name)
		, m_doc(std::make_unique<Document>(name.toStdString()))
	{}

	Project::~Project() = default;

	QString   Project::name() const    { return m_name; }
	Document* Project::document() const { return m_doc.get(); }

} // namespace SongYun
