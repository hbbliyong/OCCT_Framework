#include "command/CommandRegistry.h"
#include "command/CommandFactory.h"
#include "command/commands/CreateCylinderCommand.h"
#include "command/commands/CreatePolylineCommand.h"
#include <AIS_InteractiveContext.hxx>

void CommandRegistry::registerAllCommands()
{
    auto &factory = CommandFactory::instance();

    factory.registerCommand("CreateCylinder", [](const std::map<QString, std::any> &params) -> std::shared_ptr<Command>
                            {
        try
        {
            auto context = std::any_cast<Handle(AIS_InteractiveContext)>(params.at("context"));
            auto statusCallback = std::any_cast<std::function<void(const QString &)>>(params.at("statusCallback"));
            return std::make_shared<CreateCylinderCommand>(context, statusCallback);
        }
        catch (const std::exception &)
        {
            return nullptr;
        } });

    factory.registerCommand("CreatePolyline", [](const std::map<QString, std::any> &params) -> std::shared_ptr<Command>
                            {
        try
        {
            auto context = std::any_cast<Handle(AIS_InteractiveContext)>(params.at("context"));
            auto statusCallback = std::any_cast<std::function<void(const QString &)>>(params.at("statusCallback"));
            return std::make_shared<CreatePolylineCommand>(context, statusCallback);
        }
        catch (const std::exception &)
        {
            return nullptr;
        } });
}
