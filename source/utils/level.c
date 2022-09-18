new LevelTimer[MAX_PLAYERS];
#define setLevel SetPlayerScore

stock addLevelScore(playerid, amount)
{  
    playerData[playerid][player_score] += amount;

    new reszta = 0;
    if(playerData[playerid][player_score] >= playerData[playerid][player_score_needed]) {        
        reszta = playerData[playerid][player_score] - playerData[playerid][player_score_needed];
        playerData[playerid][player_score_needed] = playerData[playerid][player_score_needed] + 500;
        playerData[playerid][player_score] = reszta;

        playerData[playerid][player_level] ++; 
        playerData[playerid][player_levelPoints] ++;

        setLevel(playerid, playerData[playerid][player_level]);
        msg(playerid, C_LBLUE, "Awansowa³eœ(aœ) na {c}%s{/c} poziom postaci! Punkty umiejêtnoœci: {c}%s{/c}.", strdot(playerData[playerid][player_level]), strdot(playerData[playerid][player_levelPoints]));
        m_pquery_format("UPDATE es_accounts SET score=%d, score_needed=%d, level=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_score], playerData[playerid][player_score_needed], playerData[playerid][player_level], playerData[playerid][player_accountID]);
    } 

    ShowPlayerProgressBar(playerid, LevelBar[playerid]);
    for(new i = 0; i < 3; i++) PlayerTextDrawShow(playerid, tdLevel[i]);
    return UpdateLevelBar(playerid, amount);
}

stock UpdateLevelBar(playerid, amount)
{
    if(IsPlayerProgressBarVisible(playerid, LevelBar[playerid]) && LevelTimer[playerid] != -1) KillTimer(LevelTimer[playerid]);

    SetPlayerProgressBarValue(playerid, LevelBar[playerid], float(playerData[playerid][player_score]));
    SetPlayerProgressBarMaxValue(playerid, LevelBar[playerid], float(playerData[playerid][player_score_needed]));

    playerTextDrawSetString(playerid, tdLevel[0], "~w~~h~%s", strdot(playerData[playerid][player_level]));
    playerTextDrawSetString(playerid, tdLevel[1], "~w~~h~%s", strdot(playerData[playerid][player_level] + 1));
    playerTextDrawSetString(playerid, tdLevel[2], "~w~~h~%s ~r~~h~/ ~w~~h~%s exp ~g~~h~~h~(+%s)", strdot(playerData[playerid][player_score]), strdot(playerData[playerid][player_score_needed]), strdot(amount));

    LevelTimer[playerid] = SetTimerEx("HideLevelBar", 1000 * 7, false, "d", playerid);
    return 1;
}

function HideLevelBar(playerid)
{
    KillTimer(LevelTimer[playerid]);
    for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdLevel[i]);
    return HidePlayerProgressBar(playerid, LevelBar[playerid]);
}
