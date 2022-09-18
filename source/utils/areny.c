#include YSI_Coding\y_hooks

#define ARENA_ONEDE     0
#define ARENA_POMPA     1
#define ARENA_MINI      2
#define ARENA_SNIPER    3

#define MAX_ARENY       (ARENA_SNIPER + 1)

new
    pArena[MAX_PLAYERS] = -1,
    pArenaKills[MAX_PLAYERS][MAX_ARENY],
    pArenaDeaths[MAX_PLAYERS][MAX_ARENY],
    Iterator:Areny[MAX_ARENY]<MAX_PLAYERS>
;

stock Init_Areny() {
    for(new i = 0; i < MAX_ARENY; i++) Iter_Init(Areny[i]);
    UpdateAreny();
}
stock UpdateAreny() return TextDrawSetString(tdAreny, form("~w~~h~/onede (~r~~h~%d~w~~h~) - ~w~~h~/pompa (~b~~h~~h~%d~w~~h~) - ~w~~h~/mini (~p~~h~%d~w~~h~) - ~w~~h~/sniper (~y~~h~%d~w~~h~) - ~w~~h~/tgl", Iter_Count(Areny[ARENA_ONEDE]), Iter_Count(Areny[ARENA_POMPA]), Iter_Count(Areny[ARENA_MINI]), Iter_Count(Areny[ARENA_SNIPER])));

stock GetArenaName(arenaid) {
    new name[25];
    switch(arenaid) {
        case ARENA_ONEDE: name = "/onede";
        case ARENA_POMPA: name = "/pompa";
        case ARENA_SNIPER: name = "/sniper";
        case ARENA_MINI: name = "/mini";
    }
    return name;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(pArena[playerid] != -1 && pArena[killerid] != -1 && pArena[playerid] == pArena[killerid]) {
            pArenaKills[killerid][pArena[killerid]] ++;
            pArenaDeaths[playerid][pArena[playerid]] ++;

            UpdateArenaDraw(playerid, pArena[playerid]);
            UpdateArenaDraw(killerid, pArena[killerid]);

            CheckQuest(killerid, DAILY_ARENA);
        }
    }
    return 1;
}

stock UpdateArenaDraw(playerid, arenaid)
{
    new Float:ratio = getRatio(pArenaKills[playerid][arenaid], pArenaDeaths[playerid][arenaid]);
    playerTextDrawSetString(playerid, tdInfo[1], "~b~~h~%s", GetArenaName(arenaid));
    playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Twoje statystyki: ~r~~h~%s~w~~h~/~b~~h~~h~%s ~y~~h~(%.1f)~n~~w~~h~Liczba osob: ~p~~h~%d %s", strdot(pArenaKills[playerid][arenaid]), strdot(pArenaDeaths[playerid][arenaid]), ratio, Iter_Count(Areny[arenaid]), ConvertTimeFormat(Iter_Count(Areny[arenaid]), "osob", "osoba", "osoby", "osob"));
}

stock getArenaKills(playerid)
{
    new astr[128];
    for(new i = 0; i < MAX_ARENY; i++) format(astr, sizeof(astr), "%s%s%i", astr, i == 0 ? ("") : (","), pArenaKills[playerid][i]);
    return astr;
}

stock getArenaDeaths(playerid)
{
    new astr[128];
    for(new i = 0; i < MAX_ARENY; i++) format(astr, sizeof(astr), "%s%s%i", astr, i == 0 ? ("") : (","), pArenaDeaths[playerid][i]);
    return astr;
}

