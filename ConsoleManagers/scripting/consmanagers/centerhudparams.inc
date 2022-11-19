#define CONSOLEMANAGERS_CENTERHUDPARAMS

static char consoleHTMLtag[MAXPHRASE][480];

stock void ImportCenterHudParameters(const char[] console, int phrase)
{
    if(!StrEqual(console, ""))
    {
        if(configsDetail[phrase].isEnabledCenterHud)
        {
            if(!configsDetail[phrase].isEnabledCountdown || configsDetail[phrase].trigtimer <= 0) CreateTimer(5.0, RefreshCenterHudMessage, phrase);

            ConvertToHTMLSyntax(console, phrase, configsDetail[phrase].RGBsColor, false, configsDetail[phrase].trigtimer);
        }
    }
}

void ConvertToHTMLSyntax(const char[] console, int phrase, int color[3], bool isCountable, int timer)
{
    char colorHTMLtag[32];
    colorHTMLtag = ConvertColorToHTML(color);

    char tempConsole[480];
    strcopy(tempConsole, sizeof(tempConsole), console);

    ReplaceString(tempConsole, sizeof(tempConsole), ">", "");
    ReplaceString(tempConsole, sizeof(tempConsole), "<", "");
    ReplaceString(tempConsole, sizeof(tempConsole), "&", "");
    ReplaceString(tempConsole, sizeof(tempConsole), "'", "");

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i) && clientSettings[i].isEnabledColorSync)
        {
            char clientColorHTMLtag[32];
            clientColorHTMLtag = ConvertColorToHTML(clientSettings[i].RGBsColor);

            char tempColorTimer[160];
            FormatEx(tempColorTimer, sizeof(tempColorTimer), "<font color='#%s'>%i<font color='#%s'>", clientColorHTMLtag, timer, colorHTMLtag);

            char stringTypeTimer[8];
            IntToString(timer, stringTypeTimer, sizeof(stringTypeTimer));
            ReplaceString(tempConsole, sizeof(tempConsole), stringTypeTimer, tempColorTimer);
        }
    }
    Format(consoleHTMLtag[phrase], sizeof(consoleHTMLtag[]), "<font color='#%s'>%s", colorHTMLtag, tempConsole);

    if(isCountable) PrepareExportCenterHud();
    else ExportCenterHudParameters(phrase);
}

stock void ExportCenterHudParameters(int phrase)
{
    int availablePhrases[3] = {-1, ...};
    for(int i = phrase; i >= 0; i--)
    {
        static int j = 2;
        if(!StrEqual(consoleHTMLtag[i], ""))
        {
            availablePhrases[j] = i;
            j--;
        }

        if(j < 0 || i == 0)
        {
            j = 2;
            break;
        }
    }

    int lengthConsoleHTMLtag[2];
    char buildupTextMessage[1440];

    if(availablePhrases[2] == -1) return;
    else if(availablePhrases[1] == -1)
    {
        lengthConsoleHTMLtag[0] = strlen(consoleHTMLtag[availablePhrases[2]]);
        if(lengthConsoleHTMLtag[0] <= 72) Format(buildupTextMessage, sizeof(buildupTextMessage), "<span class='fontSize-xl'>%s", consoleHTMLtag[availablePhrases[2]]);
        else Format(buildupTextMessage, sizeof(buildupTextMessage), "<span class='fontSize-l'>%s", consoleHTMLtag[availablePhrases[2]]);
    }
    else if(availablePhrases[0] == -1)
    {
        lengthConsoleHTMLtag[0] = strlen(consoleHTMLtag[availablePhrases[2]]);
        lengthConsoleHTMLtag[1] = strlen(consoleHTMLtag[availablePhrases[1]]);
        if(lengthConsoleHTMLtag[0] <= 104 && lengthConsoleHTMLtag[1] <= 104) Format(buildupTextMessage, sizeof(buildupTextMessage), "<span class='fontSize-l'>%s<br>%s", consoleHTMLtag[availablePhrases[1]], consoleHTMLtag[availablePhrases[2]]);
        else Format(buildupTextMessage, sizeof(buildupTextMessage), "<span class='fontSize-m'>%s<br>%s", consoleHTMLtag[availablePhrases[1]], consoleHTMLtag[availablePhrases[2]]);
    }
    else Format(buildupTextMessage, sizeof(buildupTextMessage), "<span class='fontSize-m'>%s<br>%s<br>%s", consoleHTMLtag[availablePhrases[0]], consoleHTMLtag[availablePhrases[1]], consoleHTMLtag[availablePhrases[2]]);

    for(int i = 0; i <= MaxClients; i++)
    {
        if(IsValidClient(i) && clientSettings[i].displayStyle == 2)
        {
            Event displayCenterHud = CreateEvent("show_survival_respawn_status");

            displayCenterHud.SetString("loc_token", buildupTextMessage);
            displayCenterHud.SetInt("duration", 5);
            displayCenterHud.SetInt("userid", -1);

            displayCenterHud.FireToClient(i);
            displayCenterHud.Cancel();
        }
    }
}

Action RefreshCenterHudMessage(Handle timer, int phrase)
{
    if(timer != INVALID_HANDLE)
    {
        KillTimer(timer);
        timer = INVALID_HANDLE;
    }
    consoleHTMLtag[phrase] = "";

    PrepareExportCenterHud();
    return Plugin_Handled;
}

stock void PrepareExportCenterHud()
{
    for(int i = 0; i < MAXPHRASE; i++)
    {
        if(StrEqual(importConsole[i], ""))
        {
            ExportCenterHudParameters(i - 1);
            break;
        }
    }
}