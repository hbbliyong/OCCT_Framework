#pragma once

class QMouseEvent;

class Tool
{
public:

    virtual ~Tool() = default;

    virtual void OnEnter() {}

    virtual void OnLeave() {}

    virtual void MouseMove(QMouseEvent*) {}

    virtual void MousePress(QMouseEvent*) {}

    virtual void MouseRelease(QMouseEvent*) {}
};
