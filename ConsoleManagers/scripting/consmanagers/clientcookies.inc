#define CONSOLEMANAGERS_CLIENTCOOKIES

#include <clientprefs>

Handle  selectLanguage,
        toggleDisplayStyle,
        textHudPosition,
        colorSyncEnabled,
        tracksRGBsColor;
classClientSetting clientSettings[MAXPLAYERS + 1];

public void ClientCookies_OnPluginStart()
{
    selectLanguage      = RegClientCookie("consolemanagers_language", "", CookieAccess_Private);
    toggleDisplayStyle  = RegClientCookie("consolemanagers_displaystyle", "", CookieAccess_Private);
    textHudPosition     = RegClientCookie("consolemanagers_hudpos", "", CookieAccess_Private);
    colorSyncEnabled    = RegClientCookie("consolemanager_colorSync", "", CookieAccess_Private);
    tracksRGBsColor     = RegClientCookie("consolemanagers_timercolor", "", CookieAccess_Private);
}

public void OnClientPutInServer(int client)
{
    if(!AreClientCookiesCached(client)) SaveDefaultClientSettings(client);
}

public void OnClientCookiesCached(int client)
{
    char cookieValue[32];

    GetClientCookie(client, selectLanguage, cookieValue, sizeof(cookieValue));
    if(!StrEqual(cookieValue, "")) clientSettings[client].language = StringToInt(cookieValue);
    else clientSettings[client].language = 0;

    GetClientCookie(client, toggleDisplayStyle, cookieValue, sizeof(cookieValue));
    if(!StrEqual(cookieValue, "")) clientSettings[client].displayStyle = StringToInt(cookieValue);
    else clientSettings[client].displayStyle = 1;

    GetClientCookie(client, textHudPosition, cookieValue, sizeof(cookieValue));
    if(!StrEqual(cookieValue, ""))
    {
        char hudBuffer[2][16];
        ExplodeString(cookieValue, " ", hudBuffer, 2, 16);

        float assignedValue = StringToFloat(hudBuffer[0]);
        if((assignedValue >= 0.0 && assignedValue <= 1.0) || assignedValue == -1.0) clientSettings[client].hudPosition[0] = assignedValue;
        else clientSettings[client].hudPosition[0] = 0.16;

        assignedValue = StringToFloat(hudBuffer[1]);
        if((assignedValue >= 0.0 && assignedValue <= 1.0) || assignedValue == -1.0) clientSettings[client].hudPosition[1] = assignedValue;
        else clientSettings[client].hudPosition[1] = 0.09;
    }
    else
    {
        clientSettings[client].hudPosition[0] = 0.16;
        clientSettings[client].hudPosition[1] = 0.09;
    }

    GetClientCookie(client, colorSyncEnabled, cookieValue, sizeof(cookieValue));
    if(!StrEqual(cookieValue, "")) clientSettings[client].isEnabledColorSync = view_as<bool>(StringToInt(cookieValue));
    else clientSettings[client].isEnabledColorSync = false;

    GetClientCookie(client, tracksRGBsColor, cookieValue, sizeof(cookieValue));
    if(!StrEqual(cookieValue, ""))
    {
        char colorBuffer[3][6]
        ExplodeString(cookieValue, " ", colorBuffer, 3, 6);

        int assignedValue = StringToInt(colorBuffer[0])
        if(assignedValue >= 0 && assignedValue <= 255) clientSettings[client].RGBsColor[0] = assignedValue;
        else clientSettings[client].RGBsColor[0] = 255;

        assignedValue = StringToInt(colorBuffer[1])
        if(assignedValue >= 0 && assignedValue <= 255) clientSettings[client].RGBsColor[1] = assignedValue;
        else clientSettings[client].RGBsColor[1] = 0;

        assignedValue = StringToInt(colorBuffer[2]);
        if(assignedValue >= 0 && assignedValue <= 255) clientSettings[client].RGBsColor[2] = assignedValue;
        else clientSettings[client].RGBsColor[2] = 0;
    }
    else
    {
        clientSettings[client].RGBsColor[0]         = 255;
        clientSettings[client].RGBsColor[1]         = 0;
        clientSettings[client].RGBsColor[2]         = 0;
    }
}

public void OnClientDisconnect(int client)
{
    SaveDefaultClientSettings(client);
}

void SaveDefaultClientSettings(int client)
{
    clientSettings[client].language             = 0;
    clientSettings[client].displayStyle         = 1;
    clientSettings[client].hudPosition[0]       = 0.16;
    clientSettings[client].hudPosition[1]       = 0.09;
    clientSettings[client].isEnabledColorSync   = false;
    clientSettings[client].RGBsColor[0]         = 255;
    clientSettings[client].RGBsColor[1]         = 0;
    clientSettings[client].RGBsColor[2]         = 0;
}