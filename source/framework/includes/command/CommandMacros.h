// CommandMacros.h
#pragma once
#define REGISTER_COMMAND(CommandClass, id)                  \
    static struct CommandRegistrar_##CommandClass           \
    {                                                       \
        CommandRegistrar_##CommandClass()                   \
        {                                                   \
            SongYun::CommandRegistry::instance().registerDescriptor( \
                id, [] { return new CommandClass(); });     \
        }                                                   \
    } s_registrar_##CommandClass