stock ResetPlayerData(playerid)
{
    static const resetData[e_playerData];
    playerData[playerid] = resetData;

    ResetTeleportData(playerid);
    ResetGangData(playerid);
    ResetDuelData(playerid);
    ResetAccountData(playerid);
    ResetPrivCarData(playerid);
    ResetGameData(playerid);
    ResetRaceCreatorData(playerid);

    pArena[playerid] = pOwnRankColor[playerid] = playerData[playerid][player_lastPM] = playerData[playerid][player_gangbase] = playerData[playerid][player_business] = playerData[playerid][player_poolID] = playerData[playerid][player_sparingGangSpec] = -1;
    pDuelRank[playerid] = pDuelPoints[playerid] = pStalyInterior[playerid] = pCheatDetected[playerid] = 0;
    pOwnRankHave[playerid] = playerData[playerid][player_holding] = pCheatTemp[playerid] = pCheatWH[playerid] = false;
    pDrawVisible[playerid] = true;

    pWeapons[playerid] = {0, WEAPON_DEAGLE, WEAPON_SHOTGUN, WEAPON_MP5, 0, 0, 0};
    pArenaKills[playerid] = {0, 0, 0, 0};
    pArenaDeaths[playerid] = {0, 0, 0, 0};
    pStalyPos[playerid] = Float:{0.0, 0.0, 0.0, 0.0};

    strcat(pOwnRank[playerid], "@NULL");
    for(new i = 0; i < MAX_ARENY; i++) Iter_Remove(Areny[i], playerid);

    strcat(pSecretQuestion[playerid], "@NULL");
    strcat(pSecretAnswer[playerid], "@NULL");
    strcat(pSecretPinCode[playerid], "@NULL");
    strcat(playerData[playerid][player_lastmessage], "@NULL");

    Iter_Remove(Permission, playerid);
    Iter_Remove(Premium, playerid);
    return 1;
}