stock JoinPlayerArena(playerid, arenaid, bool:cmd = false)
{
    if(cmd) {
        if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");
        SavePlayerPosition(playerid);
        ResetPlayerWeapons(playerid);

        msgAll(C_ORANGE2, "Gracz {c}%s(%d){/c} do³¹czy³(a) do areny: {c}%s{/c}. Do³¹cz i Ty!", getNick(playerid), playerid, GetArenaName(arenaid));
        for(new i = 0; i < 3; i++) PlayerTextDrawShow(playerid, tdInfo[i]);
    }

    pArena[playerid] = arenaid;
    Iter_Add(Areny[arenaid], playerid);
    UpdateArenaDraw(playerid, arenaid);

    if(cmd) {
        switch(arenaid) {
            case ARENA_ONEDE: checkRecord(RECORD_ONEDE, Iter_Count(Areny[ARENA_ONEDE]));
            case ARENA_POMPA: checkRecord(RECORD_POMPA, Iter_Count(Areny[ARENA_POMPA]));
            case ARENA_MINI: checkRecord(RECORD_MINI, Iter_Count(Areny[ARENA_MINI]));
            case ARENA_SNIPER: checkRecord(RECORD_SNIPER, Iter_Count(Areny[ARENA_SNIPER]));
        }
    }

    new worldid = VW_ARENA + arenaid + 1000;
    switch(arenaid) {
        case ARENA_ONEDE: {
            static const Float:onedePos[][] = {
                {299.7782, 173.2495, 1007.1719},
                {221.2334, 150.3305, 1003.0234},
                {210.7774, 186.4514, 1003.0313},
                {249.1874, 160.9782, 1003.0234},
                {205.3420, 157.3225, 1003.0234},
                {228.8251, 180.7334, 1003.0313}
            };
            new i = random(sizeof(onedePos));

            SetPlayerHealth(playerid, 15.0);
            SetPlayerArmour(playerid, 0.0);
            giveWeapon(playerid, WEAPON_DEAGLE, 1000);
            Teleport(playerid, false, onedePos[i][0], onedePos[i][1], onedePos[i][2], 0.0, 3, worldid, false);
        }

        case ARENA_POMPA: {
            static const Float:pompaPos[][] = {
                {963.9974, 2162.9009, 1011.0303},
                {948.2177, 2176.5933, 1011.0234},
                {933.5432, 2144.0266, 1011.0234},
                {960.3269, 2097.0989, 1011.0225},
                {963.3406, 2143.5618, 1011.0217},
                {948.2094, 2129.5647, 1011.0234},
                {935.0652, 2106.8689, 1011.0234},
                {949.6942, 2143.6091, 1011.0208},
                {964.2220, 2115.7434, 1011.0303}
            };
            new i = random(sizeof(pompaPos));

            SetPlayerHealth(playerid, 35.0);
            SetPlayerArmour(playerid, 0.0);
            giveWeapon(playerid, WEAPON_SHOTGUN, 1000);
            Teleport(playerid, false, pompaPos[i][0], pompaPos[i][1], pompaPos[i][2], 0.0, 1, worldid, false);
        }

        case ARENA_MINI: {
            static const Float:miniPos[][] = {
                {155.7019, 1873.0198, 17.9364},
                {232.1738, 1837.5021, 23.2422},
                {228.4798, 1934.8726, 17.6406},
                {211.1540, 1810.7789, 21.8672},
                {220.3861, 1861.8052, 13.1470},
                {189.6951, 1931.7057, 17.6406},
                {187.9848, 1867.2131, 18.5775}
            };
            new i = random(sizeof(miniPos));

            SetPlayerHealth(playerid, 100.0);
            SetPlayerArmour(playerid, 100.0);
            giveWeapon(playerid, WEAPON_MINIGUN, 1000);
            Teleport(playerid, false, miniPos[i][0], miniPos[i][1], miniPos[i][2], 0.0, 0, worldid, false);
        }

        case ARENA_SNIPER: {
            static const Float:sniperPos[][] = {
                {-976.62, 1061.26, 1345.67},
                {-994.17, 1041.68, 1342.46},
                {-1026.62, 1048.61, 1342.27},
                {-1069.61, 1045.42, 1343.93},
                {-1092.13, 1046.95, 1343.45},
                {-1113.64, 1089.77, 1341.84}
            };
            new i = random(sizeof(sniperPos));

            SetPlayerHealth(playerid, 35.0);
            SetPlayerArmour(playerid, 0.0);
            giveWeapon(playerid, WEAPON_SNIPER, 1000);
            Teleport(playerid, false, sniperPos[i][0], sniperPos[i][1], sniperPos[i][2], 0.0, 10, worldid, false);
        }
    }

    UpdateAreny();
    return printf(" >>> [ARENY]: Gracz %s(%d) do³¹czy³(a) do areny: %s (K: %d, D: %d)", getNick(playerid), playerid, GetArenaName(arenaid), pArenaKills[playerid][arenaid], pArenaDeaths[playerid][arenaid]), 1;
}

stock ExitPlayerArena(playerid, arenaid)
{
    if(pArena[playerid] == -1) return 1;

    pArena[playerid] = -1;
    Iter_Remove(Areny[arenaid], playerid);

    foreach(new i : Areny[arenaid]) UpdateArenaDraw(i, arenaid);
    for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
    LoadPlayerPosition(playerid);

    UpdateAreny();
    msg(playerid, C_CHAT, "Opuœci³eœ(aœ) arenê: {c}%s{/c}.", GetArenaName(arenaid));
    return printf(" >>> [ARENY]: Gracz %s(%d) opuœci³(a) arenê: %s (K: %d, D: %d)", getNick(playerid), playerid, GetArenaName(arenaid), pArenaKills[playerid][arenaid], pArenaDeaths[playerid][arenaid]), 1;
}

CMD:onede(playerid) return JoinPlayerArena(playerid, ARENA_ONEDE, true);
CMD:pompa(playerid) return JoinPlayerArena(playerid, ARENA_POMPA, true);
CMD:sniper(playerid) return JoinPlayerArena(playerid, ARENA_SNIPER, true);
CMD:mini(playerid) return JoinPlayerArena(playerid, ARENA_MINI, true);

alias:pompa("pump")
alias:sniper("snajper", "rifle")
