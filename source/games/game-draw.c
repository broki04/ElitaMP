stock UpdateGameStatus(playerid, gameid)
{
    if(gameid == -1) return 1;

    playerTextDrawSetString(playerid, tdInfo[1], "~p~%s", gameName[gameid]);
    playerTextDrawSetString(playerid, tdInfo[2], "~w~~h~%d", GameCountDown[gameid]);

    switch(gameid) {
        case GAME_BTR:  playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Zabojstwa: ~y~~h~%d ~b~~h~~h~(osob: %d)~n~~w~~h~Obrazenia od strefy: ~r~~h~%.1f HP", RoyaleKills[playerid], Iter_Count(Game[GAME_BTR]) - 1, RoyaleZoneDamage);
        case GAME_DA:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Zabitych osob: ~y~~h~%d zabojstw~n~~w~~h~Zadanych obrazen: ~r~~h~%.1f damage", deathArenaKills[playerid][da_kills], deathArenaKills[playerid][da_damage]);
        case GAME_CH:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Osoba szukajaca: ~y~~h~%s(%d)~n~~w~~h~Ilosc chowajacych: ~p~~h~%d %s", getNick(ChowanySzukajacy), ChowanySzukajacy, Iter_Count(Game[GAME_CH]) - 1, ConvertTimeFormat(Iter_Count(Game[GAME_CH]) - 1, "osob", "osoba", "osoby", "osob"));
        case GAME_RC:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Punkty kontrolne: ~r~~h~~h~%d/%d~n~~w~~h~Pozycja w wyscigu: ~g~~h~#%d", RaceStatusData[playerid][race_status], RaceCountCPs, RacePlacement[playerid] + 1);
        case GAME_SK:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Poczatkowa liczba osob: ~p~~h~%d~n~~w~~h~Aktualna liczba osob: ~y~~h~%d", SiankoCount, Iter_Count(Game[GAME_SK]));
        case GAME_SM:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Poczatkowa liczba osob: ~p~~h~%d~n~~w~~h~Aktualna liczba osob: ~y~~h~%d", sumoZapisanyCount - 1, Iter_Count(Game[GAME_SM]) - 1);
        case GAME_WG:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Druzyna: ~y~~h~%s~n~~w~~h~Wynik: ~r~~h~%d~w~~h~:~b~~h~~h~%d", GetWarTeam(playerData[playerid][player_warTeam]), WarResult[0], WarResult[1]);
        case GAME_ZP:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Odgadnietych pojazdow: ~y~%d/%d~n~~w~~h~Pojazd pojawi sie za: ~r~~h~%d ~w~~h~sek!", StatusZP[playerid], MaxVehicleZP, GameCountDown[GAME_ZP]);

        case GAME_GG: {
            playerTextDrawSetString(playerid, tdInfo[2], " ");
            playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Poziom broni: ~y~~h~%d/%d~n~~w~~h~Aktualnie wygrywa: ~r~~h~%s", GunGameLevel[playerid] + 1, sizeof(GunGameAllowed), (GG_GetWinner() == -1) ? ("---") : (form("%s(%d)", getNick(GG_GetWinner()), GG_GetWinner())));
        }

        case GAME_WP: {
            playerTextDrawSetString(playerid, tdInfo[2], "%s%d", (SwitchZP == ZP_VEHICLE_CREATED) ? ("~b~~h~~h~") : ("~y~~h~"), GameCountDown[GAME_ZP]);
			playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Poczatkowa liczba osob: ~r~~h~%d~n~~w~~h~Obecna liczba osob: ~y~~h~%d", wpCountPeople, Iter_Count(Game[GAME_WP]));
        }
    }
    return 1;
}