#include YSI_Coding\y_hooks

new 
    WarArenaID,
    WarZoneID,
    Float:WarZonePosition[4],
    Float:WarPosition[2][4],
    WarArenaName[50],
    WarWeapons[3],
    WarTeam,
    WarResult[2]    
;

/*
    co 20 sekund na strefie pojawia siê leczenie (¿ycie, kamizelka).
*/

#define TEAM_ATT        0
#define TEAM_DEF        1

function StartWG()
{
    if(!GameStarted[GAME_WG]) return 1;
    GameStarted[GAME_WG] = true;
    GameCountDown[GAME_WG] = 40 + 5;

    WarResult = {0, 0};
    if(cache_num_rows() > 0) {
        cache_get_value_name_int(0, "id", WarArenaID);
        cache_get_value_name(0, "name", WarArenaName, sizeof(WarArenaName));

        new buffer[90];
        cache_get_value_name(0, "att", buffer);
        sscanf(buffer, "p<,>a<f>[4]", WarPosition[TEAM_ATT]);

        cache_get_value_name(0, "def", buffer);
        sscanf(buffer, "p<,>a<f>[4]", WarPosition[TEAM_DEF]);

        cache_get_value_name(0, "zone", buffer);
        sscanf(buffer, "p<,>a<f>[4]", WarZonePosition);

        cache_get_value_name(0, "weap", buffer);
        sscanf(buffer, "p<,>a<i>[3]", WarWeapons);

        // 

        if(WarZoneID != -1) DestroyZone(WarZoneID);
        WarZoneID = CreateZone(unpackXYZR(WarZonePosition));

        foreach(new i : Game[GAME_WG]) {
            SavePlayerPosition(i);
            ResetPlayerWeapons(i);
            SetHealth(i, 100.0, 100.0);

            FreezePlayer(i, 5);
            setOdliczanie(i, 5);

            ShowZoneForPlayer(i, WarZoneID, setAlpha(C_BLUE, 160));
            ZoneFlashForPlayer(i, WarZoneID, setAlpha(C_RED, 160));

            for(new x = 0; x < 3; x++) giveWeapon(i, WarWeapons[x], 1000);
            playerData[i][player_warTeam] = WarTeam;
            playerData[i][player_god] = false;

            Teleport(i, false, unpackXYZR(WarPosition[(playerData[i][player_warTeam] == TEAM_ATT) ? (TEAM_ATT) : (TEAM_DEF)]), 0, VW_WG);
            SetPlayerColor(i, (playerData[i][player_warTeam] == TEAM_ATT) ? (C_RED) : (C_BLUE));
            SetPlayerTeam(i, playerData[i][player_warTeam]);

            UpdateGameStatus(i, GAME_WG);
            for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

            WarTeam = !WarTeam;
            msg(i, (playerData[i][player_warTeam] == TEAM_ATT) ? (C_RED) : (C_BLUE), "Zosta³eœ(aœ) przydzielony do {c}%s{/c} dru¿yny!", (playerData[i][player_warTeam] == TEAM_ATT) ? ("atakuj¹cej") : ("broni¹cej"));
        }
        msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%s, %d %s)", gameName[GAME_WG], gameCMD[GAME_WG], WarArenaName, Iter_Count(Game[GAME_WG]), ConvertTimeFormat(Iter_Count(Game[GAME_WG]), "osób", "osoba", "osoby", "osób"));
    }
    return 1;
}

stock UpdateWG()
{
    if(!GameStarted[GAME_WG]) return 1;
    if(Iter_Count(Game[GAME_WG]) < 2) return StopWG(true);

    foreach(new i : Game[GAME_WG]) {
        if(!IsPlayerInArea(i, unpackXYZR(WarZonePosition))) {
            new Float:health; GetPlayerHealth(i, health);
            SetPlayerHealth(i, health - 10.0);
            GameTextForPlayer(i, "~r~~h~WROC NA OBSZAR ZABAWY!", 1000, 3);
        }
        UpdateGameStatus(i, GAME_WG);
    }
    return 1;
}

