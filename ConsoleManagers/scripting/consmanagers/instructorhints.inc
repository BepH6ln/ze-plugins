#define CONSOLEMANAGERS_INSTRUCTORHINT

stock void GenerateInstructorHint(const char[] console, int phrase, int color[3], bool isTimerWithinFive)
{
    if(!StrEqual(console, ""))
    {
        if(configsDetail[phrase].isEnabledHintHud)
        {
            int entity = CreateEntityByName("env_instructor_hint");
            if(entity <= 0) return;

            char tempBuffer[32];
            DispatchKeyValue(entity, "hint_target", configsDetail[phrase].hintTarget);

            if(!StrEqual(configsDetail[phrase].hintTarget, "")) DispatchKeyValue(entity, "hint_static", "0");
            else DispatchKeyValue(entity, "hint_static", "1");

            DispatchKeyValue(entity, "hint_allow_nodraw_target", "1");

            DispatchKeyValue(entity, "hint_caption", console);

            FormatEx(tempBuffer, sizeof(tempBuffer), "%i %i %i", color[0], color[1], color[2]);
            DispatchKeyValue(entity, "hint_color", tempBuffer);

            DispatchKeyValue(entity, "hint_forcecaption", "1");

            if(isTimerWithinFive && (StrEqual(configsDetail[phrase].hintIcon, "icon_tip") || StrEqual(configsDetail[phrase].hintIcon, "icon_alert")))
            {
                DispatchKeyValue(entity, "hint_icon_onscreen", "icon_alert_red");
                DispatchKeyValue(entity, "hint_icon_offscreen", "icon_alert_red");
            }
            else if(configsDetail[phrase].hintIcon, "use_binding")
            {
                DispatchKeyValue(entity, "hint_icon_onscreen", "use_binding");
                DispatchKeyValue(entity, "hint_icon_offscreen", "icon_alert_red");
            }
            else
            {
                DispatchKeyValue(entity, "hint_icon_onscreen", configsDetail[phrase].hintIcon);
                DispatchKeyValue(entity, "hint_icon_offscreen", configsDetail[phrase].hintIcon);
            }

            DispatchKeyValue(entity, "hint_nooffscreen", "0");

            DispatchKeyValue(entity, "hint_binding", configsDetail[phrase].hintKeyBinds);

            if(isTimerWithinFive) DispatchKeyValue(entity, "hint_shakeoption", "1");
            else DispatchKeyValue(entity, "hint_shakeoption", "0");

            FormatEx(tempBuffer, sizeof(tempBuffer), "%i", configsDetail[phrase].hintHoldtime);
            DispatchKeyValue(entity, "hint_timeout", tempBuffer);

            FormatEx(tempBuffer, sizeof(tempBuffer), "%f", configsDetail[phrase].hintRange);
            DispatchKeyValue(entity, "hint_range", tempBuffer);

            DispatchSpawn(entity);
            AcceptEntityInput(entity, "ShowHint");
        }
    }
}