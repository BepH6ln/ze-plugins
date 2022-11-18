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
    version     = "6.0xalpha_02",
    url         = "https://github.com/BepH6ln"
};

char importConsole[MAXPHRASE][256];

#include <consmanagers/functions>
#include <consmanagers/applyconfigs>
#include <consmanagers/timemanager>
#include <consmanagers/timercountdown>
#include <consmanagers/texthudparams>
#include <consmanagers/colorconverter>
#include <consmanagers/centerhudparams>
#include <consmanagers/instructorhints>
#include <consmanagers/clientcookies>
#include <consmanagers/clientcommands>

public void OnPluginStart()
{
    TextHudParams_OnPluginStart();
    ClientCookies_OnPluginStart();
    ClientCommands_OnPluginStart();

    HookEvent("round_start", OnRoundStart, EventHookMode_PostNoCopy);
    HookEvent("round_end", OnRoundEnd, EventHookMode_PostNoCopy);
}

void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{
    LoadConfigsFile();
    SaveTimeWhenRoundStart();
}

void OnRoundEnd(Event event, const char[] name, bool dontBroadcast)
{
    InitializePlugin();
}

public void OnMapEnd()
{
    InitializePlugin();
}

public void OnPluginEnd()
{
    InitializePlugin();
}

void InitializePlugin()
{
    if(kvsConsole) delete kvsConsole;

    for(int i = 0; i < MAXPHRASE; i++)
    {
        if(!StrEqual(importConsole[i], "")) importConsole[i][0] = '\0';
    }
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

                CreateNewSyntaxConsole(importConsole[i], i);
                ExtractMultipliedConsole(importConsole[i], i);

                if(!StrEqual(importConsole[i], "") && configsDetail[i].trigtimer > 0 && configsDetail[i].isEnabledCountdown) SaveConsoleParameters(i);
                ImportTextHudParameters(importConsole[i], i);
                ImportCenterHudParameters(importConsole[i], i);
                GenerateInstructorHint(importConsole[i], i, configsDetail[i].RGBsColor, false);
                break;
            }
        }
    }
    return Plugin_Handled;
}

void CreateNewSyntaxConsole(const char[] console, int phrase)
{
    char exportConsole[280];

    if(configsDetail[phrase].trigtimer > 0)
    {
        int timeTriggerEnds = ExportTimeWhenTrigEnds(phrase);
        float tempMinutes   = timeTriggerEnds / 60.0;
        int minutes         = RoundToFloor(tempMinutes);
        int seconds         = timeTriggerEnds - minutes * 60;

        Format(exportConsole, sizeof(exportConsole), "%s%s %s- @ %i:%s%i", "{lime}", console, "{red}", minutes, (seconds < 10 ? "0":""), seconds);
    }
    else Format(exportConsole, sizeof(exportConsole), "%s%s", "{lime}", console);

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i)) CPrintToChat(i, "Console: %s", exportConsole);
    }
}