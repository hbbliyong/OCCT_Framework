#pragma once

#include "core/CoreMacro.hpp"
#include "common/Singleton.h"
#include "selection/PickResult.h"
#include <memory>

namespace SongYun {

class SelectionFilter;

class SelectionManager : public Singleton<SelectionManager>
{
public:
    SONGYUN_API void setFilter(std::shared_ptr<SelectionFilter> filter);
    SONGYUN_API std::shared_ptr<SelectionFilter> filter() const;

    SONGYUN_API void setCurrentSelection(const PickResult& result);
    SONGYUN_API const PickResult& currentSelection() const;

private:
    std::shared_ptr<SelectionFilter> filter_;
    PickResult currentSelection_;
};

} // namespace SongYun
