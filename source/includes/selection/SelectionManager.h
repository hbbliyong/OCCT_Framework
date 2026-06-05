#pragma once

#include "common/Singleton.h"
#include "selection/PickResult.h"
#include <memory>

class SelectionFilter;

class SelectionManager : public Singleton<SelectionManager>
{
public:
    void setFilter(std::shared_ptr<SelectionFilter> filter);
    std::shared_ptr<SelectionFilter> filter() const;

    void setCurrentSelection(const PickResult& result);
    const PickResult& currentSelection() const;

private:
    std::shared_ptr<SelectionFilter> filter_;
    PickResult currentSelection_;
};
