new const hudOptions[][] = {"Wszystko", "Pasek", "Gotówka", "Poziom", "Zabójstwa", "Œmierci", "Czas gry", "Pojazd", "Pasek poziomu", "Testy reakcji", "Zadanie dnia", "Osi¹gniêcia", "Lista zabaw", "Proces ³owienia", "Bilard - moc uderzenia"};
new pHud[MAX_PLAYERS][sizeof(hudOptions)];
new bool:pDrawVisible[MAX_PLAYERS] = true;

stock SetPlayerDraw(playerid, bool:toggle) 
{
    for(new i = 0; i < 5; i++) {
        toggleTextDraw(tdBox[i], playerid, toggle);
        togglePlayerTextDraw(tdPlayer[i], playerid, toggle); }
    for(new i = 0; i < 2; i++) toggleTextDraw(tdOnline[i], playerid, toggle);
    for(new i = 0; i < 6; i++) togglePlayerTextDraw(tdPerformance[i], playerid, toggle);

    togglePlayerTextDraw(tdMoney[0], playerid, toggle);
    toggleTextDraw(tdLogo, playerid, toggle);
    toggleTextDraw(tdAreny, playerid, toggle);
    toggleTextDraw(tdMessage, playerid, toggle);
    toggleTextDraw(tdPasek[playerid], playerid, toggle);
    toggleTextDraw(tdDate, playerid, toggle);
    togglePlayerTextDraw(tdGameList, playerid, toggle);
    toggleTextDraw(tdGameLimit, playerid, toggle);

    if(quizData[quiz_active]) {
        UpdateQuizDraw(playerid);
        togglePlayerTextDraw(tdQuiz, playerid, toggle); }
    if(pDailyQuest[playerid] != -1 && !pDailyDone[playerid]) togglePlayerTextDraw(tdQuest, playerid, toggle);
    if(walizkaPickup != -1) toggleTextDraw(tdWalizka, playerid, toggle);
    if(bombaPickup != -1) toggleTextDraw(tdBomba, playerid, toggle);

    PlayerTextDrawColor(playerid, tdPosition, GetPlayerColor(playerid));
    PlayerTextDrawShow(playerid, tdPosition);
    return UpdateHud(playerid), 1;
}

stock getHud(playerid) 
{
    new hudstr[512];
    for(new i = 0; i < sizeof(hudOptions); i++) format(hudstr, sizeof(hudstr), "%s%s%d", hudstr, i == 0 ? ("") : (","), pHud[playerid][i]);
    return hudstr;
}

alias:td("textdraw", "szata", "grafika")
CMD:td(playerid)
{
    s[0] = EOS;
    catstr(s, C_BLUE2, -1, "Widocznoœæ szaty graficznej: {c}%s{/c}\n", (pDrawVisible[playerid]) ? (""GREEN"(ON)") : (""RED"(OFF)"));
    catstr(s, C_BLUE2, -1, "Zarz¹dzanie szat¹ graficzn¹\n");

    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii
        if(!rr) return 1;

        switch(ll) {
            case 0: {
                pDrawVisible[playerid] = !pDrawVisible[playerid];
                SetPlayerDraw(playerid, pDrawVisible[playerid]);
                m_pquery_format("UDPATE es_accounts SET draw_visible=%d WHERE id=%d LIMIT 1;", pDrawVisible[playerid]);
                callcmd::td(playerid);
            }
            case 1: callcmd::hud(playerid);
        }
        return 1;
    }
    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""YELLOW"Szata graficzna", s, "Wybierz", "Zamknij"), 1;
}

alias:hud("pasek")
CMD:hud(playerid) 
{
    s[0] = EOS;
    for(new i = 0; i < sizeof(hudOptions); i++) format(s, sizeof(s), "%s"WHITE"»  {%06x}%s\n", s, pHud[playerid][i] >>> 8, hudOptions[i]);
    return showDialog(playerid, DIALOG_HUD, DIALOG_STYLE_LIST, ""YELLOW"Zarz¹dzanie szat¹ graficzn¹", s, "Wybierz", "Zamknij");
}

stock changePlayerHudColor(playerid, PlayerText:draw, colour, bool:box = false, alpha = 255) 
{
    new bool:visible = bool:IsPlayerTextDrawVisible(playerid, draw);
    togglePlayerTextDraw(draw, playerid, false);
    setPlayerTextDrawColor(draw, playerid, box, (box) ? (setAlpha(colour, alpha)) : (colour));
    if(visible) togglePlayerTextDraw(draw, playerid, true);
    return 1;
}

stock changeHudColor(playerid, Text:draw, colour, bool:box = false, alpha = 255) 
{
    new bool:visible = bool:IsTextDrawVisibleForPlayer(playerid, draw);
    toggleTextDraw(draw, playerid, false);
    setTextDrawColor(draw, box, (box) ? (setAlpha(colour, alpha)) : (colour));
    if(visible) toggleTextDraw(draw, playerid, true);
    return 1;
}

stock changeBarColor(playerid, PlayerBar:bar, colour) 
{
    new bool:visible = IsPlayerProgressBarVisible(playerid, bar);
    HidePlayerProgressBar(playerid, bar);
    SetPlayerProgressBarColour(playerid, bar, colour);
    if(visible) ShowPlayerProgressBar(playerid, bar);
    return 1;
}

stock UpdateHud(playerid)
{
    changeHudColor(playerid, tdPasek[playerid], pHud[playerid][1], true);
    changePlayerHudColor(playerid, tdMoney[0], pHud[playerid][2]);

    for(new i = 1; i < 5; i++) changePlayerHudColor(playerid, tdPlayer[i], pHud[playerid][i + 2]);
    for(new i = 1; i < 4; i++) changePlayerHudColor(playerid, tdLicznik[i], pHud[playerid][7]);

    changeBarColor(playerid, LevelBar[playerid], pHud[playerid][8]);
    changePlayerHudColor(playerid, tdQuiz, pHud[playerid][9]);
    changePlayerHudColor(playerid, tdQuest, pHud[playerid][10]);
    changePlayerHudColor(playerid, tdAchievement[1], pHud[playerid][11], true, 150);
    changePlayerHudColor(playerid, tdGameList, pHud[playerid][12]);
    changeBarColor(playerid, FishBar[playerid], pHud[playerid][13]);
    changeBarColor(playerid, PoolBar[playerid], pHud[playerid][14]);
    return UpdateGameDraw(), 1;
}

stock OnDialogHud(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_HUD: {
            if(!response) return 1;
            playerData[playerid][player_saveData][0] = listitem;

            s[0] = EOS;
            for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
            return showDialog(playerid, DIALOG_HUD_COLOR, DIALOG_STYLE_LIST, ""BLUE"Wybierz kolor", #, "Ustaw", "Cofnij", 10);
        }

        case DIALOG_HUD_COLOR: {
            if(!response) return callcmd::hud(playerid);

            if(listitem != INVALID_LISTITEM) {
                new optionid = playerData[playerid][player_saveData][0];
                if(optionid == 0) { for(new i = 0; i < sizeof(hudOptions); i++) pHud[playerid][i] = KoloryGraczy[listitem]; }
                else pHud[playerid][optionid] = KoloryGraczy[listitem];

                m_pquery_format("UPDATE es_accounts SET hud='%s' WHERE id=%d LIMIT 1;", getHud(playerid), playerData[playerid][player_accountID]);
                UpdateHud(playerid);
            }
            return 1;
        }
    }
    return 0;
}
