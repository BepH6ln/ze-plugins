#define CONSOLEMANAGERS_TEXTHUDPARAMS

#include <adt_array>

ArrayList       replaceableChannel;
static bool     isAvailableChannel[3]   = {true, ...};
static int      useChannel[MAXPHRASE];

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
    float textPositionX = 0.16;
    float textPositionY = 0.09 + 0.03 * channel;

    if(isCountable) SetHudTextParams(textPositionX, textPositionY, 1.0, color[0], color[1], color[2], 255, 1, 0.0, 0.02, 5.0);
    else SetHudTextParams(textPositionX, textPositionY, configsDetail[phrase].holdtime, color[0], color[1], color[2], 255, 2, 0.0, 0.02, 5.0);

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i))
        {
            #if defined DYNAMIC_CHANNELS
                ShowHudText(i, GetDynamicChannel(channel), console);
            #else
                ShowHudText(i, channel, console);
            #endif
        }
    }
}