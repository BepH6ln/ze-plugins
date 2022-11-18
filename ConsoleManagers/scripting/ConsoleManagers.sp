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
    version     = "2.0xalpha_01",
    url         = "https://github.com/BepH6ln"
};

char importConsole[MAXPHRASE][256];

#include <consmanagers/functions>
#include <consmanagers/applyconfigs>

public void OnPluginStart()
{
    HookEvent("round_start", OnRoundStart, EventHookMode_PostNoCopy);
}

void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{
    LoadConfigsFile();
}

public Action OnClientSayCommand(int client, const char[] command, const char[] sArgs)
{
    if(client == 0)
    {
        for(int i = 0; i < MAXPHRASE; i++)
        {
            if(StrEqual(importConsole[i], ""))
            {
                strcopy(importConsole[i], sizeof(importConsole[]), sArgs);
                ReadConfigsFile(importConsole[i], i);

                CreateNewSyntaxConsole(importConsole[i]);
                break;
            }
        }
    }
    return Plugin_Handled;
}

void CreateNewSyntaxConsole(const char[] console)
{
    char exportConsole[280];
    Format(exportConsole, sizeof(exportConsole), "%s%s", "{lime}", console);

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i)) CPrintToChat(i, "Console: %s", exportConsole);
    }
}