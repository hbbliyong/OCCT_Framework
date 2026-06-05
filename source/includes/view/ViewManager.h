#pragma once

#include <QObject>

class View;

class ViewManager : public QObject
{
    Q_OBJECT

public:
    explicit ViewManager(QObject* parent = nullptr);

    void setActiveView(View* view);
    View* activeView() const;

private:
    View* activeView_ = nullptr;
};
