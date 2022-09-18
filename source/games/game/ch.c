#include YSI_Coding\y_hooks

enum e_chowanyDataArena {
    chowany_name[25],
    Float:chowany_area[4],
    Float:chowany_seeker[4],
    Float:chowany_hideer[4],
    chowany_interior,
};

new 
    chowanyDataArena[][e_chowanyDataArena] = {
        {"Arena #1",    {2527.5, 1076.5, 2606.5, 1193.5},       {2574.7505, 1112.8290, 10.8203, 0.1080},        {2575.0850, 1105.6218, 10.8203, 178.9997},      0},
        {"Arena #2",    {-2699.5, 41.5, -2599.5, 152.5},        {-2761.7317, -6.7885, 6.8969, 269.5558},        {-2760.6216, -16.2814, 6.8980, 182.9288},       0},
        {"Arena #3",    {-626.5, -569.5, -459.5, -462.5},       {-513.6683, -502.6236, 25.5234, 268.7472},      {-568.4553, -501.3104, 25.5234, 87.4384},       0},
        {"Arena #4",    {-1133.5, -767.5, -969.5, -582.5},      {-1015.2912, -685.6987, 32.0078, 8.9075},       {-1036.3887, -713.5793, 32.0078, 85.4248},      0},
        {"Arena #5",    {0.0, 0.0, 0.0, 0.0},                   {367.7560, 173.7912, 1008.3828, 89.3844},       {380.8548, 173.9036, 1008.3828, 91.6822},       3},
        {"Arena #6",    {0.0, 0.0, 0.0, 0.0},                   {246.6695, 65.8039, 1003.6406, 0.0000},         {246.6376, 75.1803, 1003.6406, 358.1618},       6},
        {"Arena #7",    {0.0, 0.0, 0.0, 0.0},                   {1994.7295, 1017.7973, 994.8906, 91.2227},      {1977.6738, 998.6609, 994.4688, 149.3570},      10},
        {"Arena #8",    {0.0, 0.0, 0.0, 0.0},                   {237.5798, 112.6969, 1003.2188, 354.7801},      {246.3602, 114.1560, 1003.2188, 356.7831},      10},
        {"Arena #9",    {0.0, 0.0, 0.0, 0.0},                   {2235.6282, 1677.3036, 1008.3594, 180.1464},    {2235.9702, 1649.9536 ,1008.3594, 180.1464},    1}
    },

    ChowanySzukajacy = -1,
    ChowanyZone = -1,
    ChowanyArenaID = -1
;

stock StartChowany()
{
    if(!GameStarted[GAME_CH]) return 1;
    GameStarted[GAME_CH] = true;
    GameCountDown[GAME_CH] = 200 + 15;

    ChowanySzukajacy = Iter_Random(Game[GAME_CH]);
    ChowanyArenaID = random(sizeof(chowanyDataArena));
    
    if(chowanyDataArena[ChowanyArenaID][chowany_interior] == 0) {
        ChowanyZone = CreateZone(unpackXYZR(chowanyDataArena[ChowanyArenaID][chowany_area]));
        CreateZoneBorders(ChowanyZone);
    }
    
    foreach(new i : Game[GAME_CH]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 100.0);

        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0x00));
        HideNameTagForAll(i);

        UpdateGameStatus(i, GAME_CH);
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

        if(!chowanyDataArena[ChowanyArenaID][chowany_interior]) {
            ShowZoneForPlayer(i, ChowanyZone, setAlpha(C_RED, 160));
            ZoneFlashForPlayer(i, ChowanyZone, setAlpha(C_YELLOW, 160));
        }

        if(ChowanySzukajacy == i) {
            SetPlayerTeam(i, 0);
            FreezePlayer(i, 15);
            giveWeapon(i, WEAPON_MINIGUN, 9999);
            showGameDraw(i, "~p~ZOSTALES(AS) WYBRANY(A) JAKO ~w~~h~OSOBA SZUKAJACA~p~!~n~~p~BEDZIESZ MOGL SZUKAC PO UPLYWIE: ~w~~h~15 ~p~SEKUND!", 15);
            Teleport(i, false, unpackXYZR(chowanyDataArena[ChowanyArenaID][chowany_seeker]), chowanyDataArena[ChowanyArenaID][chowany_interior], VW_CH, false);
        } else {
            SetPlayerTeam(i, 1);
            showGameDraw(i, "~p~JESTES ~w~~h~OSOBA CHOWAJACA~p~!~n~~p~MASZ ~w~~h~15 ~p~NA ZNALEZIENIE DOBREJ KRYJOWKI!", 15);
            Teleport(i, false, unpackXYZR(chowanyDataArena[ChowanyArenaID][chowany_hideer]), chowanyDataArena[ChowanyArenaID][chowany_interior], VW_CH, false);
        }
        setOdliczanie(i, 15);
    }

    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%s, %d %s)", gameName[GAME_CH], gameCMD[GAME_CH], chowanyDataArena[ChowanyArenaID][chowany_name], Iter_Count(Game[GAME_CH]), ConvertTimeFormat(Iter_Count(Game[GAME_CH]), "osób", "osoba", "osoby", "osób"));
    return UpdateGameDraw(), 1;
}

