#define CONSOLEMANAGERS_FUNCTIONS#define CONSOLEMANAGERS_FUNCTIONS

enum struct classConfigFileDetail
{
    int     trigtimer;              // "timer"
}

stock bool IsValidClient(int client)
{
    if(client > 0 && client <= MaxClients && IsClientInGame(client) && !IsFakeClient(client)) return true;
    return false;
}