#pragma once

#include "core/CoreMacro.hpp"

class QMouseEvent;

namespace SongYun {

class Tool
{
public:

    virtual SONGYUN_API ~Tool() = default;

    virtual SONGYUN_API void OnEnter() {}

    virtual SONGYUN_API void OnLeave() {}

    virtual SONGYUN_API void MouseMove(QMouseEvent*) {}

    virtual SONGYUN_API void MousePress(QMouseEvent*) {}

    virtual SONGYUN_API void MouseRelease(QMouseEvent*) {}
};

} // namespace SongYun
