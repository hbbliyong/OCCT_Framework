#pragma once

#include "core/CoreMacro.hpp"
#include "document/Document.h"
#include <memory>
#include <vector>

namespace SongYun {

class DocumentManager
{
public:
    SONGYUN_API static DocumentManager& Instance();

    SONGYUN_API std::shared_ptr<Document> createDocument(const std::string& name = "Untitled");
    SONGYUN_API std::shared_ptr<Document> activeDocument() const;
    SONGYUN_API void setActiveDocument(const std::shared_ptr<Document>& document);
    SONGYUN_API const std::vector<std::shared_ptr<Document>>& documents() const noexcept;

private:
    DocumentManager();
    std::vector<std::shared_ptr<Document>> m_documents;
    std::shared_ptr<Document> m_activeDocument;
};

} // namespace SongYun
