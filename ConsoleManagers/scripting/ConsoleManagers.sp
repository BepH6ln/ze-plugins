#pragma newdecls required
#pragma semicolon 1

#include <sdkhooks>
#include <sdktools>
#include <sourcemod>
#include <multicolors>

// #define DYNAMIC_CHANNELS
#if defined DYNAMIC_CHANNELS
#include <DynamicChannels>
#endif

#define MAXPHRASE 300

public Plugin myinfo =
{
    name        = "ConsoleManagers",
    author      = "Beppu",
    description = "Better console syntaxes and additional commands.",
    version     = "dev221118",
    url         = "https://github.com/BepH6ln"
};

