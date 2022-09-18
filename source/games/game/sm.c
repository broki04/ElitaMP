new 
    sumoCar[3][40],
    sumoZapisanyCount,
    sumoVehicle[MAX_PLAYERS]
;

stock StartSM()
{
    if(!GameStarted[GAME_SM]) return 1;
    GameStarted[GAME_SM] = true;
    GameCountDown[GAME_SM] = 200 + 5;
    sumoZapisanyCount = Iter_Count(Game[GAME_SM]);

    new count;
    foreach(new i : Game[GAME_SM]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 0.0);

        UpdateGameStatus(i, GAME_SM);
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

        FreezePlayer(i, 5);
        setOdliczanie(i, 5);

        switch(sumoArenaID) {
            case 0: Teleport(i, false, 201.5300, 392.2506, 607.3614 + 5.0, 0.0, 0, VW_SM, false); 
            case 1: Teleport(i, false, 1560.7894, -3084.5784, 78.3953 + 5.0, 0.0, 0, VW_SM, false);  
            case 2: Teleport(i, false, -189.3335, -612.0821, 63.7825 + 5.0, 0.0, 0, VW_SM, false);  
        }

        sumoVehicle[i] = sumoCar[sumoArenaID][count];
        SetVehicleHealth(sumoCar[sumoArenaID][count], 99999.0);
        SetVehicleVirtualWorld(sumoCar[sumoArenaID][count], VW_SM);
        PutPlayerInVehicle(i, sumoCar[sumoArenaID][count], 0);

        count ++;
    }

    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(Arena #%d, %d %s)", gameName[GAME_SM], gameCMD[GAME_SM], sumoArenaID + 1, Iter_Count(Game[GAME_SM]), ConvertTimeFormat(Iter_Count(Game[GAME_SM]), "osób", "osoba", "osoby", "osób"));
    return UpdateGameDraw(), 1;
}

function PutPlayerSumo(playerid, arenaid, vehicleid)
{
    sumoVehicle[playerid] = sumoCar[arenaid][vehicleid];
    SetVehicleHealth(sumoCar[arenaid][vehicleid], 99999.0);
    PutPlayerInVehicle(playerid, sumoCar[arenaid][vehicleid], 0);
    return TogglePlayerControllable(playerid, true), 1;
}

stock UpdateSM()
{
    if(!GameStarted[GAME_SM]) return 1;
    if(Iter_Count(Game[GAME_SM]) < 1) return StopSM(SUMO_TIME_OUT);
    if(Iter_Count(Game[GAME_SM]) == 1) return StopSM(SUMO_ONE_PLAYER);

    new iterate_max;
    foreach(new i : Game[GAME_SM]) {
        new Float:pos[3]; GetPlayerPos(i, unpackXYZ(pos));
        if(pos[2] <= 10.0 || GetPlayerState(i) != PLAYER_STATE_DRIVER) {
            SetVehicleToRespawn(sumoVehicle[i]);
            SetVehicleVirtualWorld(sumoVehicle[i], VW_SM);

            SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
            for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
            LoadPlayerPosition(i);

            CheckBet(i, BET_GAME, false);

            pGame[i] = -1;
            Iter_SafeRemove(Game[GAME_SM], i, iterate_max);
            i = iterate_max;
        }
        UpdateGameStatus(i, GAME_SM);
    }
    return 1;
}

stock StopSM(reason)
{
    if(!GameStarted[GAME_SM]) return 1;
    GameStarted[GAME_SM] = false;
    GameCountDown[GAME_SM] = 0;

    sumoArenaID = -1;
    sumoZapisanyCount = sumoVehicleCount = 0;

	for(new i = 0; i < 3; i++) { for(new x = 0; x < 40; x++) { SetVehicleToRespawn(sumoCar[i][x]); SetVehicleVirtualWorld(sumoCar[i][x], VW_SM); } }
    if(reason == SUMO_TIME_OUT) msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona! Liczba ¿ywych: {c}%d{/c}.", gameName[GAME_SM], gameCMD[GAME_SM], Iter_Count(Game[GAME_SM]));

    foreach(new i : Game[GAME_SM]) {
        switch(reason) {
            case SUMO_ONE_PLAYER: {
                new money = math_random(10000, 30000);
                giveMoney(i, money);
                showGameDraw(i, form("~y~ZWYCIEZYLES(AS) NA ~r~SUMO~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));

                achievement(i, ACH_SM);
                CheckBet(i, BET_GAME, true);

                msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona! Zwyciêzca: {c}%s(%d){/c}.", gameName[GAME_SM], gameCMD[GAME_SM], getNick(i), i);
            }

            case SUMO_TIME_OUT: {
                new money = math_random(10000, 25000);
                giveMoney(i, money);

                achievement(i, ACH_SM);
                CheckBet(i, BET_GAME, true);

                showGameDraw(i, form("~y~PRZETRWALES(AS) NA ~r~SUMO~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));
            }
        }

        pGame[i] = -1;
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_SM]);

    UpdateGameDraw();
    return KillTimer(GameTimer[GAME_SM]);
}