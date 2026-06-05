#pragma once

#include <string>
#include <vector>
#include <memory>

class DocumentObject;

class Document
{
public:
    explicit Document(const std::string& name = "Untitled");
    virtual ~Document() = default;

    const std::string& name() const noexcept;
    void setName(const std::string& name);

    virtual bool load(const std::string& path);
    virtual bool save(const std::string& path) const;

    void addObject(const std::shared_ptr<DocumentObject>& object);
    const std::vector<std::shared_ptr<DocumentObject>>& objects() const noexcept;

private:
    std::string name_;
    std::vector<std::shared_ptr<DocumentObject>> objects_;
};
