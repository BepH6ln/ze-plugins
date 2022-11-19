#pragma newdecls required
#pragma semicolon 1

#include <clientprefs>
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
    description = "Better console syntaxes and additional commands.", // Special thanks to Lupercalia[JP].
    version     = "6.9alpha",
    url         = "https://github.com/BepH6ln"
};

char importConsole[MAXPHRASE][256];

#include "consmanagers/functions.inc"
#include "consmanagers/applyconfigs.inc"
#include "consmanagers/clientcookies.inc"
#include "consmanagers/clientcommands.inc"
#include "consmanagers/timemanager.inc"
#include "consmanagers/timercountdown.inc"
#include "consmanagers/texthudparams.inc"
#include "consmanagers/colorconverter.inc"
#include "consmanagers/centerhudparams.inc"
#include "consmanagers/instructorhints.inc"

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

                char console[256];
                console = AdaptClientLanguage(i);

                CreateNewSyntaxConsole(console, i);
                ExtractMultipliedConsole(importConsole[i], i);

                if(!StrEqual(importConsole[i], ""))
                {
                    if(configsDetail[i].trigtimer > 0 && configsDetail[i].isEnabledCountdown) SaveConsoleParameters(i);

                    ImportTextHudParameters(console, i);
                    ImportCenterHudParameters(console, i);
                    GenerateInstructorHint(console, i, configsDetail[i].RGBsColor, false);
                }
                break;
            }
        }
        return Plugin_Stop;
    }
    return Plugin_Continue;
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

stock char[] AdaptClientLanguage(int phrase)
{
    char console[256];

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i))
        {
            switch(clientSettings[i].language)
            {
                case 0: strcopy(console, sizeof(console), configsDetail[phrase].consoleEN);
                case 1: strcopy(console, sizeof(console), configsDetail[phrase].consoleJP);
                case 2: strcopy(console, sizeof(console), configsDetail[phrase].consoleCHI);
                case 3: strcopy(console, sizeof(console), configsDetail[phrase].consoleZHO);
                case 4: strcopy(console, sizeof(console), configsDetail[phrase].consoleKR);
            }
        }
    }
    return console;
}