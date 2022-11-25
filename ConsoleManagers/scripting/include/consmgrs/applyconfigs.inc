KeyValues               kvsConsoles;
classConfigFileDetail   configsDetail[MAXPHRASE];

stock void LoadConfigsFile()
{
    if(kvsConsoles) delete kvsConsoles;

    char stringMapName[64], configFullPath[PLATFORM_MAX_PATH];
    GetCurrentMap(stringMapName, sizeof(stringMapName));
    FormatEx(configFullPath, sizeof(configFullPath), "cfg/sourcemod/consolemanagers/%s.txt", stringMapName);

    kvsConsoles = CreateKeyValues("consoles");
    if(FileExists(configFullPath))
    {
        FileToKeyValues(kvsConsoles, configFullPath);
        CPrintToChatAll("%s[ConsMgrs] %sThe current map is supported by ConsoleManagers.", "{green}", "{gold}");
    }
}

stock void ReadConfigsFile(const char[] console, int phrase)
{
    if(!kvsConsoles) LoadConfigsFile();

    kvsConsoles.Rewind();
    kvsConsoles.JumpToKey(console);

    char tempBuffer[64];

    kvsConsoles.GetString("en", configsDetail[phrase].consoleEN, sizeof(configsDetail[].consoleEN), console);
    kvsConsoles.GetString("jp", configsDetail[phrase].consoleJP, sizeof(configsDetail[].consoleJP), console);
    kvsConsoles.GetString("chi", configsDetail[phrase].consoleCHI, sizeof(configsDetail[].consoleCHI), console);
    kvsConsoles.GetString("zho", configsDetail[phrase].consoleZHO, sizeof(configsDetail[].consoleZHO), console);
    kvsConsoles.GetString("kr", configsDetail[phrase].consoleKR, sizeof(configsDetail[].consoleKR), console);

    kvsConsoles.GetString("color", tempBuffer, sizeof(tempBuffer), "{lightgreen}")
    if(StrEqual(tempBuffer, "{default}")){configsDetail[phrase].RGBsColor[0] = 255; configsDetail[phrase].RGBsColor[1] = 255; configsDetail[phrase].RGBsColor[2] = 255;}
    else if(StrEqual(tempBuffer, "{red}")){configsDetail[phrase].RGBsColor[0] = 255; configsDetail[phrase].RGBsColor[1] = 64; configsDetail[phrase].RGBsColor[2] = 64;}
    else if(StrEqual(tempBuffer, "{lightred}")){configsDetail[phrase].RGBsColor[0] = 230; configsDetail[phrase].RGBsColor[1] = 76; configsDetail[phrase].RGBsColor[2] = 76;}
    else if(StrEqual(tempBuffer, "{darkred}")){configsDetail[phrase].RGBsColor[0] = 255; configsDetail[phrase].RGBsColor[1] = 0; configsDetail[phrase].RGBsColor[2] = 0;}
    else if(StrEqual(tempBuffer, "{bluegrey}")){configsDetail[phrase].RGBsColor[0] = 176; configsDetail[phrase].RGBsColor[1] = 196; configsDetail[phrase].RGBsColor[2] = 216;}
    else if(StrEqual(tempBuffer, "{blue}")){configsDetail[phrase].RGBsColor[0] = 94; configsDetail[phrase].RGBsColor[1] = 152; configsDetail[phrase].RGBsColor[2] = 216;}
    else if(StrEqual(tempBuffer, "{darkblue}")){configsDetail[phrase].RGBsColor[0] = 76; configsDetail[phrase].RGBsColor[1] = 106; configsDetail[phrase].RGBsColor[2] = 255;}
    else if(StrEqual(tempBuffer, "{purple}")){configsDetail[phrase].RGBsColor[0] = 186; configsDetail[phrase].RGBsColor[1] = 128; configsDetail[phrase].RGBsColor[2] = 240;}
    else if(StrEqual(tempBuffer, "{orchid}")){configsDetail[phrase].RGBsColor[0] = 210; configsDetail[phrase].RGBsColor[1] = 44; configsDetail[phrase].RGBsColor[2] = 228;}
    else if(StrEqual(tempBuffer, "{yellow}")){configsDetail[phrase].RGBsColor[0] = 240; configsDetail[phrase].RGBsColor[1] = 240; configsDetail[phrase].RGBsColor[2] = 128;}
    else if(StrEqual(tempBuffer, "{gold}")){configsDetail[phrase].RGBsColor[0] = 255; configsDetail[phrase].RGBsColor[1] = 196; configsDetail[phrase].RGBsColor[2] = 56;}
    else if(StrEqual(tempBuffer, "{lightgreen}")){configsDetail[phrase].RGBsColor[0] = 190; configsDetail[phrase].RGBsColor[1] = 255; configsDetail[phrase].RGBsColor[2] = 144;}
    else if(StrEqual(tempBuffer, "{green}")){configsDetail[phrase].RGBsColor[0] = 64; configsDetail[phrase].RGBsColor[1] = 255; configsDetail[phrase].RGBsColor[2] = 64;}
    else if(StrEqual(tempBuffer, "{lime}")){configsDetail[phrase].RGBsColor[0] = 160; configsDetail[phrase].RGBsColor[1] = 255; configsDetail[phrase].RGBsColor[2] = 70;}
    else if(StrEqual(tempBuffer, "{grey}")){configsDetail[phrase].RGBsColor[0] = 128; configsDetail[phrase].RGBsColor[1] = 128; configsDetail[phrase].RGBsColor[2] = 128;}
    else if(StrEqual(tempBuffer, "{grey2}")){configsDetail[phrase].RGBsColor[0] = 192; configsDetail[phrase].RGBsColor[1] = 192; configsDetail[phrase].RGBsColor[2] = 192;}
    else if(tempBuffer[0] == '#' && strlen(tempBuffer) == 7) ConvertHTMLToColor(tempBuffer, phrase);
    else {configsDetail[phrase].RGBsColor[0] = 190; configsDetail[phrase].RGBsColor[1] = 255; configsDetail[phrase].RGBsColor[2] = 144;}

    configsDetail[phrase].trigtimer = kvsConsoles.GetNum("timer", -2147483648);

    kvsConsoles.GetString("countdown", tempBuffer, sizeof(tempBuffer), "false");
    configsDetail[phrase].isEnabledCountdown = StrEqual(tempBuffer, "true");

    kvsConsoles.GetString("showtext", tempBuffer, sizeof(tempBuffer), "true");
    configsDetail[phrase].isEnabledTextHud = StrEqual(tempBuffer, "true");

    kvsConsoles.GetString("showhud", tempBuffer, sizeof(tempBuffer), "true");
    configsDetail[phrase].isEnabledCenterHud = StrEqual(tempBuffer, "true");

    configsDetail[phrase].outputTimes = kvsConsoles.GetNum("times", -1);

    kvsConsoles.GetString("initialize", configsDetail[phrase].trigInitConsole, sizeof(configsDetail[].trigInitConsole), "");

    configsDetail[phrase].holdtime = kvsConsoles.GetFloat("holdtime", 1.0);

    kvsConsoles.GetString("instructor", tempBuffer, sizeof(tempBuffer), "false");
    configsDetail[phrase].isEnabledHintHud = StrEqual(tempBuffer, "true");

    kvsConsoles.GetString("caption", tempBuffer, sizeof(tempBuffer), "false")
    configsDetail[phrase].isEnableHintCountdown = StrEqual(tempBuffer, "true");

    kvsConsoles.GetString("target", configsDetail[phrase].hintTarget, sizeof(configsDetail[].hintTarget), "");
    kvsConsoles.GetString("icon", configsDetail[phrase].hintIcon, sizeof(configsDetail[].hintIcon), "icon_tip");
    kvsConsoles.GetString("bind", configsDetail[phrase].hintKeyBinds, sizeof(configsDetail[].hintKeyBinds), "");

    configsDetail[phrase].hintHoldtime = kvsConsoles.GetNum("timeout", 5);
    configsDetail[phrase].hintRange = kvsConsoles.GetFloat("range", 0.0);
}

stock void ExtractMultipliedConsole(const char[] console, int phrase)
{
    int equalsConsole = 0;
    for(int i = SubtractNeverNegative(phrase, 10); i <= phrase; i++)
    {
        if(StrEqual(console, inputConsoles[i]) && configsDetail[phrase].outputTimes >= 0)
        {
            if(equalsConsole >= configsDetail[phrase].outputTimes)
            {
                inputConsoles[phrase][0] = '\0';
                break;
            }
            equalsConsole++;
        }
    }
}