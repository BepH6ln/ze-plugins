#define CONSOLEMANAGERS_TIMERCOUNTDOWN

static int timerCountdown[MAXPHRASE][2];

stock void SaveConsoleParameters(int phrase)
{
    timerCountdown[phrase][0] = configsDetail[phrase].trigtimer;
    timerCountdown[phrase][1] = configsDetail[phrase].trigtimer;

    CreateTimer(1.0, ConvertConsoleByTimer, phrase, TIMER_REPEAT);
}

Action ConvertConsoleByTimer(Handle timer, int phrase)
{
    timerCountdown[phrase][1]--;
    if(timerCountdown[phrase][1] <= 0 || HasTrigInitialized(phrase))
    {
        if(timer != INVALID_HANDLE)
        {
            KillTimer(timer);
            timer = INVALID_HANDLE;
        }

        if(configsDetail[phrase].isEnabledTextHud) CreateTimer(2.0, RefreshChannelStatusEnds, phrase);
        if(configsDetail[phrase].isEnabledCenterHud) CreateTimer(1.0, RefreshCenterHudMessage, phrase);
        return Plugin_Handled;
    }

    char stringTypeTimer[2][8];
    IntToString(timerCountdown[phrase][0], stringTypeTimer[0], sizeof(stringTypeTimer[]));
    IntToString(timerCountdown[phrase][1], stringTypeTimer[1], sizeof(stringTypeTimer[]));

    char console[256];
    console = AdaptClientLanguage(phrase);
    ReplaceString(console, sizeof(console), stringTypeTimer[0], stringTypeTimer[1]);

    if(configsDetail[phrase].isEnabledTextHud)
    {
        if(timerCountdown[phrase][1] <= 5) ExportTextHudParameters(console, phrase, {255, 0, 128}, true);
        else ExportTextHudParameters(console, phrase, configsDetail[phrase].RGBsColor, true);
    }

    if(configsDetail[phrase].isEnabledCenterHud)
    {
        if(timerCountdown[phrase][1] <= 5)  ConvertToHTMLSyntax(console, phrase, {255, 0, 128}, true, timerCountdown[phrase][1]);
        else ConvertToHTMLSyntax(console, phrase, configsDetail[phrase].RGBsColor, true, timerCountdown[phrase][1]);
    }

    if(configsDetail[phrase].isEnabledHintHud && configsDetail[phrase].isEnableHintCountdown)
    {
        if(timerCountdown[phrase][1] <= 5) GenerateInstructorHint(console, phrase, {255, 0, 128}, true);
        else GenerateInstructorHint(console, phrase, configsDetail[phrase].RGBsColor, false);
    }

    return Plugin_Handled;
}

stock bool HasTrigInitialized(int phrase)
{
    for(int i = phrase; i < MAXPHRASE; i++)
    {
        if(importConsole[i][0] != 0 && StrEqual(configsDetail[phrase].trigInitConsole, importConsole[i])) return true;
    }
    return false;
}