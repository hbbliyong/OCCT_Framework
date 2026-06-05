#pragma once

#include "document/Document.h"
#include <memory>
#include <vector>

class DocumentManager
{
public:
    static DocumentManager& Instance();

    std::shared_ptr<Document> createDocument(const std::string& name = "Untitled");
    std::shared_ptr<Document> activeDocument() const;
    void setActiveDocument(const std::shared_ptr<Document>& document);
    const std::vector<std::shared_ptr<Document>>& documents() const noexcept;

private:
    DocumentManager();
    std::vector<std::shared_ptr<Document>> documents_;
    std::shared_ptr<Document> activeDocument_;
};
