#define CONSOLEMANAGERS_CLIENTCOMMANDS

public void ClientCommands_OnPluginStart()
{
    RegConsoleCmd("sm_chud", OpenConsManagersClientMenu);
}

Action OpenConsManagersClientMenu(int client, int args)
{
    Menu clientMenu = new Menu(clientMenuHandle, MENU_ACTIONS_ALL);
    clientMenu.SetTitle("ConsoleManagers Settings");

    switch(clientSettings[client].language)
    {
        case 0: clientMenu.AddItem("LANGUAGE", "Console Language: [English]");
        case 1: clientMenu.AddItem("LANGUAGE", "コンソール言語：[日本語]");
        case 2: clientMenu.AddItem("LANGUAGE", "地图消息语言：[简体中文]");
        case 3: clientMenu.AddItem("LANGUAGE", "地圖消息語言：[繁體中文]");
        case 4: clientMenu.AddItem("LANGUAGE", "지도메시지의언어：[한국어]");
    }

    clientMenu.AddItem("", "", ITEMDRAW_IGNORE);

    char tempBuffer[128];
    switch(clientSettings[client].displayStyle)
    {
        case 0: clientMenu.AddItem("HUDSTYLES", "Hud Style: [OFF]");
        case 1:
        {
            clientMenu.AddItem("HUDSTYLES", "Hud Style: [GameText]");

            FormatEx(tempBuffer, sizeof(tempBuffer), "Hud Position: [%.3f %.3f]", clientSettings[client].hudPosition[0], clientSettings[client].hudPosition[1]);
            clientMenu.AddItem("HUDPOSITION", tempBuffer);
        }
        case 2:
        {
            clientMenu.AddItem("HUDSTYLES", "Hud Style: [CenterHud]");
            if(!clientSettings[client].isEnabledColorSync) clientMenu.AddItem("COLORSYNCS", "Color Sync: [Disable]");
            else
            {
                clientMenu.AddItem("COLORSYNCS", "Color Sync: [Enable]");

                FormatEx(tempBuffer, sizeof(tempBuffer), "Timer Color: [%i %i %i]", clientSettings[client].RGBsColor[0], clientSettings[client].RGBsColor[1], clientSettings[client].RGBsColor[2]);
                clientMenu.AddItem("TIMERCOLOR", tempBuffer);
            }
        }
    }

    clientMenu.Display(client, MENU_TIME_FOREVER);
    return Plugin_Handled;
}

Action OpenMenuChangeHudPos(int client)
{
    Menu hudposMenu = new Menu(hudposMenuHandle, MENU_ACTIONS_ALL);
    hudposMenu.SetTitle("ConsoleManagers Settings");

    hudposMenu.AddItem("MOVERIGHT", "Move to right");
    hudposMenu.AddItem("MOVELEFT", "Move to left");
    hudposMenu.AddItem("MOVEUP", "Move to up");
    hudposMenu.AddItem("MOVEDOWN", "Move to down");
    hudposMenu.AddItem("MOVERIGHTBIT", "Move to right a bit");
    hudposMenu.AddItem("MOVELEFTBIT", "Move to left a bit");
    hudposMenu.AddItem("MOVEUPBIT", "Move to up a bit");
    hudposMenu.AddItem("MOVEDOWNBIT", "Move to down a bit");
    hudposMenu.ExitBackButton = true;

    hudposMenu.Display(client, MENU_TIME_FOREVER);
    return Plugin_Handled;
}

int clientMenuHandle(Menu menu, MenuAction action, int client, int item)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char menuItem[16];
            GetMenuItem(menu, item, menuItem, sizeof(menuItem));

            if(StrEqual(menuItem, "LANGUAGE")) ClientCookieLanguage(client, clientSettings[client].language < 4 ? clientSettings[client].language + 1 : 0);
            else if(StrEqual(menuItem, "HUDSTYLES")) ClientCookieHudStyles(client, clientSettings[client].displayStyle < 2 ? clientSettings[client].displayStyle + 1 : 0);
            else if(StrEqual(menuItem, "HUDPOSITION")) OpenMenuChangeHudPos(client);
            else if(StrEqual(menuItem, "COLORSYNCS")) ClientCookieColorSync(client, clientSettings[client].isEnabledColorSync ? false : true);
            // else if(StrEqual(menuItem, "TIMERCOLOR")) OpenMenuChangeTimerColor(client);
        }
        case MenuAction_End:
        {
            delete menu;
        }
    }
    return 0;
}

