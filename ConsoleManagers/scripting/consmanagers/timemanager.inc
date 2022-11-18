#define CONSOLEMANAGERS_TIMERMANAGERS

static int timeRoundStart;

void SaveTimeWhenRoundStart()
{
    timeRoundStart = GetTime();
}

stock int SaveRoundTime()
{
    return GameRules_GetProp("m_iRoundTime");
}

stock int SaveCurrentTime()
{
    Handle hasFreezeTime = FindConVar("mp_freezetime");
    int lengthFreezeTime = GetConVarInt(hasFreezeTime);

    return SaveRoundTime() - ((GetTime() - timeRoundStart) - lengthFreezeTime);
}

stock int ExportTimeWhenTrigEnds(int phrase)
{
    return SaveCurrentTime() - configsDetail[phrase].trigtimer;
}