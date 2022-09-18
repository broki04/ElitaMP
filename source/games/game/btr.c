#include YSI_Coding\y_hooks

#define MAX_ROYALE_WEAPONS      100

enum e_royaleDataAreny {
    royale_name[50],
    Float:royale_spawn[4],
    Float:royale_zone[4]
};

new 
    royaleDataAreny[][e_royaleDataAreny] = {
        {"Bayside",     {-2468.9893, 2432.2444, 15.4347, 187.1607},         {-2705.5, 2172.0, -2086.5, 2572.0}}
    },
    royaleWeaponData[] = {9, 16, 18, 22, 23, 24, 25, 26, 27, 29, 31, 30, 32, 34, 35, 38, 1337, 2137},

    RoyaleZone,
    Float:RoyaleZonePosition[4],
    RoyaleWeaponPickup[MAX_ROYALE_WEAPONS],
    RoyaleWeaponID[MAX_ROYALE_WEAPONS],
    RoyaleZoneSwitch,
    Float:RoyaleZoneDamage,
    RoyaleArenaID,
    RoyaleZoneCount,
    RoyaleKills[MAX_PLAYERS]
;

#define ROYALE_ZONE_STOP        0
#define ROYALE_ZONE_MOVE        1

stock StartBTR()
{
    if(!GameStarted[GAME_BTR]) return 1;
    GameStarted[GAME_BTR] = true;
    GameCountDown[GAME_BTR] = 40;

    RoyaleZoneDamage = 3.0;
    RoyaleZoneSwitch = ROYALE_ZONE_STOP;
    RoyaleZoneCount = 0;

    RoyaleArenaID = random(sizeof(royaleDataAreny));
    RoyaleZone = CreateZone(unpackXYZR(royaleDataAreny[RoyaleArenaID][royale_zone]));
    CreateZoneBorders(RoyaleZone);
    for(new i = 0; i < 4; i++) RoyaleZonePosition[i] = royaleDataAreny[RoyaleArenaID][royale_zone][i];

    new Float:x, Float:y, Float:z, Float:a;
    x = royaleDataAreny[RoyaleArenaID][royale_spawn][0];
    y = royaleDataAreny[RoyaleArenaID][royale_spawn][1];
    z = royaleDataAreny[RoyaleArenaID][royale_spawn][2];
    a = royaleDataAreny[RoyaleArenaID][royale_spawn][3];

    foreach(new i : Game[GAME_BTR]) {
        SavePlayerPosition(i);
        ResetPlayerWeapons(i);
        giveWeapon(i, WEAPON_PARACHUTE, 1);

        HideNameTagForAll(i);
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0x00));

        SetPlayerHealth(i, 100.0);
        SetPlayerArmour(i, 0.0);

        ShowZoneForPlayer(i, RoyaleZone, setAlpha(C_RED, 160));
        ZoneFlashForPlayer(i, RoyaleZone, setAlpha(C_YELLOW, 160));

        RoyaleKills[i] = 0;
        UpdateGameStatus(i, GAME_BTR);
        for(new j = 0; j < 3; j++) PlayerTextDrawShow(i, tdInfo[j]);

        Teleport(i, false, x + math_random_float(-3.0, 7.0), y + math_random_float(-3.0, 7.0), z + 400.0, a, 0, VW_BTR, false);
    }

    CreateRoyaleWeapons(RoyaleArenaID);
    msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%s, %d %s)", gameName[GAME_BTR], gameCMD[GAME_BTR], royaleDataAreny[RoyaleArenaID][royale_name], Iter_Count(Game[GAME_BTR]), ConvertTimeFormat(Iter_Count(Game[GAME_BTR]), "osób", "osoba", "osoby", "osób"));
    return UpdateGameDraw(), 1;
}

