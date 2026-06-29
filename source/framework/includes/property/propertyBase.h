#pragma once
namespace SongYun{
class PropertyBase
{
public:
    virtual ~PropertyBase() = default;

    virtual void clearDirty() = 0;
    virtual bool isDirty() const = 0;
};}