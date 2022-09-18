#include YSI_Coding\y_hooks

new const Float:zonesNoDM[][] = {
    {2607.5, 1958.5, 2685.5, 2011.5},   // POŒREDNIAK
    {2338.5, 1070.5, 2434.5, 1199.5}    // BANK
};
new noDMZone[sizeof zonesNoDM], noDMRectangle[sizeof zonesNoDM];
new bool:pNoDM[MAX_PLAYERS];

hook function ResetPlayerData(playerid) {
    pNoDM[playerid] = false;
    return continue(playerid);
}

stock Init_NoDM()
{
    for(new i = 0; i < sizeof(zonesNoDM); i++) {
        noDMZone[i] = CreateZone(unpackXYZR(zonesNoDM[i]));
        CreateZoneBorders(noDMZone[i]);

        noDMRectangle[i] = CreateDynamicRectangle(unpackXYZR(zonesNoDM[i]), 0, 0);
        Streamer_SetIntData(STREAMER_TYPE_AREA, noDMRectangle[i], E_STREAMER_EXTRA_ID, i + NODM_MAGIC);
    }
    return printf(" >>> [NO-DM]: Wczytanych stref NoDM: %d", sizeof(zonesNoDM)), 1;
}