stock CreateRoyaleWeapons(arenaid)
{
    for(new i = 0; i < MAX_ROYALE_WEAPONS; i++) {
        if(IsValidDynamicPickup(RoyaleWeaponPickup[i])) DestroyDynamicPickup(RoyaleWeaponPickup[i]);
        RoyaleWeaponID[i] = -1;
    }

    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_ROYALE_WEAPONS; i++) {
        if(IsValidDynamicPickup(RoyaleWeaponPickup[i])) DestroyDynamicPickup(RoyaleWeaponPickup[i]);
        randomPositionArea(unpackXYZR(royaleDataAreny[arenaid][royale_zone]), x, y);
        CA_FindZ_For2DCoord(x, y, z);
        if(z <= 0.0 || IsPosInWater(x, y, z)) { i--; continue; }
        z += 1.0;

        new weaponid = royaleWeaponData[random(sizeof(royaleWeaponData))];
        RoyaleWeaponPickup[i] = CreateDynamicPickup(GetWeaponModelById(weaponid), 1, x, y, z, .worldid=VW_BTR);
        RoyaleWeaponID[i] = weaponid;

        if(weaponid == 1337 || weaponid == 2137) printf(" >>> [BTR]: Dodano leczenie (x: %.2f, y: %.2f, z: %.2f)", x, y, z);
        else printf(" >>> [BTR]: Dodano broñ %s (x: %.2f, y: %.2f, z: %.2f)", WeaponNames[weaponid], x, y, z);
    }
    return 1;
}

stock UpdateBTR()
{
    if(!GameStarted[GAME_BTR]) return 1;
    if(Iter_Count(Game[GAME_BTR]) < 1) return StopBTR(true);
    if(Iter_Count(Game[GAME_BTR]) == 1) {
        new money = math_random(10000, 30000);
        foreach(new i : Game[GAME_BTR]) {
            achievement(i, ACH_BTR);
            giveMoney(i, money);

            CheckBet(i, BET_GAME, true);
            showGameDraw(i, form("~y~WYGRALES(AS) ~r~BATTLE ROYALE~y~!~n~~y~NAGRODA: ~r~%s$", strdot(money)));
            msgAll(C_CHAT, "Gracz {c}%s(%d){/c} wygra³(a) {c}Battle Royale{/c}! Iloœæ zabójstw: {c}%d{/c}.", getNick(i), i, RoyaleKills[i]);
        }
        return StopBTR(false);
    }

    switch(RoyaleZoneSwitch) {
        case ROYALE_ZONE_MOVE: {
            if(GameCountDown[GAME_BTR] < 1) {
                GameCountDown[GAME_BTR] = 20;
                RoyaleZoneSwitch = ROYALE_ZONE_STOP;

                RoyaleZoneDamage += 2.0;
                foreach(new i : Game[GAME_BTR]) msg(i, C_VIOLET2, "Strefa zatrzyma³a siê! Nastêpna strefa za: {c}%s{/c}.", ConvertTime(GameCountDown[GAME_BTR], TIME_TYPE_MS));
            } else {
                if(RoyaleZoneCount < 7) {
                    RoyaleZonePosition[0] += 4.5;
                    RoyaleZonePosition[1] += 7.0;
                    RoyaleZonePosition[2] -= 4.5;
                    RoyaleZonePosition[3] -= 7.0;
                } else {
                    RoyaleZonePosition[0] += 1.25;
                    RoyaleZonePosition[1] += 0.85;
                    RoyaleZonePosition[2] -= 1.25;
                    RoyaleZonePosition[3] -= 0.85;
                }

                DestroyZone(RoyaleZone);
                RoyaleZone = CreateZone(unpackXYZR(RoyaleZonePosition));
                CreateZoneBorders(RoyaleZone);
            }
        }

        case ROYALE_ZONE_STOP: {
            if(GameCountDown[GAME_BTR] < 1) {
                GameCountDown[GAME_BTR] = 10;
                RoyaleZoneSwitch = ROYALE_ZONE_MOVE;
                RoyaleZoneCount ++;
                foreach(new i : Game[GAME_BTR]) msg(i, C_VIOLET2, "Strefa zmniejsza siê! Zatrzyma siê za: {c}%s{/c}.", ConvertTime(GameCountDown[GAME_BTR], TIME_TYPE_MS));
            } 
        }
    }

    foreach(new i : Game[GAME_BTR]) {
        if(!IsPlayerInArea(i, unpackXYZR(RoyaleZonePosition))) {
            new Float:health; GetPlayerHealth(i, health);
            SetPlayerHealth(i, health - RoyaleZoneDamage);
            GameTextForPlayer(i, "~r~~h~WROC DO STREFY!", 1000, 3);
        }

        if(RoyaleZoneSwitch == ROYALE_ZONE_MOVE && GameCountDown[GAME_BTR] > 0) {
            HideZoneForPlayer(i, RoyaleZone);
            ShowZoneForPlayer(i, RoyaleZone, setAlpha(C_RED, 160));
            ZoneFlashForPlayer(i, RoyaleZone, setAlpha(C_YELLOW, 160));
        }
        UpdateGameStatus(i, GAME_BTR);
    }
    return 1;
}

