#include YSI_Coding\y_hooks

#define MAX_HUNTING     20

new 
    HuntingZone[MAX_PLAYERS],
    HuntingReward[MAX_PLAYERS],
    HuntingCount[MAX_PLAYERS],
    HuntingCountAlive[MAX_PLAYERS],

    bool:HuntingAlive[MAX_PLAYERS][MAX_HUNTING],
    HuntingObject[MAX_PLAYERS][MAX_HUNTING],
    Text3D:HuntingLabel[MAX_PLAYERS][MAX_HUNTING],
    Float:HuntingHealth[MAX_PLAYERS][MAX_HUNTING]
;

hook function ResetPlayerData(playerid)
{
    HuntingCountAlive[playerid] = MAX_HUNTING;
    HuntingCount[playerid] = 0;
    HuntingReward[playerid] = 0;
    return continue(playerid);
}

stock Mysliwy_Start(playerid)
{
    HuntingZone[playerid] = CreateZone(-716.5, -2144.5, -393.5, -1891.5);
    CreateZoneBorders(HuntingZone[playerid]);

    ShowZoneForPlayer(playerid, HuntingZone[playerid], setAlpha(C_GREEN, 160));
    ZoneFlashForPlayer(playerid, HuntingZone[playerid], setAlpha(C_CORAL, 160));

    HuntingCount[playerid] = 0;
    HuntingReward[playerid] = (math_random(1000, 3000) * pJobLevel[playerid][JOB_MYSLIWY]) + (pJobReward[playerid][JOB_MYSLIWY] * 100) + ((JobDay == JOB_MYSLIWY) ? (1000) : (0));

    UpdateJobDraw(playerid, JOB_MYSLIWY);
    giveWeapon(playerid, 33, 1000);
    SetHealth(playerid, 100.0, 100.0);
    return Mysliwy_Random(playerid), 1;
}

stock Mysliwy_Random(playerid)
{
    HuntingCountAlive[playerid] = MAX_HUNTING;

    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_HUNTING; i++) {
        validMysliwy:
        randomPositionArea(-716.5, -2144.5, -393.5, -1891.5, x, y);
        CA_FindZ_For2DCoord(x, y, z);
        if(z <= 25.0 || IsPosInWater(x, y, z)) goto validMysliwy;
        z += 1.0;

        new Float:rx, Float:ry; GetGroundRotation(x, y, 0.25, rx, ry);
        HuntingObject[playerid][i] = CreateDynamicObject(19315, x, y, z, rx, ry, 0.0, .worldid=GetPVarInt(playerid, "player_mysliwy_world"), .playerid=playerid);
        HuntingLabel[playerid][i] = CreateDynamic3DTextLabel(" ", -1, x, y, z + 0.25, 30.0, .worldid=GetPVarInt(playerid, "player_mysliwy_world"), .playerid=playerid);

        HuntingAlive[playerid][i] = true;
        HuntingHealth[playerid][i] = math_random_float(100.0, 300.0);

        new mstr[128]; format(mstr, sizeof(mstr), ""CORAL"SARNA "WHITE"#%d\n"LY"¯ycie: "WHITE"%.2f HP", i + 1, HuntingHealth[playerid][i]);
        UpdateDynamic3DTextLabelText(HuntingLabel[playerid][i], -1, mstr);
    }
    return 1;
}

stock CheckMysliwyZone(playerid)
{
    if(pJob[playerid] == JOB_MYSLIWY && !IsPlayerInArea(playerid, -716.5, -2144.5, -393.5, -1891.5)) {
        new Float:health; GetPlayerHealth(playerid, health);
        SetPlayerHealth(playerid, health - 5.0);
        GameTextForPlayer(playerid, "~r~~h~WROC DO TERENU PRACY!", 3000, 3);
    }
    return 1;
}

hook OnPlayerShootDynObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
    if(pJob[playerid] == JOB_MYSLIWY) {
        for(new sarnaid = 0; sarnaid < MAX_HUNTING; sarnaid++) {
            if(objectid == HuntingObject[playerid][sarnaid] && HuntingAlive[playerid][sarnaid] && weaponid == 33) {
                HuntingHealth[playerid][sarnaid] -= math_random_float(70.0, 100.0);
                if(HuntingHealth[playerid][sarnaid] <= 0.0) {
                    if(IsValidDynamicObject(HuntingObject[playerid][sarnaid])) DestroyDynamicObject(HuntingObject[playerid][sarnaid]);
                    if(IsValidDynamic3DTextLabel(HuntingLabel[playerid][sarnaid])) DestroyDynamic3DTextLabel(HuntingLabel[playerid][sarnaid]);

                    HuntingHealth[playerid][sarnaid] = 0.0;
                    HuntingAlive[playerid][sarnaid] = false;

                    HuntingCountAlive[playerid] --;
                    if(HuntingCountAlive[playerid] < 1) Mysliwy_Random(playerid);

                    giveMoney(playerid, HuntingReward[playerid]);
                    addJobScore(playerid, math_random(30, 50));
                    achievement(playerid, ACH_MYSLIWY);

                    HuntingCount[playerid] ++;
                    msg(playerid, C_CHAT, "Zabi³eœ(aœ) {c}%d{/c} sarnê! Nagroda: {c}%s${/c}.", HuntingCount[playerid], strdot(HuntingReward[playerid]));
                } else {
                    new mstr[128]; format(mstr, sizeof(mstr), ""CORAL"SARNA "WHITE"#%d\n"LY"¯ycie: "WHITE"%.2f HP", sarnaid + 1, HuntingHealth[playerid][sarnaid]);
                    UpdateDynamic3DTextLabelText(HuntingLabel[playerid][sarnaid], -1, mstr);
                }
                UpdateJobDraw(playerid, JOB_MYSLIWY);
            }
        }
    } 
    return 1;
}