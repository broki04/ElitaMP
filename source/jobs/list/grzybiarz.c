#include YSI_Coding\y_hooks

#define MAX_GRZYBY      30

new 
    Text3D:GrzybLabel[MAX_PLAYERS][MAX_GRZYBY],
    GrzybObject[MAX_PLAYERS][MAX_GRZYBY],
    bool:GrzybAlive[MAX_PLAYERS][MAX_GRZYBY],

    GrzybCount[MAX_PLAYERS],
    GrzybZone[MAX_PLAYERS],
    GrzybCountAlive[MAX_PLAYERS],
    GrzybReward[MAX_PLAYERS],
    bool:GrzybiarzZbiera[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
    GrzybCount[playerid] = 0;
    GrzybCountAlive[playerid] = MAX_GRZYBY;
    GrzybReward[playerid] = 0;
    GrzybiarzZbiera[playerid] = false;
    return continue(playerid);
}

stock Grzybiarz_Start(playerid)
{
    GrzybZone[playerid] = CreateZone(770.5, -459.5, 1016.5, -270.5);
    CreateZoneBorders(GrzybZone[playerid]);
    
    ShowZoneForPlayer(playerid, GrzybZone[playerid], setAlpha(C_BROWN, 160));
    ZoneFlashForPlayer(playerid, GrzybZone[playerid], setAlpha(C_LORANGE, 160));

    GrzybCount[playerid] = 0;
    GrzybReward[playerid] = (math_random(1000, 3000) * pJobLevel[playerid][JOB_GRZYBIARZ]) + (pJobReward[playerid][JOB_GRZYBIARZ] * 100) + ((JobDay == JOB_GRZYBIARZ) ? (1000) : (0));
    GrzybiarzZbiera[playerid] = false;

    UpdateJobDraw(playerid, JOB_GRZYBIARZ);
    SetHealth(playerid, 100.0, 0.0);
    return Grzybiarz_Random(playerid), 1;
}

stock Grzybiarz_Random(playerid)
{
    GrzybCountAlive[playerid] = MAX_GRZYBY;

    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_GRZYBY; i++) {
        validGrzyb: 
        randomPositionArea(770.5, -459.5, 1016.5, -270.5, x, y);
        CA_FindZ_For2DCoord(x, y, z);

        if(z <= 10.0 || IsPosInWater(x, y, z)) goto validGrzyb;
        z -= 0.07;

        new 
            Float:rx, Float:ry,
            grzybName[][] = {"Muchomor", "Pieczarka", "Borowik"},
            grzybColor[] = {C_RED, C_WHITE, C_BROWN},
            typeid = random(sizeof(grzybName))
        ;

        GetGroundRotation(x, y, 0.35, rx, ry);
        GrzybObject[playerid][i] = CreateDynamicObject(19124, x, y, z, rx, ry, 0.0, .worldid=GetPVarInt(playerid, "player_grzybiarz_world"), .playerid=playerid);
        SetDynamicObjectMaterial(GrzybObject[playerid][i], 0, -1, "none", "none", RGBAToARGB(grzybColor[typeid]));

        GrzybLabel[playerid][i] = CreateDynamic3DTextLabel(" ", -1, x, y, z + 0.25, 30.0, .worldid=GetPVarInt(playerid, "player_grzybiarz_world"), .playerid=playerid);
        GrzybAlive[playerid][i] = true;

        new gstr[128]; format(gstr, sizeof(gstr), "{%06x}%s "WHITE"#%d\n"LY"Kliknij "WHITE"ENTER "LY"aby zebraæ go!", grzybColor[typeid] >>> 8, grzybName[typeid], i + 1);
        UpdateDynamic3DTextLabelText(GrzybLabel[playerid][i], -1, gstr);
    }
    return 1;
}

stock GetNearestGrzyb(playerid)
{
    new id = -1;
    for(new i = 0; i < MAX_GRZYBY; i++) {
        if(GrzybAlive[playerid][i]) {
            new Float:x, Float:y, Float:z;
            GetDynamicObjectPos(GrzybObject[playerid][i], x, y, z);
            if(IsPlayerInRangeOfPoint(playerid, 2.0, x, y, z)) {
                id = i;
                break;
            }
        } 
    }
    return id;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new grzybid = GetNearestGrzyb(playerid);
    if(PRESSED(KEY_SECONDARY_ATTACK) && pJob[playerid] == JOB_GRZYBIARZ && !IsPlayerInAnyVehicle(playerid) && grzybid != -1 && !GrzybiarzZbiera[playerid]) {
        GrzybiarzZbiera[playerid] = true;

        ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, false, false, false, false, 0);
        SetTimerEx("Grzybiarz_Zbierz", 1500, false, "dd", playerid, grzybid);
    }
    return 1;
}

stock CheckGrzybiarzZone(playerid)
{
    if(pJob[playerid] == JOB_GRZYBIARZ && !IsPlayerInArea(playerid, 770.5, -459.5, 1016.5, -270.5)) {
        new Float:health; GetPlayerHealth(playerid, health);
        SetPlayerHealth(playerid, health - 5.0);
        GameTextForPlayer(playerid, "~r~~h~WROC DO TERENU PRACY!", 3000, 3);
    }
    return 1;
}

function Grzybiarz_Zbierz(playerid, grzybid)
{
    if(IsValidDynamic3DTextLabel(GrzybLabel[playerid][grzybid])) DestroyDynamic3DTextLabel(GrzybLabel[playerid][grzybid]);
    if(IsValidDynamicObject(GrzybObject[playerid][grzybid])) DestroyDynamicObject(GrzybObject[playerid][grzybid]);

    ClearAnimations(playerid);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    GrzybAlive[playerid][grzybid] = false;
    
    GrzybCountAlive[playerid] --;
    if(GrzybCountAlive[playerid] < 1) Grzybiarz_Random(playerid);

    giveMoney(playerid, GrzybReward[playerid]);
    addJobScore(playerid, math_random(30, 50));
    achievement(playerid, ACH_GRZYBIARZ);
    GrzybiarzZbiera[playerid] = false;

    GrzybCount[playerid] ++;
    msg(playerid, C_CHAT, "Zebra³eœ(aœ) ju¿ {c}%d{/c} grzyba! Nagroda: {c}%s${/c}.", GrzybCount[playerid], strdot(GrzybReward[playerid]));
    return UpdateJobDraw(playerid, JOB_GRZYBIARZ), 1;
}