int hudposMenuHandle(Menu menu, MenuAction action, int client, int item)
{
    switch(action)
    {
        case MenuAction_Select:
        {
            char menuItem[16];
            menu.GetItem(item, menuItem, sizeof(menuItem));

            if(StrEqual(menuItem, "MOVERIGHT")) ClientCookieHudPos(client, 1);
            else if(StrEqual(menuItem, "MOVELEFT")) ClientCookieHudPos(client, 2);
            else if(StrEqual(menuItem, "MOVEUP")) ClientCookieHudPos(client, 3);
            else if(StrEqual(menuItem, "MOVEDOWN")) ClientCookieHudPos(client, 4);
            else if(StrEqual(menuItem, "MOVERIGHTBIT")) ClientCookieHudPos(client, 5);
            else if(StrEqual(menuItem, "MOVELEFTBIT")) ClientCookieHudPos(client, 6);
            else if(StrEqual(menuItem, "MOVEUPBIT")) ClientCookieHudPos(client, 7);
            else if(StrEqual(menuItem, "MOVEDOWNBIT")) ClientCookieHudPos(client, 8);
        }
        case MenuAction_Cancel:
        {
            OpenConsManagersClientMenu(client, 1);
        }
        case MenuAction_DrawItem:
        {
            char menuItem[16];
            menu.GetItem(item, menuItem, sizeof(menuItem));

            if(StrEqual(menuItem, "MOVERIGHT") && clientSettings[client].hudPosition[0] > 0.95) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVELEFT") && clientSettings[client].hudPosition[0] < 0.05) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVEUP") && clientSettings[client].hudPosition[1] < 0.05) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVEDOWN") && clientSettings[client].hudPosition[1] > 0.86) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVERIGHTBIT") && clientSettings[client].hudPosition[0] > 0.99) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVELEFTBIT") && clientSettings[client].hudPosition[0] < 0.01) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVEUPBIT") && clientSettings[client].hudPosition[1] < 0.01) return ITEMDRAW_DISABLED;
            else if(StrEqual(menuItem, "MOVEDOWNBIT") && clientSettings[client].hudPosition[1] > 0.9) return ITEMDRAW_DISABLED;
            else return ITEMDRAW_DEFAULT;
        }
    }
    return 0;
}

stock bool ClientCookieLanguage(int client, int language)
{
    if(IsValidClient(client))
    {
        clientSettings[client].language = language;

        char cookieValue[2];
        IntToString(language, cookieValue, sizeof(cookieValue));
        SetClientCookie(client, selectLanguage, cookieValue);
        OpenConsManagersClientMenu(client, 1);

        switch(language)
        {
            case 0: CPrintToChat(client, "%s[ConsManagers] %sYou changed the console language to [English].", "{lime}", "{gold}");
            case 1: CPrintToChat(client, "%s[ConsManagers] %sコンソールの言語を [日本語] に変更しました.", "{lime}", "{gold}");
            case 2: CPrintToChat(client, "%s[ConsManagers] %s您的地图消息语言已切换为 [简体中文].", "{lime}", "{gold}");
            case 3: CPrintToChat(client, "%s[ConsManagers] %s您的地圖消息語言已切換為 [繁體中文].", "{lime}", "{gold}");
            case 4: CPrintToChat(client, "%s[ConsManagers] %s맵메시지의언어가 [한국어] 로변경되었습니다. ", "{lime}", "{gold}");
        }
        return true;
    }
    return false;
}

stock bool ClientCookieHudStyles(int client, int hudstyle)
{
    if(IsValidClient(client))
    {
        clientSettings[client].displayStyle = hudstyle;

        char cookieValue[2];
        IntToString(hudstyle, cookieValue, sizeof(cookieValue));
        SetClientCookie(client, toggleDisplayStyle, cookieValue);
        OpenConsManagersClientMenu(client, 1);

        switch(hudstyle)
        {
            case 0: CPrintToChat(client, "%s[ConsManager] %sYou changed the console Hud style: %s[OFF].", "{lime}", "{gold}", "{red}");
            case 1: CPrintToChat(client, "%s[ConsManager] %sYou changed the console Hud style: [GameText].", "{lime}", "{gold}");
            case 2: CPrintToChat(client, "%s[ConsManager] %sYou changed the console Hud style: [CenterHud].", "{lime}", "{gold}");
        }
        return true;
    }
    return false;
}

stock bool ClientCookieHudPos(int client, int iteminfo)
{
    if(IsValidClient(client))
    {
        char newHudPosValue[32];
        float oldHudPosX = clientSettings[client].hudPosition[0];
        float oldHudPosY = clientSettings[client].hudPosition[1];

        switch(iteminfo)
        {
            case 1: clientSettings[client].hudPosition[0] = oldHudPosX + 0.05;
            case 2: clientSettings[client].hudPosition[0] = oldHudPosX - 0.05;
            case 3: clientSettings[client].hudPosition[1] = oldHudPosY - 0.05;
            case 4: clientSettings[client].hudPosition[1] = oldHudPosY + 0.05;
            case 5: clientSettings[client].hudPosition[0] = oldHudPosX + 0.01;
            case 6: clientSettings[client].hudPosition[0] = oldHudPosX - 0.01;
            case 7: clientSettings[client].hudPosition[1] = oldHudPosY - 0.01;
            case 8: clientSettings[client].hudPosition[1] = oldHudPosY + 0.01;
        }

        Format(newHudPosValue, sizeof(newHudPosValue), "%f/%f", clientSettings[client].hudPosition[0], clientSettings[client].hudPosition[1]);
        SetClientCookie(client, textHudPosition, newHudPosValue);
        OpenMenuChangeHudPos(client);
        return true;
    }
    return false;
}

stock bool ClientCookieColorSync(int client, bool hasColorSync)
{
    if(IsValidClient(client))
    {
        clientSettings[client].isEnabledColorSync = hasColorSync;
        SetClientCookie(client, toggleDisplayStyle, clientSettings[client].isEnabledColorSync ? "1":"0");
        OpenConsManagersClientMenu(client, 1);

        if(hasColorSync) CPrintToChat(client, "%s[ConsManager] %sYou changed the color sync for timers: %s[Enabled].", "{lime}", "{gold}", "{green}");
        else CPrintToChat(client, "%s[ConsManager] %sYou changed the color sync for timers: %s[Disabled].", "{lime}", "{gold}", "{red}");
        return true;
    }
    return false;
}