stock StopBTR(bool:info)
{
    if(!GameStarted[GAME_BTR]) return 1;
    GameStarted[GAME_BTR] = false;
    GameCountDown[GAME_BTR] = 0;

    RoyaleArenaID = -1;
    DestroyZone(RoyaleZone);

    foreach(new i : Game[GAME_BTR]) {
        pGame[i] = -1;
        ShowNameTagForAll(i);
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_BTR]);

    UpdateGameDraw();
    KillTimer(GameTimer[GAME_BTR]);
    return (info) ? (msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona.", gameName[GAME_BTR], gameCMD[GAME_BTR])) : (1);
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(Iter_Contains(Game[GAME_BTR], killerid) && pGame[killerid] == GAME_BTR && pGame[playerid] == pGame[killerid]) {
            RoyaleKills[killerid] ++;
    
            msg(killerid, C_CHAT, "Zabi³eœ(aœ) gracza {c}%s(%d){/c} na zabawie! £¹czna iloœæ zabójstw: {c}%d{/c}.", getNick(playerid), playerid, RoyaleKills[killerid]);
            msg(playerid, C_LRED, "Zosta³eœ(aœ) zabity(a) przez: {c}%s(%d){/c}. Odpadasz z zabawy.", getNick(killerid), killerid);
        }
    }

    new iterate_max;
    if(Iter_Contains(Game[GAME_BTR], playerid) && pGame[playerid] == GAME_BTR && GameStarted[GAME_BTR]) {
        ShowNameTagForAll(playerid);
        SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
        for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
        LoadPlayerPosition(playerid);

        CheckBet(playerid, BET_GAME, false);

        pGame[playerid] = -1;
        Iter_SafeRemove(Game[GAME_BTR], playerid, iterate_max);
        playerid = iterate_max;
    }   
    return 1;
}

stock RoyalePickup(playerid, pickupid)
{
    if(pGame[playerid] == GAME_BTR && Iter_Contains(Game[GAME_BTR], playerid) && GameStarted[GAME_BTR]) {
        for(new i = 0; i < MAX_ROYALE_WEAPONS; i++) {
            if(pickupid == RoyaleWeaponPickup[i]) {
                if(IsValidDynamicPickup(RoyaleWeaponPickup[i])) DestroyDynamicPickup(RoyaleWeaponPickup[i]);

                switch(RoyaleWeaponID[i]) {
                    case 9: giveWeapon(playerid, 9, 1);
                    case 16: giveWeapon(playerid, 16, 3);
                    case 18: giveWeapon(playerid, 18, 3);
                    case 22: giveWeapon(playerid, 22, 10);
                    case 23: giveWeapon(playerid, 23, 8);
                    case 24: giveWeapon(playerid, 24, 7);
                    case 25: giveWeapon(playerid, 25, 5);
                    case 26: giveWeapon(playerid, 26, 4);
                    case 27: giveWeapon(playerid, 27, 7);
                    case 29: giveWeapon(playerid, 29, 30);
                    case 30: giveWeapon(playerid, 30, 30);
                    case 31: giveWeapon(playerid, 31, 50);
                    case 32: giveWeapon(playerid, 32, 50);
                    case 34: giveWeapon(playerid, 34, 5);
                    case 35: giveWeapon(playerid, 35, 3);
                    case 38: giveWeapon(playerid, 38, 10);

                    case 2137: {
                        new Float:health; GetPlayerHealth(playerid, health);
                        SetPlayerHealth(playerid, health + 50.0);
                    }

                    case 1337: {
                        new Float:armour; GetPlayerArmour(playerid, armour);
                        SetPlayerArmour(playerid, armour + 50.0);
                    }
                }   

                RoyaleWeaponID[i] = -1;
                break;
            }
        }
    }
    return 1;
}