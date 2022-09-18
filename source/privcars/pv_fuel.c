#include YSI_Coding\y_hooks

new 
    Float:FuelStations[][] = {
        {1938.1498, -1773.1987, 12.5287},
        {2115.5195, 920.2801, 9.9661},
        {-1674.4263, 414.3090, 6.3256},
        {-1471.8395, 1863.9709, 31.7783}
    },
    FuelCostMoney[3],

    PlayerBar:FuelBar[MAX_PLAYERS],
    bool:PlayerFueling[MAX_PLAYERS],
    Float:PlayerFuelingAmount[MAX_PLAYERS],
    PlayerFuelingType[MAX_PLAYERS],
    PlayerFuelingCost[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
    PlayerFueling[playerid] = false;
    PlayerFuelingAmount[playerid] = 0.0;
    PlayerFuelingType[playerid] = -1;
    PlayerFuelingCost[playerid] = 0;
    return continue(playerid);
}

#define FUEL_BENZYNA        0
#define FUEL_DIESEL         1
#define FUEL_GAZ            2

stock Init_Stations()
{
    for(new i = 0; i < sizeof(FuelStations); i++) {
        CreateDynamic3DTextLabel(""ORANGE"Stacja benzynowa\n"WHITE"Kliknij "LY"LCTRL"WHITE", aby rozpocz¹æ tankowanie!", -1, unpackXYZ(FuelStations[i]) + 0.25, 30.0, .worldid=0, .interiorid=0);
        CreateDynamicPickup(1650, 1, unpackXYZ(FuelStations[i]) + 0.75, .worldid=0, .interiorid=0);
    }

    FuelCostMoney[FUEL_BENZYNA] = math_random(100, 150);
    FuelCostMoney[FUEL_DIESEL] = math_random(100, 130);
    FuelCostMoney[FUEL_GAZ] = math_random(50, 80);
    return printf(" >>> [FUEL]: Ceny paliw (B: %d$, D: %d$, G: %d$)", unpackXYZ(FuelCostMoney)), 1;
}

stock GetNearestStation(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(FuelStations); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, unpackXYZ(FuelStations[i]))) {
            id = i;
            break;
        }
    }
    return id;
}

stock GetFuelingColor(playerid)
{
    new color[10];
    switch(PlayerFuelingType[playerid]) {
        case FUEL_BENZYNA: color = "~g~~h~";
        case FUEL_DIESEL: color = "~r~";
        case FUEL_GAZ: color = "~b~~h~~h~";
    }
    return color;
}

stock GetFuelingName(playerid)
{
    new color[10];
    switch(PlayerFuelingType[playerid]) {
        case FUEL_BENZYNA: color = "Benzyna";
        case FUEL_DIESEL: color = "Diesel";
        case FUEL_GAZ: color = "Gaz";
    }
    return color;
}

