#pragma once

#include "core/CoreMacro.hpp"
#include <string>
#include <vector>
#include <memory>

namespace SongYun {

class DocumentObject;

class Document
{
public:
    SONGYUN_API explicit Document(const std::string& name = "Untitled");
    SONGYUN_API virtual ~Document() = default;

    SONGYUN_API const std::string& name() const noexcept;
    SONGYUN_API void setName(const std::string& name);

    SONGYUN_API virtual bool load(const std::string& path);
    SONGYUN_API virtual bool save(const std::string& path) const;

    SONGYUN_API void addObject(const std::shared_ptr<DocumentObject>& object);
    SONGYUN_API const std::vector<std::shared_ptr<DocumentObject>>& objects() const noexcept;

private:
    std::string name_;
    std::vector<std::shared_ptr<DocumentObject>> objects_;
};

} // namespace SongYun
