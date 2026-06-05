#pragma once

#include "command/Command.h"
#include <QString>
#include <map>
#include <functional>
#include <memory>
#include <any>

class CommandFactory
{
public:
    using Creator = std::function<std::shared_ptr<Command>(const std::map<QString, std::any> &)>;

    static CommandFactory &instance();

    void registerCommand(const QString &name, const Creator &creator);
    std::shared_ptr<Command> create(const QString &name, const std::map<QString, std::any> &params = {}) const;
    bool contains(const QString &name) const;

private:
    CommandFactory() = default;
    std::map<QString, Creator> creators_;
};
