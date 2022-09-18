alias:v("p", "pojazd", "veh", "vehicle", "pojazdy", "cars", "vehicles")
CMD:v(playerid, params[])
{
    new modelid[32];
    if(isnull(params)) return showDialog(playerid, DIALOG_POJAZDY, DIALOG_STYLE_LIST, ""BLUE"Kategoria pojazdów", ""WHITE"Samoloty\n"WHITE"Helikoptery\n"WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowriders\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"£odzie\n"WHITE"Unikalne", "Wybierz", "Zamknij");
    if(sscanf(params, "s[32]", modelid)) return syntaxError(playerid, ""RED"/v [nazwa]");
    if(!GetVehicleNameByID(modelid)) return sendError(playerid, ""RED"Podany model pojazdu jest nieprawid³owy.");
    if(DeniedVehicle(GetVehicleNameByID(modelid)) && playerData[playerid][player_admin] < MODERATOR) return sendError(playerid, ""RED"Ten pojazd jest zabroniony.");

    blockCMD(playerid, "block_v", 10);

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);

    if(IsValidVehicle(playerData[playerid][player_createdVehicle])) DestroyVehicle(playerData[playerid][player_createdVehicle]);
    playerData[playerid][player_createdVehicle] = CreateVehicle(GetVehicleNameByID(modelid), x, y, z, a, -1, -1, 60);
    msg(playerid, C_YELLOW, "Stworzy³eœ(aœ) {c}%s{/c}.", VehicleNames[GetVehicleNameByID(modelid) - 400]);

    LinkVehicleToInterior(playerData[playerid][player_createdVehicle], GetPlayerInterior(playerid));
    SetVehicleVirtualWorld(playerData[playerid][player_createdVehicle], GetPlayerVirtualWorld(playerid));
    SetVehicleToRespawn(playerData[playerid][player_createdVehicle]);
    PutPlayerInVehicle(playerid, playerData[playerid][player_createdVehicle], 0);
    return printf(" >>> [VEH-CREATOR]: Stworzono %s [owner: %s(%d)] - x: %.2f, y: %.2f, z: %.2f, a: %.2f", VehicleNames[GetVehicleNameByID(modelid) - 400], getNick(playerid), playerid, x, y, z, a);
}
CMD:nrg(playerid, params[]) return callcmd::v(playerid, "nrg");

alias:napraw("fix", "repair")
CMD:napraw(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym pojeŸdzie.");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return sendError(playerid, ""RED"Nie jesteœ kierowc¹ tego pojazdu.");
    if(getMoney(playerid) < 500) return valueError(playerid, 500);
    if(!EventSettings[EVENT_SETTING_REPAIR] && EventStarted && Iter_Contains(Event, playerid)) return 1;

    blockCMD(playerid, "block_napraw", 10);
    giveMoney(playerid, -500);
    
    RepairVehicle(GetPlayerVehicleID(playerid));
    GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~b~~h~~h~POJAZD NAPRAWIONY!", 4500, 3);
    return playSound(playerid, 1133);
}

CMD:flip(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym pojeŸdzie.");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return sendError(playerid, ""RED"Nie jesteœ kierowc¹ tego pojazdu.");
    if(!EventSettings[EVENT_SETTING_FLIP] && EventStarted && Iter_Contains(Event, playerid)) return 1;

    blockCMD(playerid, "block_flip", 5);

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, true, x, y, z, a);
    setPosition(playerid, true, x, y, z, a);
    return 1;
}

alias:tune("tuning")
CMD:tune(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym pojeŸdzie.");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return sendError(playerid, ""RED"Nie jesteœ kierowc¹ tego pojazdu.");

    blockCMD(playerid, "block_tune", 10);
    TuneVehicle(GetPlayerVehicleID(playerid));
    return playSound(playerid, 1133);
}

alias:lakier("vc")
CMD:lakier(playerid, params[])
{
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym pojeŸdzie.");
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return sendError(playerid, ""RED"Nie jesteœ kierowc¹ tego pojazdu.");

    new c1, c2;
    if(sscanf(params, "dd", c1, c2)) {
        s[0] = EOS;
        for(new i = 0; i < sizeof(VehicleColoursTableRGBA); i++) AddDialogListitem(playerid, "{%06x}#%d\n", VehicleColoursTableRGBA[i] >>> 8, i + 1);
        return showDialog(playerid, DIALOG_LAKIER_COLOR_A, DIALOG_STYLE_LIST, ""YELLOW"Wybierz pierwszy kolor", #, "Dalej", "Zamknij", 10);
    }
    if(c1 < 0 || c2 < 0 || c1 > 255 || c2 > 255) return sendError(playerid, ""RED"Nieprawid³owe ID kolorów! "WHITE"(0-255)");
    
    new privcarid = pVehID[playerid];
    if(privcarid != -1) {
        pVehColor[privcarid][0] = c1;
        pVehColor[privcarid][1] = c2;
        ChangeVehicleColor(pVeh[privcarid], c1, c2);
    }
    else ChangeVehicleColor(GetPlayerVehicleID(playerid), c1, c2);
    return playSound(playerid, 1133), 1;
}