stock UpdateChowany()
{
    if(!GameStarted[GAME_CH]) return 1;

    if(Iter_Count(Game[GAME_CH]) < 1) return StopChowany(GAME_STOP_NULL);
    if(Iter_Count(Game[GAME_CH]) == 1) return StopChowany(GAME_STOP_PLAYERS);
    if(!IsPlayerConnected(ChowanySzukajacy) || ChowanySzukajacy == -1) {
        ChowanySzukajacy = Iter_Random(Game[GAME_CH]);
        SetPlayerColor(ChowanySzukajacy, setAlpha(GetPlayerColor(ChowanySzukajacy), 0x00));
        SetPlayerTeam(ChowanySzukajacy, 0);
        giveWeapon(ChowanySzukajacy, WEAPON_MINIGUN, 9999);
        foreach(new i : Game[GAME_CH]) showGameDraw(i, form("~y~GRACZ ~r~%s ~y~ZOSTAJE SZUKAJACYM!", getNick(ChowanySzukajacy)));
    }

    foreach(new i : Game[GAME_CH]) {
        if(!IsPlayerInArea(i, unpackXYZR(chowanyDataArena[ChowanyArenaID][chowany_area])) && chowanyDataArena[ChowanyArenaID][chowany_interior] == 0) {
            new Float:health; GetPlayerHealth(i, health);
            SetPlayerHealth(i, health - 10.0);
            GameTextForPlayer(i, "~r~~h~WROC NA OBSZAR ZABAWY!", 1000, 3);
        }
        UpdateGameStatus(i, GAME_CH);
    }
    return 1;
}

stock StopChowany(reason_type)
{
    if(!GameStarted[GAME_CH]) return 1;
    GameStarted[GAME_CH] = false;
    GameCountDown[GAME_CH] = 0;

    new reason[50];
    switch(reason_type) {
        case GAME_STOP_TIMER: {
            reason = "Skoñczy³ siê czas";
            foreach(new i : Game[GAME_CH]) {
                if(i != ChowanySzukajacy) {
                    new money = math_random(10000, 30000);
                    giveMoney(i, money);
                    showGameDraw(i, form("~y~CHOWAJACY ~r~NIE ZNALAZL ~y~CIEBIE!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

                    achievement(i, ACH_CH);
                    CheckBet(i, BET_GAME, true);
                } 
                else showGameDraw(i, form("~y~NIE ~r~ZNALAZLES(AS) ~y~WSZYSTKICH CHOWAJACYCH SIE!~n~~y~CHOWAJACYCH POZOSTALO: ~r~%d~y~!!", Iter_Count(Game[GAME_CH]) - 1));
            }
        }

        case GAME_STOP_PLAYERS: {
            reason = "Szukaj¹cy znalaz³ chowaj¹cych";

            new playerid = ChowanySzukajacy, money = math_random(10000, 30000);
            giveMoney(playerid, money);
            showGameDraw(playerid, form("~y~ZABILES(AS) WSZYSTKICH ~r~CHOWAJACYCH~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

            achievement(playerid, ACH_CH);
            CheckBet(playerid, BET_GAME, true);
        }
        case GAME_STOP_NULL: reason = "Wyst¹pi³ b³¹d";
    }

    foreach(new i : Game[GAME_CH]) {
        pGame[i] = -1;
        ShowNameTagForAll(i);
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_CH]);

    if(ChowanyZone != -1) DestroyZone(ChowanyZone);
    ChowanySzukajacy = -1;
    ChowanyArenaID = -1;

    UpdateGameDraw();
    KillTimer(GameTimer[GAME_CH]);
    return msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona - powód: {c}%s{/c}.", gameName[GAME_CH], gameCMD[GAME_CH], reason), 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(Iter_Contains(Game[GAME_CH], killerid) && Iter_Contains(Game[GAME_CH], playerid) && pGame[playerid] == GAME_CH && pGame[playerid] == pGame[killerid] && GameStarted[GAME_CH] && killerid == ChowanySzukajacy) {
            msg(killerid, C_CHAT, "Zabi³eœ(aœ) chowaj¹cego siê gracza: {c}%s(%d){/c}.", getNick(playerid), playerid);
            msg(playerid, C_CHAT, "Szukaj¹cy {c}%s(%d){/c} znalaz³(a) Ciebie! Zostajesz zdyskwalifikowany.", getNick(ChowanySzukajacy), ChowanySzukajacy);
        }
    }
    
    new iterate_max;
    if(Iter_Contains(Game[GAME_CH], playerid) && pGame[playerid] == GAME_CH && GameStarted[GAME_CH]) {
        CheckBet(playerid, BET_GAME, false);

        ShowNameTagForAll(playerid);
        SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
        for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
        LoadPlayerPosition(playerid);

        pGame[playerid] = -1;
        Iter_SafeRemove(Game[GAME_CH], playerid, iterate_max);
        playerid = iterate_max;
    }
    return 1;
}
