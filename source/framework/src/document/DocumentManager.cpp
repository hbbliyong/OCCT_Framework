#include "document/DocumentManager.h"
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