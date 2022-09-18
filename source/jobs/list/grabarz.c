#include YSI_Coding\y_hooks 

enum e_corpsNames {
    corp_name[50],
    corp_alt[50]
};

new 
    corpsNames[][e_corpsNames] = {
        {"Zielak", "Zielaka"},
        {"Lagowy", "Lagowego"},
        {"Natsu", "Natsu"},
        {"Simkowy", "Simkowego"},
        {"Shender", "Shendera"},
        {"Lou", "Lou"},
        {"Matka orena", "Matki orena"},
        {"Carbonara", "Carbonary"},
        {"Broki", "Brokiego"},
        {"Tenshin", "Tenshina"},
        {"Keef", "Keefa"},
        {"Stara keefa", "Starej keefa"},
        {"Vester", "Vestera"},
        {"Emetrox", "Emetroxa"},
        {"Yamashi", "Yamashiego"},
        {"Uhqqow", "Uhqqowa"},
        {"Krokodylek", "Krokodylka"},
        {"Roke", "Roke'a"},
        {"Szyneczka", "Szyneczki"},
        {"Rokitka", "Rokitki"},
        {"Yamashi", "Yamashiego"},
        {"Vester", "Vestera"},
        {"Seth", "Setha"},
        {"Acer", "Acera"}
    },

    Float:grabarzTravelPosition[][] = {
        {2494.6343, -1667.2212, 13.3438},
        {2238.8220, -1317.1860, 23.5521},
        {2102.6819, -1272.9213, 25.0577}
    },

    Float:grabarzYardPosition[][] = {
        {908.3058, -1110.8735, 24.1966},
        {916.4743, -1095.9435, 24.2969},
        {905.5636, -1079.7412, 24.2969},
        {878.0847, -1087.2424, 24.2969},
        {843.3021, -1111.5621, 24.1708},
        {838.4544, -1120.0718, 24.0290},
        {839.0642, -1077.8839, 24.2969}
    },

    grabarzCP[MAX_PLAYERS],
    Text3D:grabarzLabel[MAX_PLAYERS],
    grabarzMapIcon[MAX_PLAYERS],
    grabarzEtap[MAX_PLAYERS],
    grabarzVehicle[MAX_PLAYERS],
    bool:grabarzTrumna[MAX_PLAYERS],
    grabarzCorps[MAX_PLAYERS],
    grabarzReward[MAX_PLAYERS],
    grabarzVehicleOwner[MAX_VEHICLES],
    grabarzCount[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
    grabarzCP[playerid] = grabarzMapIcon[playerid] = grabarzEtap[playerid] = -1;
    grabarzLabel[playerid] = Text3D:-1;
    grabarzTrumna[playerid] = false;
    grabarzReward[playerid] = 0;
    grabarzCount[playerid] = 0;
    grabarzCorps[playerid] = -1;
    return continue(playerid);
}

