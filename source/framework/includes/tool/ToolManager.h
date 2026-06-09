#pragma once

#include "core/CoreMacro.hpp"
#include "common/Singleton.h"
#include "tool/Tool.h"
#include "common/types.h"
#include <memory>

namespace SongYun
{
    class ToolManager : public Singleton<ToolManager>
    {
    public:
        void SetCurrent(Ref<Tool> tool);

        Tool *Current() const;

    private:
        Ref<Tool> m_current;
    };

} // namespace SongYun
