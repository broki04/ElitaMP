#define SK_HEIGHT   500.0

new 
    SiankoObject[81], 
    SiankoTimer,
    SiankoCount
;

stock StartSK()
{
    if(!GameStarted[GAME_SK]) return 1;
    GameStarted[GAME_SK] = true;
    GameCountDown[GAME_SK] = 200 + 5;

    SiankoCount = Iter_Count(Game[GAME_SK]);
    SiankoTimer = 0;

    new Float:x, Float:y, Float:z;
    x = 1882.0; y = -1395.0;

    for(new i = 0; i < 81; i++) {
        if(IsValidDynamicObject(SiankoObject[i])) DestroyDynamicObject(SiankoObject[i]);

        switch(i) {
            case 9, 18, 27, 36, 45, 54, 63, 72: {
                x = 1882.0;
                y += 4;
            }
        }

        SiankoObject[i] = CreateDynamicObject(3374, x, y, SK_HEIGHT, 0.0, 0.0, 0.0, VW_SK, 0, -1, 200.0);
        x += 4;
    }

    foreach(new i : Game[GAME_SK]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);
        SetHealth(i, 100.0, 0.0);

        UpdateGameStatus(i, GAME_SK);
        for(new j = 0; j < 3; j++) PlayerTextDrawShow(i, tdInfo[j]);

        GetDynamicObjectPos(SiankoObject[random(81)], x, y, z);
        Teleport(i, false, x, y, z + 1.0, 0.0, 0, VW_SK, true);
    }
    return 1;
}

stock UpdateSK()
{
    if(!GameStarted[GAME_SK]) return 1;
    if(Iter_Count(Game[GAME_SK]) == 1) return StopSK(SK_WINNER_FIRST);
    if(Iter_Count(Game[GAME_SK]) < 1) return StopSK(SK_CANCEL_GAME);

    SiankoTimer ++;
    if(SiankoTimer >= 15) {
        new shooterid = Iter_Random(Game[GAME_SK]);
        foreach(new i : Game[GAME_SK]) {
            ResetPlayerWeapons(i);
            showGameDraw(i, form("~r~~h~STRZELCEM ZOSTAJE: ~y~%s~r~~h~!", getNick(shooterid)), 3);
        }

        giveWeapon(shooterid, WEAPON_SILENCED, 1);
        SiankoTimer = 0;
    }

    new iterate_max;
    foreach(new i : Game[GAME_SK]) { 
        new Float:x, Float:y, Float:z; GetPlayerPos(i, x, y, z);
        if(z < 500.0) {
            SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
            for(new loopid = 0; loopid < 3; loopid++) PlayerTextDrawHide(i, tdInfo[loopid]);
            LoadPlayerPosition(i);
            
            CheckBet(i, BET_GAME, false);

            pGame[i] = -1;
            Iter_SafeRemove(Game[GAME_SK], i, iterate_max);
            i = iterate_max;
        }
        UpdateGameStatus(i, GAME_SK);
    }
    return 1;
}

stock StopSK(winner)
{
    if(!GameStarted[GAME_SK]) return 1;
    GameStarted[GAME_SK] = false;
    GameCountDown[GAME_SK] = 0;

    for(new i = 0; i < 81; i++) { if(IsValidDynamicObject(SiankoObject[i])) DestroyDynamicObject(SiankoObject[i]); }
    SiankoTimer = SiankoCount = 0;

    if(winner == SK_CANCEL_GAME) msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona! Liczba ¿ywych: {c}%d{/c}.", gameName[GAME_SK], gameCMD[GAME_SK], Iter_Count(Game[GAME_SK]));
    foreach(new i : Game[GAME_SK]) {
        switch(winner) {
            case SK_WINNER_FIRST: {
                new money = math_random(10000, 30000);
                giveMoney(i, money);
                showGameDraw(i, form("~y~ZWYCIEZYLES(AS) NA ~r~SIANKO~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));
                msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona! Zwyciêzca: {c}%s(%d){/c}.", gameName[GAME_SK], gameCMD[GAME_SK], getNick(i), i);

                achievement(i, ACH_SK);
                CheckBet(i, BET_GAME, true);
            }

            case SK_CANCEL_GAME: {
                new money = math_random(10000, 25000);
                giveMoney(i, money);
                showGameDraw(i, form("~y~PRZETRWALES(AS) NA ~r~SIANKO~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

                achievement(i, ACH_SK);
                CheckBet(i, BET_GAME, true);
            }
        }

        pGame[i] = -1;
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_SK]);

    UpdateGameDraw();
    return KillTimer(GameTimer[GAME_SK]);
}