stock Grabarz_RandomPosition(playerid)
{
    SavePlayerPosition(playerid);
    ResetPlayerWeapons(playerid);
    DisableRemoteVehicleCollisions(playerid, true);

    grabarzVehicle[playerid] = CreateVehicle(442, 924.7722, -1122.4525, 23.8300, 0.4851, -1, -1, -1);
    SetVehicleVirtualWorld(grabarzVehicle[playerid], VW_GRABARZ);
    LinkVehicleToInterior(grabarzVehicle[playerid], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, grabarzVehicle[playerid], 0);
    grabarzVehicleOwner[grabarzVehicle[playerid]] = playerid;

    if(IsValidDynamicCP(grabarzCP[playerid])) DestroyDynamicCP(grabarzCP[playerid]);
    if(IsValidDynamicMapIcon(grabarzMapIcon[playerid])) DestroyDynamicMapIcon(grabarzMapIcon[playerid]);

    new i = random(sizeof(grabarzTravelPosition));
    grabarzCP[playerid] = CreateDynamicCP(unpackXYZ(grabarzTravelPosition[i]), 2.0, VW_GRABARZ, .playerid=playerid, .streamdistance=99999.0);
    grabarzMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(grabarzTravelPosition[i]), 5, -1, .worldid=VW_GRABARZ, .style=MAPICON_GLOBAL, .streamdistance=99999.0, .playerid=playerid);
    grabarzLabel[playerid] = CreateDynamic3DTextLabel(form(""BLUE"Karawana\n"GREEN"W³aœciciel: "WHITE"%s(%d)", getNick(playerid), playerid), -1, 0.0, 0.0, -2000.0, 30.0, .attachedvehicle=grabarzVehicle[playerid], .worldid=VW_GRABARZ);

    grabarzEtap[playerid] = 0; 
    UpdateJobDraw(playerid, JOB_GRABARZ);
    return msg(playerid, C_LORANGE, "Udaj siê do {c}%s{/c}, by odebraæ zw³oki.", getZoneName(unpackXYZ(grabarzTravelPosition[i]))), 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(pJob[playerid] == JOB_GRABARZ && grabarzVehicleOwner[vehicleid] != playerid) {
        ClearAnimations(playerid);
        return msg(playerid, C_DRED, "Ten pojazd roboczy nale¿y do: {c}%s(%d){/c}.", getNick(grabarzVehicleOwner[vehicleid]), grabarzVehicleOwner[vehicleid]), 0;
    }
    return 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(checkpointid == grabarzCP[playerid] && pJob[playerid] == JOB_GRABARZ) {
        switch(grabarzEtap[playerid]) {
            case 0: {
                if(!IsPlayerInVehicle(playerid, grabarzVehicle[playerid])) return 1;
                dialogBox(playerid, C_BLUE, ""BLUE"Trwa odbieranie zw³ok...");

                TogglePlayerControllable(playerid, false);
                SetTimerEx("Grabarz_TravelDone", 1500, false, "d", playerid);
            }

            case 1: {
                if(IsPlayerInVehicle(playerid, grabarzVehicle[playerid])) return 1;
                if(!grabarzTrumna[playerid]) return dialogBox(playerid, C_RED, ""RED"Nie wzi¹³eœ(aœ) trumny z pojazdu! PodejdŸ do pojazdu i kliknij: "YELLOW"przycisk H"RED".");

                ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, false, false, false, false, 0);
                SetTimerEx("Grabarz_PochowajTrumna", 1000 * 2, false, "d", playerid);
            }
        }
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_CTRL_BACK) && pJob[playerid] == JOB_GRABARZ && IsPlayerInArea(playerid, 804.5, -1134.0, 958.5, -1049.0) && !grabarzTrumna[playerid]) {
        if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return dialogBox(playerid, C_RED, ""RED"Musisz zaparkowaæ gdzieœ obok karawanê.");
    
        new Float:x, Float:y, Float:z;
        GetVehiclePos(grabarzVehicle[playerid], x, y, z);
        if(!IsPlayerInRangeOfPoint(playerid, 5.0, x, y, z)) return dialogBox(playerid, C_RED, ""RED"Nie znajdujesz siê w pobli¿u karawany.");    

        ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, false, false, false, false, 0);
        SetTimerEx("Grabarz_TakeTrumna", 1500, false, "d", playerid);
    }
    return 1;
}

