#include YSI_Coding\y_hooks

new 
    vehicleDataZP[] = {
        400,401,402,403,404,405,410,411,412,413,415,418,419,421,422,424,426,429,434,436,439,440,442,444,445,451,
        458,459,461,462,463,466,467,468,470,471,474,475,477,478,479,480,481,482,483,489,490,491,492,494,495,496,
        500,502,503,504,506,507,508,509,510,516,517,518,521,522,526,527,528,529,531,533,534,535,536,540,541,542,
        543,545,546,547,549,550,551,554,555,556,557,558,559,560,561,562,565,566,567,568,571,572,573,575,576,579,
        580,581,585,586,587,589,600,602,603
    },

    VehicleZP,
    VehicleModelZP,
    ZoneZP,
    SwitchZP,
    StatusZP[MAX_PLAYERS],
    MaxVehicleZP
;

#define ZP_VEHICLE_WAIT     0
#define ZP_VEHICLE_CREATED  1

stock StartZP()
{
    if(!GameStarted[GAME_ZP]) return 1;

    GameStarted[GAME_ZP] = true;
    GameCountDown[GAME_ZP] = 10;

    ZoneZP = CreateZone(-1999.5, 237.5, -1907.5, 314.5);
    CreateZoneBorders(ZoneZP);

    SwitchZP = ZP_VEHICLE_WAIT;
    MaxVehicleZP = math_random(10, 15);
    VehicleModelZP = -1;

    foreach(new i : Game[GAME_ZP]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 100.0);

        ShowZoneForPlayer(i, ZoneZP, setAlpha(C_BLUE, 160));
        ZoneFlashForPlayer(i, ZoneZP, setAlpha(C_LBLUE, 160));

        StatusZP[i] = 0;
        UpdateGameStatus(i, GAME_ZP);
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

        Teleport(i, false, -1946.5525, 274.6809, 41.0471, 161.5055, 0, VW_ZP);
        showGameDraw(i, "~y~POJAZD POJAWI SIE ZA ~r~10 ~y~SEKUND!~n~~y~PRZYGOTUJ SIE!!", 5);
    }

    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%d %s)", gameName[GAME_ZP], gameCMD[GAME_ZP], Iter_Count(Game[GAME_ZP]), ConvertTimeFormat(Iter_Count(Game[GAME_ZP]), "osób", "osoba", "osoby", "osób"));
    return 1;
}

stock CreateVehicleZP()
{
    if(!GameStarted[GAME_ZP]) return 1;
    if(IsValidVehicle(VehicleZP)) DestroyVehicle(VehicleZP);
    GameCountDown[GAME_ZP] = 30;

    VehicleModelZP = vehicleDataZP[random(sizeof(vehicleDataZP))];
    VehicleZP = CreateVehicle(VehicleModelZP, -1950.4094, 257.7974, 40.7757, 296.7649, -1, -1, -1);
    SetVehicleVirtualWorld(VehicleZP, VW_ZP);

    SwitchVehicleEngine(VehicleZP, false);
    SwitchVehicleDoors(VehicleZP, false);

    SwitchZP = ZP_VEHICLE_CREATED;
    return 1;
}

stock GetStatusZP(type)
{
    new id = -1, status = 0;
    foreach(new i : Game[GAME_ZP]) {
        if(StatusZP[i] > status) {
            id = i;
            status = StatusZP[i];
        }
    }
    return (type == 0) ? (id) : (status);
}

stock UpdateZP()
{
    if(!GameStarted[GAME_ZP]) return 1;
    if(Iter_Count(Game[GAME_ZP]) < 2) return StopZP(INVALID_PLAYER_ID);

    foreach(new i : Game[GAME_ZP]) {
        if(!IsPlayerInArea(i, -1999.5, 237.5, -1907.5, 314.5)) {
            new Float:health; GetPlayerHealth(i, health);
            SetPlayerHealth(i, health - 10.0);
            GameTextForPlayer(i, "~r~~h~WROC NA OBSZAR ZABAWY!", 1000, 3);
        }

        switch(SwitchZP) {
            case ZP_VEHICLE_CREATED: showGameDraw(i, form("~p~MASZ ~w~~h~%d ~p~SEK NA ODGADNIECIE POJAZDU!", GameCountDown[GAME_ZP]), 1);
            case ZP_VEHICLE_WAIT: showGameDraw(i, form("~p~POJAZD POJAWI SIE ZA ~w~~h~%d ~p~SEK!", GameCountDown[GAME_ZP]), 1);
        }

        UpdateGameStatus(i, GAME_ZP);
    }
    return 1;
}

stock StopZP(playerid)
{
    if(!GameStarted[GAME_ZP]) return 1;
    GameStarted[GAME_ZP] = false;
    GameCountDown[GAME_ZP] = 0;

    if(IsValidVehicle(VehicleZP)) DestroyVehicle(VehicleZP);
    VehicleModelZP = -1;
    DestroyZone(ZoneZP);
    SwitchZP = ZP_VEHICLE_WAIT;

    new winner_str[128];
    if(playerid != -1) format(winner_str, sizeof(winner_str), " Zwyciêzc¹ zosta³: {c}%s(%d){/c}.", getNick(playerid), playerid);

    foreach(new i : Game[GAME_ZP]) {
        pGame[i] = -1;
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_ZP]);

    UpdateGameDraw();
    KillTimer(GameTimer[GAME_ZP]);
    return msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona.%s", gameName[GAME_ZP], gameCMD[GAME_ZP], winner_str), 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(vehicleid == VehicleZP && pGame[playerid] == GAME_ZP && Iter_Contains(Game[GAME_ZP], playerid) && GameStarted[GAME_ZP]) return ClearAnimations(playerid), 0;
    return 1;
}