#include "document/DocumentManager.h"
#include "document/ProjectManager.h"
#include "document/Project.h"

namespace SongYun {
	DocumentManager& DocumentManager::Instance()
	{
		static DocumentManager instance;
		return instance;
	}

	DocumentManager::DocumentManager() = default;

	std::shared_ptr<Document> DocumentManager::createDocument(const std::string& name)
	{
		auto document = std::make_shared<Document>(name);
		m_documents.push_back(document);
		m_activeDocument = document;
		return document;
	}

	std::shared_ptr<Document> DocumentManager::activeDocument() const
	{
		// 优先取 ProjectManager 的项目文档
		if (auto* proj = ProjectManager::Instance().activeProject())
			return std::shared_ptr<Document>(proj->document(), [](Document*){});
		return m_activeDocument;
	}

	void DocumentManager::setActiveDocument(const std::shared_ptr<Document>& document)
	{
		m_activeDocument = document;
	}

	const std::vector<std::shared_ptr<Document>>& DocumentManager::documents() const noexcept
	{
		return m_documents;
	}
}