# ==========================================================
# CAD Framework Generator
# Qt + OCCT + C++20
# ==========================================================

$Root = "src"

# ----------------------------------------------------------
# Create Folder
# ----------------------------------------------------------

$Folders = @(
    "$Root/app",

    "$Root/ui",
    "$Root/ui/MainWindow",
    "$Root/ui/Ribbon",

    "$Root/view",

    "$Root/selection",

    "$Root/command",
    "$Root/command/commands",

    "$Root/tool",
    "$Root/tool/tools",

    "$Root/document",

    "$Root/occ",

    "$Root/geometry",

    "$Root/common"
)

foreach ($folder in $Folders)
{
    if (!(Test-Path $folder))
    {
        New-Item -ItemType Directory -Path $folder | Out-Null
        Write-Host "Created Folder: $folder"
    }
}

# ----------------------------------------------------------
# Create Files
# ----------------------------------------------------------

$Files = @(

    # app
    "$Root/app/App.h",
    "$Root/app/App.cpp",

    # ui
    "$Root/ui/MainWindow/MainWindow.h",
    "$Root/ui/MainWindow/MainWindow.cpp",

    "$Root/ui/Ribbon/RibbonWidget.h",
    "$Root/ui/Ribbon/RibbonWidget.cpp",

    # view
    "$Root/view/OccView.h",
    "$Root/view/OccView.cpp",

    "$Root/view/ViewManager.h",
    "$Root/view/ViewManager.cpp",

    "$Root/view/CameraController.h",
    "$Root/view/CameraController.cpp",

    # selection
    "$Root/selection/PickResult.h",

    "$Root/selection/SelectionManager.h",
    "$Root/selection/SelectionManager.cpp",

    "$Root/selection/SelectionFilter.h",
    "$Root/selection/SelectionFilter.cpp",

    # command
    "$Root/command/ICommand.h",

    "$Root/command/CommandManager.h",
    "$Root/command/CommandManager.cpp",

    # tool
    "$Root/tool/Tool.h",

    "$Root/tool/ToolManager.h",
    "$Root/tool/ToolManager.cpp",

    # document
    "$Root/document/Document.h",
    "$Root/document/Document.cpp",

    "$Root/document/DocumentObject.h",
    "$Root/document/DocumentObject.cpp",

    "$Root/document/DocumentManager.h",
    "$Root/document/DocumentManager.cpp",

    # occ
    "$Root/occ/OccContext.h",
    "$Root/occ/OccContext.cpp",

    "$Root/occ/OccViewer.h",
    "$Root/occ/OccViewer.cpp",

    "$Root/occ/OccSelection.h",
    "$Root/occ/OccSelection.cpp",

    # geometry
    "$Root/geometry/SweepBuilder.h",
    "$Root/geometry/SweepBuilder.cpp",

    "$Root/geometry/LoftBuilder.h",
    "$Root/geometry/LoftBuilder.cpp",

    "$Root/geometry/BooleanBuilder.h",
    "$Root/geometry/BooleanBuilder.cpp",

    "$Root/geometry/FilletBuilder.h",
    "$Root/geometry/FilletBuilder.cpp",

    # common
    "$Root/common/EventBus.h",
    "$Root/common/EventBus.cpp",

    "$Root/common/Types.h",

    "$Root/common/Singleton.h"
)

foreach ($file in $Files)
{
    if (!(Test-Path $file))
    {
        New-Item -ItemType File -Path $file | Out-Null
        Write-Host "Created File: $file"
    }
}

# ----------------------------------------------------------
# Write Templates
# ----------------------------------------------------------

# App.h

@"
#pragma once

class App
{
public:

    static App& Instance();

private:

    App();
};
"@ | Set-Content "$Root/app/App.h"

# ICommand.h

@"
#pragma once

enum class CommandState
{
    Idle,
    Running,
    Finished,
    Cancelled
};

class ICommand
{
public:

    virtual ~ICommand() = default;

    virtual void Start() = 0;

    virtual void Cancel() = 0;

    virtual CommandState State() const = 0;
};
"@ | Set-Content "$Root/command/ICommand.h"

# Tool.h

@"
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
"@ | Set-Content "$Root/tool/Tool.h"

# PickResult.h

@"
#pragma once

#include <optional>

#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>

struct PickResult
{
    gp_Pnt worldPoint;

    TopoDS_Shape shape;

    Handle(AIS_InteractiveObject) object;
};
"@ | Set-Content "$Root/selection/PickResult.h"

# Singleton.h

@"
#pragma once

template<typename T>
class Singleton
{
public:

    static T& Instance()
    {
        static T instance;
        return instance;
    }

protected:

    Singleton() = default;

    ~Singleton() = default;

private:

    Singleton(const Singleton&) = delete;

    Singleton& operator=(const Singleton&) = delete;
};
"@ | Set-Content "$Root/common/Singleton.h"

# Types.h

@"
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <string>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;
"@ | Set-Content "$Root/common/Types.h"

Write-Host ""
Write-Host "========================================"
Write-Host " CAD Framework Generated Successfully"
Write-Host "========================================"
Write-Host ""