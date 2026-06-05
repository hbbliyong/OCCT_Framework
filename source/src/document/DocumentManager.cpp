#include "document/DocumentManager.h"

DocumentManager& DocumentManager::Instance()
{
    static DocumentManager instance;
    return instance;
}

DocumentManager::DocumentManager() = default;

std::shared_ptr<Document> DocumentManager::createDocument(const std::string& name)
{
    auto document = std::make_shared<Document>(name);
    documents_.push_back(document);
    activeDocument_ = document;
    return document;
}

std::shared_ptr<Document> DocumentManager::activeDocument() const
{
    return activeDocument_;
}

void DocumentManager::setActiveDocument(const std::shared_ptr<Document>& document)
{
    activeDocument_ = document;
}

const std::vector<std::shared_ptr<Document>>& DocumentManager::documents() const noexcept
{
    return documents_;
}
