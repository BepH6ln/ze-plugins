#define CONSOLEMANAGERS_FUNCTIONS#define CONSOLEMANAGERS_FUNCTIONS

enum struct classConfigFileDetail
{
    char    consoleEN[256];         // "en"
    char    consoleJP[256];         // "jp"
    char    consoleCHI[256];        // "chi"
    char    consoleZHO[256];        // "zho"
    char    consoleKR[256];         // "kr"
    int     RGBsColor[3];           // "color"
    int     trigtimer;              // "timer"
    bool    isEnabledCountdown;     // "countdown"
    bool    isEnabledTextHud;       // "showtext"
    bool    isEnabledCenterHud;     // "showhud"
    int     outputTimes;            // "times"
    char    trigInitConsole[256];   // "initialize"
    float   holdtime;               // "holdtime"
    bool    isEnabledHintHud;       // "instructor"
    bool    isEnableHintCountdown;  // "caption"
    char    hintTarget[64];         // "target"
    char    hintIcon[32];           // "icon"
    char    hintKeyBinds[32];       // "bind"
    int     hintHoldtime;           // "timeout"
    float   hintRange;              // "range"
}

stock bool IsValidClient(int client)
{
    if(client > 0 && client <= MaxClients && IsClientInGame(client) && !IsFakeClient(client)) return true;
    return false;
}

/**
 * Returns result of subtraction but does not take negative values.
 *
 * @param str1          First integer (left).
 * @param str2          Second integer (right).
 * @return              Integer of str1 minus str2. If result is a negative value, it will return 0.
 */
stock int SubtractNeverNegative(int i, int j)
{
    if(i - j < 0) return 0;
    return i - j;
}