function Grabarz_PochowajTrumna(playerid)
{
    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 9);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    giveMoney(playerid, grabarzReward[playerid]);
    addJobScore(playerid, math_random(30, 70));
    achievement(playerid, ACH_GRABARZ);

    grabarzTrumna[playerid] = false;
    grabarzCount[playerid] --;
    
    if(grabarzCount[playerid] < 1) {
        if(IsValidDynamicCP(grabarzCP[playerid])) DestroyDynamicCP(grabarzCP[playerid]);
        if(IsValidDynamicMapIcon(grabarzMapIcon[playerid])) DestroyDynamicMapIcon(grabarzMapIcon[playerid]);

        new i = random(sizeof(grabarzTravelPosition));
        grabarzCP[playerid] = CreateDynamicCP(unpackXYZ(grabarzTravelPosition[i]), 2.0, VW_GRABARZ, .playerid=playerid, .streamdistance=99999.0);
        grabarzMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(grabarzTravelPosition[i]), 5, -1, .worldid=VW_GRABARZ, .style=MAPICON_GLOBAL, .streamdistance=99999.0, .playerid=playerid);
        grabarzEtap[playerid] = grabarzCount[playerid] = 0;
        
        msg(playerid, C_LORANGE, "Pochowa³eœ(aœ) trumnê {c}%s{/c}, teraz udaj siê do {c}%s{/c} odebraæ kolejne cia³a.", corpsNames[grabarzCorps[playerid]][corp_alt], getZoneName(unpackXYZ(grabarzTravelPosition[i])));
    } 
    else msg(playerid, C_LORANGE, "Pochowa³eœ(aœ) trumnê {c}%s{/c}. Pozosta³o jeszcze {c}%d {/c}%s.", corpsNames[grabarzCorps[playerid]][corp_alt], grabarzCount[playerid], ConvertTimeFormat(grabarzCount[playerid], "cia³", "cia³o", "cia³a", "cia³"));
    return UpdateJobDraw(playerid, JOB_GRABARZ), 1;
}

function Grabarz_TakeTrumna(playerid)
{
    ClearAnimations(playerid);
    SetPlayerAttachedObject(playerid, 9, 19339, 5, 0.09, 0.14, 0.13, -82.49, -11.20, 116.30, 0.51, 0.43, 0.59);    
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
 
    grabarzTrumna[playerid] = true;
    UpdateJobDraw(playerid, JOB_GRABARZ);
    return msg(playerid, C_LORANGE, "Odebra³eœ(aœ) {c}trumnê{/c}, teraz podejdŸ do {c}markeru{/c} przy grobie."), 1;
}

function Grabarz_TravelDone(playerid)
{
    if(IsValidDynamicCP(grabarzCP[playerid])) DestroyDynamicCP(grabarzCP[playerid]);
    if(IsValidDynamicMapIcon(grabarzMapIcon[playerid])) DestroyDynamicMapIcon(grabarzMapIcon[playerid]);

    grabarzCount[playerid] = (pJobLevel[playerid][JOB_GRABARZ] < 3) ? (math_random(1, 3)) : (math_random(2, 6));
    grabarzReward[playerid] = (math_random(1000, 3000) * pJobLevel[playerid][JOB_GRABARZ]) + (pJobReward[playerid][JOB_GRABARZ] * 100) + ((JobDay == JOB_GRABARZ) ? (1000) : (0));
    SetPVarInt(playerid, "player_grabarz_corps", grabarzCount[playerid]);
    addLevelScore(playerid, math_random(10, 15));

    new i = random(sizeof(grabarzYardPosition));
    grabarzCP[playerid] = CreateDynamicCP(unpackXYZ(grabarzYardPosition[i]), 1.0, VW_GRABARZ, .playerid=playerid, .streamdistance=99999.0);
    grabarzMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(grabarzYardPosition[i]), 5, -1, .worldid=VW_GRABARZ, .style=MAPICON_GLOBAL, .streamdistance=99999.0, .playerid=playerid);
    grabarzCorps[playerid] = random(sizeof(corpsNames));
    grabarzEtap[playerid] = 1;

    msg(playerid, C_LORANGE, "Udaj siê ponownie na {c}cmentarz{/c}, by pogrzebaæ %d %s rodziny {c}%s{/c}.", grabarzCount[playerid], ConvertTimeFormat(grabarzCount[playerid], "cia³", "cia³o", "cia³a", "cia³"), corpsNames[grabarzCorps[playerid]][corp_alt]);
    msg(playerid, C_LORANGE, "Pamiêtaj, by wyj¹æ {c}trumnê{/c} z karawany przyciskiem {c}H{/c}.");

    UpdateJobDraw(playerid, JOB_GRABARZ);
    return TogglePlayerControllable(playerid, true), 1;
}
