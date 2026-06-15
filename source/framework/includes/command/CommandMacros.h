// CommandMacros.h
#pragma once
#include "command/CommandRegistry.h"
#include "iostream"
#define REGISTER_COMMAND(CommandClass, id)                  \
 namespace {                                                \
    static struct CommandRegistrar_##CommandClass           \
    {                                                       \
        CommandRegistrar_##CommandClass()                   \
        {                                                   \
             std::cout << "Executing Registrar: " << &SongYun::CommandRegistry::Instance() << std::endl;                                               \
            SongYun::CommandRegistry::Instance().registerDescriptor( \
                id, [] { return new CommandClass(); });     \
        }                                                   \
    } s_registrar_##CommandClass;                            \
 }