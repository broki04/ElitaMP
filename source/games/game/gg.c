#include YSI_Coding\y_hooks

new 
    GunGameAllowed[] = {23, 24, 25, 26, 27, 29, 31, 32, 33, 38},
    GunGameLevel[MAX_PLAYERS],
    GunGameStatus[MAX_PLAYERS]
;

stock GG_GetWinner()
{
    new id = -1, level = 0;
    foreach(new i : Game[GAME_GG]) { 
        if(GunGameLevel[i] > level) { 
            level = GunGameLevel[i]; 
            id = i; 
        }
    }
    return id;
}

stock GG_GetWeapon(playerid) return GunGameAllowed[GunGameLevel[playerid]];
stock StartGG()
{
    if(!GameStarted[GAME_GG]) return 1;
    GameStarted[GAME_GG] = true;

    foreach(new i : Game[GAME_GG]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 0.0);

        switch(random(6)) {
            case 0: Teleport(i, false, -1424.9868, 1642.1792, 1052.5313, 46.2537, 14, VW_GG, false);
            case 1: Teleport(i, false, -1453.3512, 1643.2424, 1052.5313, 89.1082, 14, VW_GG, false);
            case 2: Teleport(i, false, -1486.2833, 1630.3832, 1056.5469, 86.6015, 14, VW_GG, false);
            case 3: Teleport(i, false, -1494.3676, 1600.5166, 1052.5313, 171.2023, 14, VW_GG, false);
            case 4: Teleport(i, false, -1440.4066, 1572.3822, 1052.5313, 273.0365, 14, VW_GG, false);
            case 5: Teleport(i, false, -1370.8308, 1608.7809, 1052.5313, 317.8439, 14, VW_GG, false);
        }

        FreezePlayer(i, 3);
        setOdliczanie(i, 3);

        GunGameLevel[i] = GunGameStatus[i] = 0;
        giveWeapon(i, GG_GetWeapon(i), 1000);

        UpdateGameStatus(i, GAME_GG);
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);
    }

    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%d %s)", gameName[GAME_GG], gameCMD[GAME_GG], Iter_Count(Game[GAME_GG]), ConvertTimeFormat(Iter_Count(Game[GAME_GG]), "osób", "osoba", "osoby", "osób"));
    return UpdateGameDraw(), 1;
}

stock UpdateGG()
{
    if(!GameStarted[GAME_GG]) return 1;
    if(Iter_Count(Game[GAME_GG]) < 2) return StopGG(INVALID_PLAYER_ID);

    foreach(new i : Game[GAME_GG]) UpdateGameStatus(i, GAME_GG);
    return 1;
}

stock StopGG(winner = -1)
{
    if(!GameStarted[GAME_GG]) return 1;
    GameStarted[GAME_GG] = false;
    GameCountDown[GAME_GG] = 0;

    if(winner == INVALID_PLAYER_ID) msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a przerwana.", gameName[GAME_GG], gameCMD[GAME_GG]);
    else msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a zakoñczona. Wygrywa: {c}%s(%d){/c}.", gameName[GAME_GG], gameCMD[GAME_GG], getNick(winner), winner);

    foreach(new i : Game[GAME_GG]) {
        pGame[i] = -1;
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_GG]);

    UpdateGameDraw();
    return KillTimer(GameTimer[GAME_GG]);
}

stock RespawnGG(playerid)
{
    ResetPlayerWeapons(playerid);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 0.0);

    switch(random(6)) {
        case 0: Teleport(playerid, false, -1424.9868, 1642.1792, 1052.5313, 46.2537, 14, VW_GG, false);
        case 1: Teleport(playerid, false, -1453.3512, 1643.2424, 1052.5313, 89.1082, 14, VW_GG, false);
        case 2: Teleport(playerid, false, -1486.2833, 1630.3832, 1056.5469, 86.6015, 14, VW_GG, false);
        case 3: Teleport(playerid, false, -1494.3676, 1600.5166, 1052.5313, 171.2023, 14, VW_GG, false);
        case 4: Teleport(playerid, false, -1440.4066, 1572.3822, 1052.5313, 273.0365, 14, VW_GG, false);
        case 5: Teleport(playerid, false, -1370.8308, 1608.7809, 1052.5313, 317.8439, 14, VW_GG, false);
    }
    return giveWeapon(playerid, GG_GetWeapon(playerid), 1000), 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != -1) {
        if(GameStarted[GAME_GG] && Iter_Contains(Game[GAME_GG], playerid) && Iter_Contains(Game[GAME_GG], killerid) && pGame[killerid] == GAME_GG && pGame[playerid] == pGame[killerid]) {
            GunGameStatus[killerid] ++;
            if(GunGameStatus[killerid] >= 2) {
                GunGameLevel[killerid] ++;
                GunGameStatus[killerid] = 0;

                if(GunGameLevel[killerid] >= sizeof(GunGameAllowed)) {
                    new money = math_random(10000, 30000);
                    giveMoney(killerid, money);
                    showGameDraw(killerid, form("~y~WYGRALES(AS) ~r~GUN GAME~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

                    achievement(killerid, ACH_GG);
                    CheckBet(killerid, BET_GAME, true);
                    
                    foreach(new i : Game[GAME_GG]) { if(Iter_Contains(Game[GAME_GG], i) && i != killerid && GameStarted[GAME_GG]) CheckBet(i, BET_GAME, false); }                    
                    StopGG(killerid);
                } else {
                    ResetPlayerWeapons(killerid);
                    addLevelScore(killerid, 50);
                    giveWeapon(killerid, GG_GetWeapon(killerid), 1000);
                }
            }
        }
    }
    return 1;
}