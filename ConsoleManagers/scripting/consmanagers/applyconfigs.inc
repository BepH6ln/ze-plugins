#define CONSOLEMANAGERS_APPLYCONFIGS

KeyValues               kvsConsole;
classConfigFileDetail   configsDetail[MAXPHRASE];

stock void LoadConfigsFile()
{
    if(kvsConsole) delete kvsConsole;

    char currentMapName[64], configFullPath[PLATFORM_MAX_PATH];
    GetCurrentMap(currentMapName, sizeof(currentMapName));
    FormatEx(configFullPath, sizeof(configFullPath), "cfg/sourcemod/consolemanagers/%s.txt", currentMapName);

    kvsConsole = CreateKeyValues("console");
    if(FileExists(configFullPath))
    {
        FileToKeyValues(kvsConsole, configFullPath);
        CPrintToChatAll("%s[ConsManagers] %sThe current map is supported by ConsoleManagers.", "{lime}", "{gold}");
    }
}

stock void ReadConfigsFile(const char[] console, int phrase)
{
    if(!kvsConsole) LoadConfigsFile();

    kvsConsole.Rewind();
    kvsConsole.JumpToKey(console);

    configsDetail[phrase].trigtimer = kvsConsole.GetNum("timer", 0);
}
