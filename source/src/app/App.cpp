#include "app/App.h"
#include <QApplication>

App& App::Instance()
{
    static App instance;
    return instance;
}

App::App() = default;

bool App::initialize(QApplication* application, const QString& appName)
{
    if (initialized_ || application == nullptr)
        return false;

    application_ = application;
    applicationName_ = appName;
    initialized_ = true;
    return true;
}

void App::shutdown()
{
    initialized_ = false;
    application_ = nullptr;
    applicationName_.clear();
}

bool App::isInitialized() const noexcept
{
    return initialized_;
}

QApplication* App::application() const noexcept
{
    return application_;
}

const QString& App::applicationName() const noexcept
{
    return applicationName_;
}
