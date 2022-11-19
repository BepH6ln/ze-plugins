#define CONSOLEMANAGERS_TEXTHUDPARAMS

#include <adt_array>

ConVar textHudFirstChannel;
ConVar textHudSecondChannel;
ConVar textHudThirdChannel;

ArrayList       replaceableChannel;
static bool     isAvailableChannel[3]   = {true, ...};
static int      useChannel[MAXPHRASE];
static int      hudChannelGroup[3] = {3, 4, 5};

public void TextHudParams_OnPluginStart()
{
    textHudFirstChannel = CreateConVar("sm_hud_firstchannel", "3", "Change HUD Channel/Group Dynamic channel for the 1st game_text paragraph.", _, true, 0.0, true, 5.0);
    textHudSecondChannel = CreateConVar("sm_hud_secondchannel", "4", "Change HUD Channel/Group Dynamic channel for the 2nd game_text paragraph.", _, true, 0.0, true, 5.0);
    textHudThirdChannel = CreateConVar("sm_hud_thirdchannel", "5", "Change HUD Channel/Group Dynamic channel for the 3rd game_text paragraph.", _, true, 0.0, true, 5.0);

    textHudFirstChannel.AddChangeHook(DetectConVarChange);
    textHudSecondChannel.AddChangeHook(DetectConVarChange);
    textHudThirdChannel.AddChangeHook(DetectConVarChange);

    ReadConfigTextHudChannels();
    AutoExecConfig(true, "ConsoleManagers");
}

void DetectConVarChange(ConVar convar, const char[] oldValue, const char[] newValue)
{
    if(convar == textHudFirstChannel || convar == textHudSecondChannel || convar == textHudThirdChannel) ReadConfigTextHudChannels();
}

stock void ReadConfigTextHudChannels()
{
    hudChannelGroup[0] = textHudFirstChannel.IntValue;
    hudChannelGroup[1] = textHudSecondChannel.IntValue;
    hudChannelGroup[2] = textHudThirdChannel.IntValue;
}

stock void ImportTextHudParameters(const char[] console, int phrase)
{
    if(!replaceableChannel) replaceableChannel = new ArrayList(4);

    if(!StrEqual(console, ""))
    {
        if(configsDetail[phrase].isEnabledTextHud)
        {
            useChannel[phrase] = FindAvailableChannel();
            RefreshChannelStatusBegins(useChannel[phrase], phrase);

            if(!configsDetail[phrase].isEnabledCountdown || configsDetail[phrase].trigtimer <= 0) CreateTimer(7.0, RefreshChannelStatusEnds, phrase);
            ExportTextHudParameters(console, phrase, configsDetail[phrase].RGBsColor, false);
        }
    }
}

stock int FindAvailableChannel()
{
    if(!isAvailableChannel[0] && !isAvailableChannel[1] && !isAvailableChannel[2])
    {
        if(GetArraySize(replaceableChannel) == 0) return 0;
        return GetArrayCell(replaceableChannel, 0);
    }

    for(int i = 0; i < 3; i++)
    {
        if(isAvailableChannel[i]) return i;
    }
    return 0;
}

void RefreshChannelStatusBegins(int channel, int phrase)
{
    if(FindValueInArray(replaceableChannel, channel) != -1) RemoveFromArray(replaceableChannel, FindValueInArray(replaceableChannel, channel));

    isAvailableChannel[channel] = false;
    if(!configsDetail[phrase].isEnabledCountdown || configsDetail[phrase].trigtimer <= 0) PushArrayCell(replaceableChannel, channel);
}

public Action RefreshChannelStatusEnds(Handle timer, int phrase)
{
    if(timer != INVALID_HANDLE)
    {
        KillTimer(timer);
        timer = INVALID_HANDLE;
    }

    isAvailableChannel[useChannel[phrase]] = true;
    if(FindValueInArray(replaceableChannel, useChannel[phrase]) != -1) RemoveFromArray(replaceableChannel, FindValueInArray(replaceableChannel, useChannel[phrase]));

    return Plugin_Handled;
}

stock void ExportTextHudParameters(const char[] console, int phrase, int color[3], bool isCountable)
{
    int channel = useChannel[phrase];
    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i))
        {
            float textPositionX = clientSettings[i].hudPosition[0];
            float textPositionY = clientSettings[i].hudPosition[1] + 0.03 * channel;

            if(isCountable) SetHudTextParams(textPositionX, textPositionY, 1.0, color[0], color[1], color[2], 255, 1, 0.0, 0.02, 5.0);
            else SetHudTextParams(textPositionX, textPositionY, configsDetail[phrase].holdtime, color[0], color[1], color[2], 255, 2, 0.0, 0.02, 5.0);

            #if defined DYNAMIC_CHANNELS
                ShowHudText(i, GetDynamicChannel(hudChannelGroup[channel]), console);
            #else
                ShowHudText(i, hudChannelGroup[channel], console);
            #endif
        }
    }
}