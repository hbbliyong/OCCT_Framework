#pragma once

#include <QString>

class QApplication;

class App
{
public:
    static App &Instance();

    bool initialize(QApplication *application, const QString &appName = "VtkQtMinimal");
    void shutdown();
    bool isInitialized() const noexcept;
    QApplication *application() const noexcept;
    const QString &applicationName() const noexcept;

private:
    App();

    QApplication *application_ = nullptr;
    QString applicationName_;
    bool initialized_ = false;
};
