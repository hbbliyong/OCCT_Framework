#include "tool/ToolManager.h"
#include <QMouseEvent>

void ToolManager::setActiveTool(std::shared_ptr<Tool> tool)
{
    activeTool_ = std::move(tool);
}

std::shared_ptr<Tool> ToolManager::activeTool() const
{
    return activeTool_;
}

void ToolManager::clearActiveTool()
{
    activeTool_.reset();
}

void ToolManager::mouseMove(void *event)
{
    if (activeTool_)
        activeTool_->MouseMove(reinterpret_cast<QMouseEvent *>(event));
}

void ToolManager::mousePress(void *event)
{
    if (activeTool_)
        activeTool_->MousePress(reinterpret_cast<QMouseEvent *>(event));
}

void ToolManager::mouseRelease(void *event)
{
    if (activeTool_)
        activeTool_->MouseRelease(reinterpret_cast<QMouseEvent *>(event));
}
