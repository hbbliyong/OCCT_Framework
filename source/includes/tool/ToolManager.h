#pragma once

#include "common/Singleton.h"
#include "tool/Tool.h"
#include <memory>

class ToolManager : public Singleton<ToolManager>
{
public:
    void setActiveTool(std::shared_ptr<Tool> tool);
    std::shared_ptr<Tool> activeTool() const;

    void clearActiveTool();

    void mouseMove(void* event);
    void mousePress(void* event);
    void mouseRelease(void* event);

private:
    std::shared_ptr<Tool> activeTool_;
};
