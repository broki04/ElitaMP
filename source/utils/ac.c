#include YSI_Coding\y_hooks

function OnCheatDetected(playerid, ip_address[], type, code) {
    switch(code) {
        case 0, 1: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 5) return KickPlayer(playerid, -1, "Air Break");
        }

        case 2, 3: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 3) {
                if(pGame[playerid] == GAME_RC && Iter_Contains(Game[GAME_RC], playerid) && GameStarted[GAME_RC]) {
                    if(IsValidVehicle(RaceVehicle[playerid])) DestroyVehicle(RaceVehicle[playerid]);
                    if(IsValidDynamicRaceCP(RaceCheckpoint[playerid])) DestroyDynamicRaceCP(RaceCheckpoint[playerid]);
                    if(IsValidDynamicMapIcon(RaceMapIcon[playerid])) DestroyDynamicMapIcon(RaceMapIcon[playerid]);

                    DisableRemoteVehicleCollisions(playerid, false);
                    CheckBet(playerid, BET_GAME, false);

                    SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
                    for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
                    LoadPlayerPosition(playerid);

                    msg(playerid, C_RED, "Wykryto prawdopodobieñstwo u¿ycia {c}Teleport Hacka{/c} - zosta³eœ(aœ) wyrzucony(a) z zabawy.");
                }
                return KickPlayer(playerid, -1, "Teleport Hack");
            }
        }

        case 8, 10: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 3) return KickPlayer(playerid, -1, "Speed Hack");
        }

        case 15, 16: {
            if(pCheatTemp[playerid]) return 1;
            pCheatWH[playerid] = true;
            if(GetPlayerVirtualWorld(playerid)) {
                pCheatDetected[playerid] ++;
                if(pCheatDetected[playerid] >= 3) return KickPlayer(playerid, -1, "Weapon Hack");
            }
        }

        case 26: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 3) return KickPlayer(playerid, -1, "Rapid Fire");
        }

        case 27, 28: return KickPlayer(playerid, -1, "Fake Kill");
        case 31, 32: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 4 /*&& !IsPlayerPaused(playerid)*/) return KickPlayer(playerid, -1, "Car Shooter");
        }

        case 34: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 3) return KickPlayer(playerid, -1, "AFK Ghost");
        }

        case 35: {
            pCheatDetected[playerid] ++;
            if(pCheatDetected[playerid] >= 5) {
                foreach(new i : Permission) if(playerData[i][player_admin] >= MODERATOR) msg(i, C_LY, ""CORAL"[AC]: "LY"Prawdopodobnie {c}%s(%d){/c} posiada aima!", getNick(playerid), playerid);
                pCheatDetected[playerid] = 0;
            }
        }
    }
    return 1;
}

hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(hittype != BULLET_HIT_TYPE_NONE && (!( -1000.0 <= fX <= 1000.0 ) || !( -1000.0 <= fY <= 1000.0 ) || !( -1000.0 <= fZ <= 1000.0))) return KickPlayer(playerid, -1, "Crasher"), 0;
	if(!((22 <= weaponid <= 34) || weaponid == 38)) return KickPlayer(playerid, -1, "Crasher"), 0;
    return 1;
}

IPacket:209(playerid, BitStream:bs) 
{
    new unoccupiedData[PR_UnoccupiedSync];
    BS_IgnoreBits(bs, 8);
    BS_ReadUnoccupiedSync(bs, unoccupiedData);
    if(unoccupiedData[PR_roll][0] < -100000 || unoccupiedData[PR_roll][1] < -100000 || unoccupiedData[PR_roll][2] < -100000) return 0;
    if(unoccupiedData[PR_roll][0] > 100000 || unoccupiedData[PR_roll][1] > 100000 || unoccupiedData[PR_roll][2] > 100000) return 0;
    return 1; 
}

IRPC:103(playerid, BitStream:bs) 
{
    new addr, len;
    BS_ReadValue(bs, PR_UINT8, len,PR_INT32, addr);
    if ((addr & 0xFC0000) != 0xFC0000) return 0;
    return 1; 
}

IPacket:211(playerid, BitStream:bs){
    if(GetPlayerVehicleSeat(playerid) == 0) return 0;
    return 1; 
}
