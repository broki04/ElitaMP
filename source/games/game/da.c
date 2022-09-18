#include YSI_Coding\y_hooks

enum e_deathArenaKills {
    da_id,
    da_kills,
    Float:da_damage
};

new
    deathArenaKills[MAX_PLAYERS][e_deathArenaKills],
    deathArenaInterior,
    deathArenaID = -1,
    deathArenaPlace[MAX_PLAYERS],

    Float:deathPosLVPD[][] = {
        {299.7782, 173.2495, 1007.1719},
        {221.2334, 150.3305, 1003.0234},
        {210.7774, 186.4514, 1003.0313},
        {249.1874, 160.9782, 1003.0234},
        {205.3420, 157.3225, 1003.0234},
        {228.8251, 180.7334, 1003.0313}
    },

    Float:deathPosWH[][] = {
        {1363.2341, -43.2493, 1000.9198},
        {1362.2455, -9.3901, 1000.9219},
        {1404.5674, -0.2907, 1000.9125},
        {1395.2415, -20.9539, 1000.9175},
        {1406.3846, -34.2408, 1001.8218},
        {1386.7581, -26.4076, 1000.9554},
        {1360.5614, -0.0812, 1007.8828},
        {1391.8356, -35.8512, 1007.8806},
        {1417.1887, -29.0298, 1007.8925}
    }
;

stock StartDA()
{
    if(!GameStarted[GAME_DA]) return 1;
    GameStarted[GAME_DA] = true;
    GameCountDown[GAME_DA] = 200;
    deathArenaID = random(2);
    deathArenaInterior = (deathArenaID == 0) ? (3) : (1);

    foreach(new i : Game[GAME_DA]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 100.0);

        deathArenaPlace[i] = 0;
        deathArenaKills[i][da_id] = -1;
        deathArenaKills[i][da_kills] = 0;
        deathArenaKills[i][da_damage] = 0.0;

        switch(deathArenaID) {
            case 0: {
                new rand = random(sizeof(deathPosLVPD));
                Teleport(i, false, unpackXYZ(deathPosLVPD[rand]), 0.0, deathArenaInterior, VW_DA, false);
            }

            case 1: {
                new rand = random(sizeof(deathPosWH));
                Teleport(i, false, unpackXYZ(deathPosWH[rand]), 0.0, deathArenaInterior, VW_DA, false);
            } 
        }
        UpdateGameStatus(i, GAME_DA);

        playerData[i][player_god] = true;
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);
        SetTimerEx("DeleteGodDA", 1000 * 2, false, "d", i);
    }

    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%d %s)", gameName[GAME_DA], gameCMD[GAME_DA], Iter_Count(Game[GAME_DA]), ConvertTimeFormat(Iter_Count(Game[GAME_DA]), "osób", "osoba", "osoby", "osób"));
    return UpdateGameDraw(), 1;
}

stock RespawnDA(playerid)
{
    ResetPlayerWeapons(playerid);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);

    switch(deathArenaID) {
        case 0: {
            new rand = random(sizeof(deathPosLVPD));
            Teleport(playerid, false, unpackXYZ(deathPosLVPD[rand]), 0.0, deathArenaInterior, VW_DA, false);
        }
        case 1: {
            new rand = random(sizeof(deathPosWH));
            Teleport(playerid, false, unpackXYZ(deathPosWH[rand]), 0.0, deathArenaInterior, VW_DA, false);
        } 
    }

    playerData[playerid][player_god] = true;
    return SetTimerEx("DeleteGodDA", 1000 * 2, false, "d", playerid), 1;
}

function DeleteGodDA(playerid) 
{
    giveWeapon(playerid, WEAPON_DEAGLE, 1000);
    giveWeapon(playerid, WEAPON_SHOTGUN, 1000);
    giveWeapon(playerid, WEAPON_SNIPER, 1000);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    return playerData[playerid][player_god] = false, 1;
}

stock UpdateDA()
{
    if(!GameStarted[GAME_DA]) return 1;
    if(Iter_Count(Game[GAME_DA]) < 2) return StopDA(false);

    foreach(new i : Game[GAME_DA]) UpdateGameStatus(i, GAME_DA);
    return 1;
}

stock StopDA(bool:time)
{
    if(!GameStarted[GAME_DA]) return 1;
    GameStarted[GAME_DA] = false;
    GameCountDown[GAME_DA] = 0;

    if(time) {
        s[0] = EOS;
        s = ""BLACK"X\t"WHITE"Nick\t"RED"Statystyki\n";

        new place = 0;
        SortDeepArray(deathArenaKills, da_damage, .order=SORT_DESC);
        for(new i = 0; i < Iter_Count(Game[GAME_DA]); i++) {
            if(!IsPlayerConnected(deathArenaKills[i][da_id])) continue;
            
            new playerid = deathArenaKills[i][da_id], kills = deathArenaKills[i][da_kills], Float:damage = deathArenaKills[i][da_damage];
            deathArenaPlace[playerid] = place ++;
            format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s(%d)\t"RED"%d kills "LRED"(%.1f damage)\n", s, i + 1, getNick(playerid), playerid, kills, damage);
        }
    }

    foreach(new i : Game[GAME_DA]) {
        if(time) {
            new money;
            switch(deathArenaPlace[i]) {
                case 1: {
                    money = math_random(50000, 70000);
                    achievement(i, ACH_DA);
                    CheckBet(i, BET_GAME, true);
                }

                case 2: money = math_random(30000, 50000);
                case 3: money = math_random(10000, 30000);
                default: money = math_random(5000, 10000);
            }
        
            if(deathArenaPlace[i] != 1) CheckBet(i, BET_GAME, false);
            giveMoney(i, money);

            msg(i, C_CHAT, "Otrzymujesz {c}%s${/c} za zajêcie {c}#%d{/c} miejsca!", strdot(money), deathArenaPlace[i]);
            showDialog(i, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Podsumowanie /DA", s, "OK", #);
        }

        LoadPlayerPosition(i);
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        pGame[i] = -1;
    }
    Iter_Clear(Game[GAME_DA]);

    UpdateGameDraw();
    KillTimer(GameTimer[GAME_DA]);
    return msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona.", gameName[GAME_DA], gameCMD[GAME_DA]), 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(pGame[killerid] == GAME_DA && Iter_Contains(Game[GAME_DA], killerid) && GameStarted[GAME_DA] && pGame[killerid] == pGame[playerid]) deathArenaKills[killerid][da_kills] ++;
    }
    return 1;
}