stock StopFueling(playerid)
{
    PlayerFueling[playerid] = false;
    PlayerFuelingAmount[playerid] = 0.0;
    PlayerFuelingType[playerid] = -1;
    PlayerFuelingCost[playerid] = 0;

    for(new i = 0; i < 5; i++) PlayerTextDrawHide(playerid, tdFuel[i]);
    HidePlayerProgressBar(playerid, FuelBar[playerid]);
    return TogglePlayerControllable(playerid, true), 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new fuelid = GetNearestStation(playerid);
    if(fuelid != -1 && PRESSED(KEY_ACTION) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && !PlayerFueling[playerid]) {
        new privcarid = pVehID[playerid];
        if(privcarid == -1 || !IsPlayerInVehicle(playerid, pVeh[privcarid])) return dialogBox(playerid, C_RED, "Musisz byæ w prywatnym pojeŸdzie!");
        //if(pVehFuel[privcarid][(pVehGasEnabled[privcarid]) ? (GAS_AMOUNT) : (FUEL_AMOUNT)] >= (pVehFuelAmount[privcarid] - 15.0)) return dialogBox(playerid, C_RED, "Twój {c}prywatny pojazd{/c} nie wymaga obecnie tankowania!");
    
        if(pVehEngine[privcarid] == GASOLINE_ENGINE) {
            if(pVehGas[privcarid] && pVehGasEnabled[privcarid]) PlayerFuelingType[playerid] = FUEL_GAZ;
            else PlayerFuelingType[playerid] = FUEL_BENZYNA;
        }
        else PlayerFuelingType[playerid] = FUEL_DIESEL;

        PlayerFueling[playerid] = true;
        PlayerFuelingCost[playerid] = 0;
        PlayerFuelingAmount[playerid] = 0.0;

        playerTextDrawSetString(playerid, tdFuel[1], "TANKOWANIE POJAZDU");
        playerTextDrawSetString(playerid, tdFuel[2], "~w~~h~Typ paliwa: %s%s ~y~~h~(%d$)~n~~w~~h~Zmiana typu paliwa: %sH", GetFuelingColor(playerid), GetFuelingName(playerid), FuelCostMoney[PlayerFuelingType[playerid]], GetFuelingColor(privcarid));
        playerTextDrawSetString(playerid, tdFuel[3], "~w~~h~Uzyj ~r~~h~Y~w~~h~, aby zatankowac litr paliwa~n~~w~~h~Uzyj ~r~~h~N~w~~h~, aby zatankowac do pelna");
        playerTextDrawSetString(playerid, tdFuel[4], "~w~~h~%.0f~y~/~w~~h~%.0f", PlayerFuelingAmount[playerid], pVehFuelAmount[privcarid]);

        SetPlayerProgressBarValue(playerid, FuelBar[playerid], 0.0);
        SetPlayerProgressBarMaxValue(playerid, FuelBar[playerid], pVehFuelAmount[privcarid]);
        SetPlayerProgressBarColour(playerid, FuelBar[playerid], ((pVehGas[privcarid] && pVehGasEnabled[privcarid]) ? (0x34ABEBFF) : (GetEngineColor(pVehEngine[privcarid]))));

        msg(playerid, -1, "%d", floatround(pVehFuelAmount[privcarid] - pVehFuel[privcarid][(PlayerFuelingType[playerid] == FUEL_GAZ) ? (GAS_AMOUNT) : (FUEL_AMOUNT)]));

        for(new i = 0; i < 5; i++) PlayerTextDrawShow(playerid, tdFuel[i]);
        ShowPlayerProgressBar(playerid, FuelBar[playerid]);
        TogglePlayerControllable(playerid, false);
    }

    if(fuelid != -1 && PlayerFueling[playerid] && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && pVehID[playerid] != -1 && IsPlayerInVehicle(playerid, pVeh[pVehID[playerid]])) {
        new privcarid = pVehID[playerid];
        if(PRESSED(KEY_CTRL_BACK) && pVehGas[privcarid] && pVehEngine[privcarid] == GASOLINE_ENGINE) PlayerFuelingType[playerid] = ((PlayerFuelingType[playerid] == FUEL_BENZYNA) ? (FUEL_GAZ) : (FUEL_BENZYNA));

        if(PRESSED(KEY_YES)) {
            new type = PlayerFuelingType[playerid];
            if(getMoney(playerid) < FuelCostMoney[type]) {
                msg(playerid, C_DRED, "Potrzebujesz {c}%d${/c}, by zatankowaæ dodatkowy litr! Tankowanie przerwane.", FuelCostMoney[type]);
                return StopFueling(playerid), 1;
            } 

            PlayerFuelingCost[playerid] += FuelCostMoney[type];
            giveMoney(playerid, -FuelCostMoney[type]);

            PlayerFuelingAmount[playerid] += 1.0;
            if(PlayerFuelingAmount[playerid] >= floatround(pVehFuelAmount[privcarid] - pVehFuel[privcarid][(type == FUEL_GAZ) ? (GAS_AMOUNT) : (FUEL_AMOUNT)])) {
                msg(playerid, C_BLUE2, "Tankowanie zakoñczone! Zap³aci³eœ(aœ) ³¹cznie {c}%s${/c} za {c}%.2f L{/c} paliwa!", strdot(PlayerFuelingCost[playerid]), PlayerFuelingAmount[playerid]);
                return StopFueling(playerid), 1;
            }

            pVehFuel[privcarid][(type == FUEL_GAZ) ? (GAS_AMOUNT) : (FUEL_AMOUNT)] += 1.0;
            SetPlayerProgressBarValue(playerid, FuelBar[playerid], PlayerFuelingAmount[playerid]);
            SetPlayerProgressBarMaxValue(playerid, FuelBar[playerid], pVehFuelAmount[privcarid]);
        }

        if(PRESSED(KEY_NO)) {
            new type = PlayerFuelingType[playerid], cost = 0;
            switch(PlayerFuelingType[playerid]) {
                case FUEL_BENZYNA, FUEL_DIESEL: cost = floatround((pVehFuelAmount[privcarid] - pVehFuel[privcarid][0]) * FuelCostMoney[type]);
                case FUEL_GAZ: cost = floatround((pVehFuelAmount[privcarid] - pVehFuel[privcarid][1]) * FuelCostMoney[type]);
            }

            if(getMoney(playerid) < cost) {
                msg(playerid, C_DRED, "Potrzebujesz {c}%s${/c}, by zatankowaæ do pe³na! Tankowanie przerwane.", strdot(cost));
                return StopFueling(playerid), 1;
            }

            switch(PlayerFuelingType[playerid]) {
                case FUEL_BENZYNA, FUEL_DIESEL: pVehFuel[privcarid][FUEL_AMOUNT] = pVehFuelAmount[privcarid];
                case FUEL_GAZ: pVehFuel[privcarid][GAS_AMOUNT] = pVehFuelAmount[privcarid];
            }

            giveMoney(playerid, -cost);
            msg(playerid, C_BLUE2, "Tankowanie zakoñczone! Zap³aci³eœ(aœ) ³¹cznie {c}%s${/c} za {c}pe³ne{/c} tankowanie!", strdot(PlayerFuelingCost[playerid]), PlayerFuelingAmount[playerid]);
            return StopFueling(playerid), 1;
        }

        playerTextDrawSetString(playerid, tdFuel[2], "~w~~h~Typ paliwa: %s%s ~y~~h~(%d$)~n~~w~~h~Zmiana typu paliwa: %sH", GetFuelingColor(playerid), GetFuelingName(playerid), FuelCostMoney[PlayerFuelingType[playerid]], GetFuelingColor(playerid));
        playerTextDrawSetString(playerid, tdFuel[3], "~w~~h~Uzyj ~r~~h~Y~w~~h~, aby zatankowac litr paliwa~n~~w~~h~Uzyj ~r~~h~N~w~~h~, aby zatankowac do pelna");
        playerTextDrawSetString(playerid, tdFuel[4], "~w~~h~%.0f~p~/~w~~h~%.0f L", PlayerFuelingAmount[playerid], pVehFuelAmount[privcarid]);
    }
    return 1;
}

hook OnPlayerUpdate(playerid) {
    if(PlayerFueling[playerid] && IsPlayerInAnyVehicle(playerid)) RepairVehicle(GetPlayerVehicleID(playerid));
    return 1;
}