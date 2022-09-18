#include YSI_Coding\y_hooks

enum e_warePaczkaData {
    ware_name[25],
    ware_money,
    bool:ware_premium
};

new 
    warePaczkaData[][e_warePaczkaData] = {
        {"Bizuteria",       400,    false},
        {"Elektronika",     300,    false},
        {"Odziez",          600,    false},
        {"Zywnosc",         200,    false},
        {"Zabawki",         300,    false},

        {"Diamenty",        700,    true},
        {"Z³oto",           800,    true},
        {"Narkotyki",       1000,   true},
        {"Medykamenty",     300,    true},
        {"Broñ",            1200,   true}
    },

    Float:MagazynPozycje[][] = {
        {1333.0013, 1038.2758, 1190.5111},
        {1333.0016, 1056.4513, 1190.5111},
        {1326.6476, 1056.3250, 1190.5111},
        {1318.6769, 1056.3888, 1190.5111},
        {1310.3745, 1056.3574, 1190.5111},
        {1304.2343, 1056.4257, 1190.5111},
        {1326.6475, 1038.4371, 1190.5111},
        {1318.6790, 1038.4602, 1190.5111},
        {1310.5387, 1038.4967, 1190.5111},
        {1304.2350, 1038.4324, 1190.5111}
    },

    magazynCP[MAX_PLAYERS],
    Text3D:magazynLabel[MAX_PLAYERS],

    bool:magazynTowar[MAX_PLAYERS],
    magazynPlacement[MAX_PLAYERS],
    magazynPaczkaType[MAX_PLAYERS], 
    magazynPaczkaSize[MAX_PLAYERS],
    magazynReward[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
    magazynCP[playerid] = -1;
    magazynLabel[playerid] = Text3D:-1;

    magazynTowar[playerid] = false;
    magazynPlacement[playerid] = magazynPaczkaType[playerid] = magazynPaczkaSize[playerid] = -1;
    magazynReward[playerid] = 0;
    return continue(playerid);
}

stock Magazynier_Fetch(playerid)
{
    magazynTowar[playerid] = false;
    magazynPaczkaType[playerid] = -1;
    return UpdateJobDraw(playerid, JOB_MAGAZYNIER), 1;
}

stock Magazynier_Pickup(playerid, pickupid)
{
    if(pickupid == magazynierPickup && pJob[playerid] == JOB_MAGAZYNIER && !magazynTowar[playerid]) {
        SetPlayerAttachedObject(playerid, 9, 1271, 5, 0.098000, 0.144999, 0.092999, 0.000000, 22.300001, 0.000000, 0.516000, 0.431000, 0.592000);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

        validPaczkaType:
        new type = random(sizeof(warePaczkaData));
        if(warePaczkaData[type][ware_premium] && !IsPremium(playerid)) goto validPaczkaType;

        magazynTowar[playerid] = true;
        magazynPaczkaSize[playerid] = random(3);
        magazynPaczkaType[playerid] = type;
        magazynReward[playerid] = (warePaczkaData[type][ware_money] * pJobLevel[playerid][JOB_MAGAZYNIER]) + (pJobReward[playerid][JOB_MAGAZYNIER] * 100) + ((JobDay == JOB_MAGAZYNIER) ? (500) : (0));
            
        magazynPlacement[playerid] = random(sizeof(MagazynPozycje));
        if(IsValidDynamicCP(magazynCP[playerid])) DestroyDynamicCP(magazynCP[playerid]);
        if(IsValidDynamic3DTextLabel(magazynLabel[playerid])) DestroyDynamic3DTextLabel(magazynLabel[playerid]);

        new mstr[128]; format(mstr, sizeof(mstr), ""YELLOW"PÓ£KA "LY"#%d\n"LBLUE"PodejdŸ do "RED"czerwonego znacznika"LBLUE", by od³o¿yæ towar!", magazynPlacement[playerid] + 1);
        magazynLabel[playerid] = CreateDynamic3DTextLabel(mstr, -1, unpackXYZ(MagazynPozycje[magazynPlacement[playerid]]) + 0.25, 30.0, .worldid=VW_MAGAZYNIER, .playerid=playerid);
        magazynCP[playerid] = CreateDynamicCP(unpackXYZ(MagazynPozycje[magazynPlacement[playerid]]), 2.0, .worldid=VW_MAGAZYNIER, .playerid=playerid, .priority=1);

        s[0] = EOS;
        catstr(s, C_BLUE, -1, "Zebrano {c}paczkê{/c} z punktu zaopatrzeniowego.\n");
        catstr(s, C_YELLOW, -1, "Typ towaru: {c}%s{/c}.\n", warePaczkaData[type][ware_name]);
        catstr(s, C_YELLOW, -1, "Wartoœæ towaru: {c} %s${/c}.", strdot(magazynReward[playerid]));
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""CORAL"Punkt zaopatrzeniowy", s, "OK", #);
    }
    return 1;
}

stock Magazynier_CP(playerid, checkpointid)
{
    if(checkpointid == magazynCP[playerid] && pJob[playerid] == JOB_MAGAZYNIER) {
        if(!magazynTowar[playerid]) return dialogBox(playerid, C_RED, ""RED"Nie masz ze sob¹ towaru. Aby go wzi¹æ, podejdŸ pod gara¿.");

        new i = magazynPlacement[playerid];
        if(IsPlayerInRangeOfPoint(playerid, 2.0, MagazynPozycje[i][0], MagazynPozycje[i][1], MagazynPozycje[i][2])) {
            ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, false, false, false, false, 0);
            SetTimerEx("MagazynPacked", 1000, false, "d", playerid);
        }
    }
    return 1;
}

function MagazynPacked(playerid)
{
    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 9);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Od³o¿y³eœ(aœ) "BLUE"towar "WHITE"do pó³ki magazynowej.\n"WHITE"Otrzymujesz: "ORANGE"%s$.\n\n"LY"Udaj siê punktu zaopatrzeniowego, by odebraæ nowy towar.", strdot(magazynReward[playerid]));
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Nagroda", s, "OK", #);

    giveMoney(playerid, magazynReward[playerid]);
    addJobScore(playerid, math_random(3, 7));

    achievement(playerid, ACH_MAGAZYNIER);
    CheckQuest(playerid, DAILY_PRACA);
    UpdateJobDraw(playerid, JOB_MAGAZYNIER);

    if(IsValidDynamicCP(magazynCP[playerid])) DestroyDynamicCP(magazynCP[playerid]);
    if(IsValidDynamic3DTextLabel(magazynLabel[playerid])) DestroyDynamic3DTextLabel(magazynLabel[playerid]);

    magazynTowar[playerid] = false;
    magazynPaczkaType[playerid] = magazynPlacement[playerid] = -1;
    return Magazynier_Fetch(playerid), 1;
}
