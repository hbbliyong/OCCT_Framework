#include "selection/SelectionManager.h"

void SelectionManager::setFilter(std::shared_ptr<SelectionFilter> filter)
{
    filter_ = std::move(filter);
}

std::shared_ptr<SelectionFilter> SelectionManager::filter() const
{
    return filter_;
}

void SelectionManager::setCurrentSelection(const PickResult& result)
{
    currentSelection_ = result;
}

const PickResult& SelectionManager::currentSelection() const
{
    return currentSelection_;
}
