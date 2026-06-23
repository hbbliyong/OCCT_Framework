// CommandMacros.h
#pragma once
#include "command/CommandRegistry.h"
#define REGISTER_COMMAND(CommandClass, id)                  \
 namespace {                                                \
    static struct CommandRegistrar_##CommandClass           \
    {                                                       \
        CommandRegistrar_##CommandClass()                   \
        {                                                   \
                                            \
            SongYun::CommandRegistry::Instance().registerDescriptor( \
                id, [] { return new CommandClass(); });     \
        }                                                   \
    } s_registrar_##CommandClass;                            \
 }