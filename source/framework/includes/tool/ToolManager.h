#pragma once

#include "core/CoreMacro.hpp"
#include "common/Singleton.h"
#include "tool/Tool.h"
#include <memory>

namespace SongYun {

class ToolManager : public Singleton<ToolManager>
{
public:
    SONGYUN_API void setActiveTool(std::shared_ptr<Tool> tool);
    SONGYUN_API std::shared_ptr<Tool> activeTool() const;

    SONGYUN_API void clearActiveTool();

    SONGYUN_API void mouseMove(void* event);
    SONGYUN_API void mousePress(void* event);
    SONGYUN_API void mouseRelease(void* event);

private:
    std::shared_ptr<Tool> activeTool_;
};

} // namespace SongYun
