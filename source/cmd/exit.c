alias:exit("return", "leave")
CMD:exit(playerid)
{
    if(pDuel[playerid] && pDuelEnemy[playerid] != -1) return CheckDuel(playerid, pDuelEnemy[playerid], playerid);
    if(pArena[playerid] != -1) return ExitPlayerArena(playerid, pArena[playerid]);
    if(pGlitchArena[playerid]) return ExitGlitchArena(playerid);
    if(pTruck[playerid]) return Truck_Stop(playerid);
    if(pGang[playerid] != -1 && Iter_Contains(Sparing, playerid)) {
        new gangid = pGang[playerid];
        if(pGangSpar[gangid]) LoadPlayerPosition(playerid);
        Iter_Remove(Sparing, playerid);
        return msg(playerid, C_CHAT, "Opuœci³eœ(aœ) {c}sparing{/c}.");
    }
    if(pGame[playerid] != -1 && Iter_Contains(Game[pGame[playerid]], playerid)) {
        new gameid = pGame[playerid];
        pGame[playerid] = -1;
        Iter_Remove(Game[gameid], playerid);

        if(GameStarted[gameid]) {
            switch(gameid) {
                case GAME_RC: {
                    if(IsValidVehicle(RaceVehicle[playerid])) DestroyVehicle(RaceVehicle[playerid]);
                    if(IsValidDynamicRaceCP(RaceCheckpoint[playerid])) DestroyDynamicRaceCP(RaceCheckpoint[playerid]);
                    if(IsValidDynamicMapIcon(RaceMapIcon[playerid])) DestroyDynamicMapIcon(RaceMapIcon[playerid]);

                    DisableRemoteVehicleCollisions(playerid, false);
                }

                case GAME_CH: {
                    if(ChowanySzukajacy == playerid) ChowanySzukajacy = -1;
                    ShowNameTagForAll(playerid);
                }

                case GAME_SM: { SetVehicleToRespawn(sumoVehicle[playerid]); SetVehicleVirtualWorld(sumoVehicle[playerid], 99); }
                case GAME_BTR: ShowNameTagForAll(playerid);
            }
            CheckBet(playerid, BET_GAME, false);

            SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
            for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
            LoadPlayerPosition(playerid);
        }

        UpdateGameDraw();
        return msg(playerid, C_CHAT, "Wypisa³eœ(aœ) siê z zabawy: {c}%s (%s){/c}.", gameName[gameid], gameCMD[gameid]);
    }
    if(pJob[playerid] != -1) return ExitPraca(playerid);
    if(Iter_Contains(Event, playerid)) {
        if(EventStarted) LoadPlayerPosition(playerid);
        Iter_Remove(Event, playerid);
        return msg(playerid, C_CHAT, "Wypisa³eœ(aœ) siê z {c}eventu{/c}.");
    }
    else return sendError(playerid, ""RED"W niczym nie uczestniczysz.");
}