stock RespawnWG(playerid)
{
    ResetPlayerWeapons(playerid);
    SetHealth(playerid, 100.0, 100.0);
    playerData[playerid][player_god] = false;
    for(new x = 0; x < 3; x++) giveWeapon(playerid, WarWeapons[x], 1000);

    Teleport(playerid, false, unpackXYZR(WarPosition[(playerData[playerid][player_warTeam] == TEAM_ATT) ? (TEAM_ATT) : (TEAM_DEF)]), 0, VW_WG);
    SetPlayerColor(playerid, (playerData[playerid][player_warTeam] == TEAM_ATT) ? (C_RED) : (C_BLUE));
    return SetPlayerTeam(playerid, playerData[playerid][player_warTeam]), 1;
}

stock StopWG(bool:none)
{
    if(!GameStarted[GAME_WG]) return 1;
    GameStarted[GAME_WG] = false;
    GameCountDown[GAME_WG] = 0;

    if(none) msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a przerwana.", gameName[GAME_WG], gameCMD[GAME_WG]);
    else {
        if(WarResult[TEAM_ATT] > WarResult[TEAM_DEF]) {
            foreach(new i : Game[GAME_WG]) {
                if(playerData[i][player_warTeam] == TEAM_ATT) {
                    new money = math_random(10000, 30000);
                    giveMoney(i, money);
                    showGameDraw(i, form("~y~WYGRALES(AS) ~r~WAR GAME~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));
                    
                    achievement(i, ACH_WG);
                    CheckBet(i, BET_GAME, true);
                }
                else CheckBet(i, BET_GAME, false);
            }
            msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a zakoñczona zwyciêstwem {c}atakuj¹cej{/c} dru¿yny. "LY"(%d:%d)", gameName[GAME_WG], gameCMD[GAME_WG], WarResult[0], WarResult[1]);
        } else if(WarResult[TEAM_DEF] > WarResult[TEAM_ATT]) {
            foreach(new i : Game[GAME_WG]) {
                if(playerData[i][player_warTeam] == TEAM_DEF) {
                    new money = math_random(10000, 30000);
                    giveMoney(i, money);
                    showGameDraw(i, form("~y~WYGRALES(AS) ~b~~h~WAR GAME~y~!~n~~y~NAGRODA: ~b~~h~%s$", strdot(money)));
                    
                    achievement(i, ACH_WG);
                    CheckBet(i, BET_GAME, true);
                }
                else CheckBet(i, BET_GAME, false);
            }
            msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a zakoñczona zwyciêstwem {c}broni¹cej{/c} dru¿yny. "LY"(%d:%d)", gameName[GAME_WG], gameCMD[GAME_WG], WarResult[0], WarResult[1]);
        } else {
            foreach(new i : Game[GAME_WG]) {
                if(playerData[i][player_warTeam] == TEAM_DEF) {
                    new money = math_random(3000, 10000);
                    giveMoney(i, money);
                    showGameDraw(i, form("~y~WAR GAME ZAKONCZONY ~r~REMISEM~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

                    CheckBet(i, BET_GAME, false);
                }
            }
            msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a zakoñczona remisem. "LY"(%d:%d)", gameName[GAME_WG], gameCMD[GAME_WG], WarResult[0], WarResult[1]);
        }
    }

    WarResult = {0, 0};
    foreach(new i : Game[GAME_WG]) {
        pGame[i] = -1;
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_WG]);

    UpdateGameDraw();
    return KillTimer(GameTimer[GAME_WG]);
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(CheckPlayerGame(playerid, GAME_WG) && CheckPlayerGame(killerid, GAME_WG) && playerData[playerid][player_warTeam] != playerData[killerid][player_warTeam]) {
            giveMoney(playerid, 100);
            msg(killerid, C_DRED, "Otrzymujesz {c}100${/c} za zabicie na zabawie {c}%s (%s){/c}!", gameName[GAME_WG], gameCMD[GAME_WG]);
            WarResult[(playerData[killerid][player_warTeam] == 0) ? (0) : (1)] ++;
        }
    }
    return 1;
}

stock GetWarTeam(teamid)
{
    new string[90];
    switch(teamid) {
        case TEAM_ATT: string = "~r~~h~Atakujacy";
        case TEAM_DEF: string = "~b~~h~~h~Broniacy";
    }
    return string;
}