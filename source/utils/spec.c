new pSpectate[MAX_PLAYERS] = -1;
hook function ResetPlayerData(playerid) { 
    pSpectate[playerid] = -1; 
    return continue(playerid), 1;
}

stock SpecToggle(playerid, targetid, bool:toggle)
{
    if(toggle) {
        SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));
        SetPlayerInterior(playerid, GetPlayerInterior(targetid));
        TogglePlayerSpectating(playerid, true);

        if(IsPlayerInAnyVehicle(playerid)) PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetid));
        else PlayerSpectatePlayer(playerid, targetid);
    }
    else TogglePlayerSpectating(playerid, false);
    pSpectate[playerid] = (toggle) ? (targetid) : (-1);
    return 1;
}

CMD:spec(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/spec [id]");
    if(!IsPlayerConnected(id) || !playerData[id][player_logged]) return Polaczenie(playerid);
    if(!IsPlayerSpawned(id)) return sendError(playerid, ""RED"Ten gracz nie jest zespawnowany!");

    msg(playerid, C_CORAL, "Obserwujesz gracza: {c}%s(%d){/c}. Aby przestaæ, wpisz: {c}/specoff{/c}.", getNick(id), id);
    return SpecToggle(playerid, id, true), 1;
}
flags:spec(MODERATOR)

CMD:specoff(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(pSpectate[playerid] == -1) return sendError(playerid, ""RED"Aktualnie nikogo nie obserwujesz.");

    msg(playerid, C_CORAL, "Przesta³eœ(aœ) obserwowaæ gracza: {c}%s(%d){/c}.", getNick(pSpectate[playerid]), pSpectate[playerid]);
    return SpecToggle(playerid, pSpectate[playerid], false), 1;
}

function RefreshSpec(playerid)
{
    foreach(new i : Player) {
        if(pSpectate[i] == playerid) {
            SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
            SetPlayerInterior(i, GetPlayerInterior(playerid));
            TogglePlayerSpectating(i, true);

            if(IsPlayerInAnyVehicle(i)) PlayerSpectateVehicle(i, GetPlayerVehicleID(playerid));
            else PlayerSpectatePlayer(i, playerid);
        }
    }
    return 1;
}