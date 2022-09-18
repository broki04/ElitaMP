#include YSI_Coding\y_hooks

new 
    Float:DrwalTreePosition[][] = {
        {-1851.50, -2291.37, 51.88},
        {-1869.24, -2291.89, 51.88},
        {-1856.06, -2279.81, 55.98},
        {-1887.39, -2386.51, 24.63},
        {-1961.00, -2339.33, 27.63},
        {-1933.87, -2334.21, 34.41},
        {-1884.72, -2351.71, 34.41},
        {-1822.53, -2357.58, 34.41},
        {-1802.20, -2368.90, 34.21},
        {-1803.10, -2413.15, 30.31},
        {-1820.80, -2426.62, 30.31},
        {-1786.35, -2418.49, 26.13},
        {-1749.35, -2443.43, 26.13},
        {-1809.66, -2374.77, 29.51},
        {-1901.10, -2491.59, 29.51},
        {-2156.53, -2203.33, 35.38},
        {-2188.88, -2212.16, 35.38},
        {-2249.08, -2272.69, 29.18},
        {-2425.66, -2353.97, 21.60},
        {-2408.46, -2358.44, 23.00},
        {-2375.87, -2335.09, 23.53},
        {-2407.36, -2420.23, 27.53},
        {-2385.62, -2401.86, 27.53},
        {-2388.01, -2486.39, 27.93},
        {-2407.45, -2475.22, 27.93},
        {-2390.93, -2448.51, 27.53},
        {-1846.13, -1989.50, 81.28},
        {-1825.69, -1965.05, 81.28},
        {-1765.88, -1949.64, 81.28},
        {-1630.10, -2047.00, 59.95},
        {-1601.95, -2053.61, 62.35},
        {-1407.62, -2123.66, 17.92},
        {-1400.11, -2104.93, 17.92},
        {-1349.18, -2075.31, 20.36},
        {-1383.50, -2078.87, 20.36},
        {-1406.06, -2459.36, 37.02},
        {-1424.80, -2459.59, 39.28},
        {-1408.41, -2469.45, 39.28},
        {-1520.86, -2717.62, 56.23},
        {-1483.15, -2720.26, 57.26},
        {-1478.52, -2744.38, 57.26},
        {-1496.27, -2707.31, 56.25},
        {-1503.29, -2688.59, 56.05},
        {-1692.66, -2561.59, 12.27},
        {-1706.43, -2522.68, 11.30},
        {-1703.96, -2527.17, 11.30},
        {-1709.96, -2527.51, 9.90},
        {-2111.39, -2158.88, 43.69},
        {-2135.81, -2146.96, 43.69},
        {-2462.51, -2183.19, 30.72},
        {-2415.39, -2354.68, 22.67},
        {-2003.99, -1818.27, 40.15},
        {-1687.08, -1945.84, 101.18},
        {-1701.24, -1938.41, 101.84},
        {-1790.05, -2184.47, 72.17},
        {-1801.30, -2205.91, 72.17},
        {-1806.99, -2169.88, 74.88},
        {-1782.16, -2172.66, 69.08},
        {-1797.95, -2161.50, 69.08},
        {-1811.25, -2156.40, 69.08},
        {-2150.20, -2096.51, 58.25},
        {-2156.90, -2090.45, 58.25},
        {-2145.20, -2082.50, 58.25},
        {-2145.80, -2554.24, 29.39},
        {-2309.23, -2527.30, 26.34},
        {-2302.89, -2524.18, 26.34},
        {-2597.54, -2228.60, 24.88},
        {-2591.45, -2238.38, 24.88},
        {-2486.12, -2190.99, 29.20},
        {-2437.42, -2238.63, 18.80},
        {-1103.55, -2026.56, 41.67},
        {-1090.52, -2035.86, 41.67},
        {-1098.29, -2036.88, 39.29},
        {-1967.98, -2300.00, 33.64},
        {-2055.13, -2168.30, 49.43},
        {-2030.14, -2233.25, 43.02}
    },

    DrwalObject[MAX_PLAYERS][sizeof(DrwalTreePosition)],
    Text3D:DrwalLabel[MAX_PLAYERS][sizeof(DrwalTreePosition)],
    DrwalMapIcon[MAX_PLAYERS][sizeof(DrwalTreePosition)],
    Float:DrwalHealth[MAX_PLAYERS][sizeof(DrwalTreePosition)],
    bool:DrwalCutted[MAX_PLAYERS][sizeof(DrwalTreePosition)],

    DrwalVehicle[MAX_PLAYERS],
    DrwalCount[MAX_PLAYERS],
    DrwalTreeCount[MAX_PLAYERS],
    DrwalReward[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
    DrwalCount[playerid] = DrwalTreeCount[playerid] = 0;
    DrwalReward[playerid] = 0;
    return continue(playerid);
}

stock Drwal_Start(playerid)
{
    DrwalVehicle[playerid] = CreateVehicle(478, -1931.2423, -2449.1733, 30.6181, 309.4200, -1, -1, -1);
    SetVehicleVirtualWorld(DrwalVehicle[playerid], GetPVarInt(playerid, "player_drwal_world"));
    PutPlayerInVehicle(playerid, DrwalVehicle[playerid], 0);

    DrwalCount[playerid] = 0;
    DrwalTreeCount[playerid] = sizeof(DrwalTreePosition);
    DrwalReward[playerid] = (math_random(1000, 3000) * pJobLevel[playerid][JOB_DRWAL]) + (pJobReward[playerid][JOB_DRWAL] * 100) + ((JobDay == JOB_DRWAL) ? (1000) : (0));

    UpdateJobDraw(playerid, JOB_DRWAL);
    giveWeapon(playerid, WEAPON_CHAINSAW, 1);
    return Drwal_Spawn(playerid);
}

stock Drwal_Spawn(playerid)
{
    new Float:x, Float:y, Float:z, treeObject[] = {660, 696, 656, 731};
    for(new i = 0; i < sizeof(DrwalTreePosition); i++) {
        validRandomTrees:
        x = DrwalTreePosition[i][0];
        y = DrwalTreePosition[i][1];

        CA_FindZ_For2DCoord(x, y, z);
        if(IsPosInWater(x, y, z)) goto validRandomTrees;
        z -= 0.85;

        if(IsValidDynamicObject(DrwalObject[playerid][i])) DestroyDynamicObject(DrwalObject[playerid][i]);
        if(IsValidDynamic3DTextLabel(DrwalLabel[playerid][i])) DestroyDynamic3DTextLabel(DrwalLabel[playerid][i]);
        if(IsValidDynamicMapIcon(DrwalMapIcon[playerid][i])) DestroyDynamicMapIcon(DrwalMapIcon[playerid][i]);

        new objectid = treeObject[random(sizeof(treeObject))];
        switch(objectid) {
            case 660: DrwalHealth[playerid][i] = 2000.0;
            case 696: DrwalHealth[playerid][i] = 3500.0;
            case 656: DrwalHealth[playerid][i] = 2500.0;
            case 731: DrwalHealth[playerid][i] = 3000.0;
        }
        DrwalCutted[playerid][i] = false;

        DrwalObject[playerid][i] = CreateDynamicObject(objectid, x, y, z, 0.0, 0.0, 0.0, .worldid=GetPVarInt(playerid, "player_drwal_world"), .playerid=playerid);
        DrwalLabel[playerid][i] = CreateDynamic3DTextLabel(" ", -1, x, y, z + 0.35, 30.0, .worldid=GetPVarInt(playerid, "player_drwal_world"), .playerid=playerid);
        DrwalMapIcon[playerid][i] = CreateDynamicMapIcon(x, y, z, 56, -1, .worldid=GetPVarInt(playerid, "player_drwal_world"), .playerid=playerid, .streamdistance=99999.0);

        new dstr[128]; format(dstr, sizeof(dstr), ""BROWN"DRZEWO "WHITE"#%d\n"DRED"¯ywotnoœæ: "WHITE"%.2f HP", i + 1, DrwalHealth[playerid][i]);
        UpdateDynamic3DTextLabelText(DrwalLabel[playerid][i], -1, dstr); 
    }

    DrwalTreeCount[playerid] = sizeof(DrwalTreePosition);
    return 1;
}

stock GetNearestTree(playerid)
{
    new id = -1, Float:x, Float:y, Float:z;
    for(new i = 0; i < sizeof(DrwalTreePosition); i++) {
        GetDynamicObjectPos(DrwalObject[playerid][i], x, y, z);
        if(IsPlayerInRangeOfPoint(playerid, 3.5, x, y, z) && !DrwalCutted[playerid][i]) {
            id = i;
            break;
        }
    }
    return id;
}

function DrwalTree(playerid, treeid)
{
    new keys, ud, lr;
    GetPlayerKeys(playerid, keys, ud, lr);

    if((keys & KEY_FIRE) && treeid != -1) {
        DrwalHealth[playerid][treeid] -= math_random_float(70.0, 100.0);
        if(DrwalHealth[playerid][treeid] < 0.0 && !DrwalCutted[playerid][treeid]) {
            DrwalHealth[playerid][treeid] = 0.0;
            DrwalCutted[playerid][treeid] = true;

            if(IsValidDynamic3DTextLabel(DrwalLabel[playerid][treeid])) DestroyDynamic3DTextLabel(DrwalLabel[playerid][treeid]);
            if(IsValidDynamicMapIcon(DrwalMapIcon[playerid][treeid])) DestroyDynamicMapIcon(DrwalMapIcon[playerid][treeid]);

            new Float:x, Float:y, Float:z;
            GetDynamicObjectPos(DrwalObject[playerid][treeid], x, y, z);
            MoveDynamicObject(DrwalObject[playerid][treeid], x, y, z + 0.11, 0.03, 0.0, -85.0, 0.0);

            DrwalCount[playerid] ++;
            DrwalTreeCount[playerid] --;
            if(DrwalTreeCount[playerid] < 1) Drwal_Spawn(playerid);

            giveMoney(playerid, DrwalReward[playerid]);
            addJobScore(playerid, math_random(30, 50));
            achievement(playerid, ACH_DRWAL);

            playSound(playerid, 1139);
            UpdateJobDraw(playerid, JOB_DRWAL);
        } else {
            new dstr[128]; format(dstr, sizeof(dstr), ""BROWN"DRZEWO "WHITE"#%d\n"DRED"¯ywotnoœæ: "WHITE"%.2f HP", treeid + 1, DrwalHealth[playerid][treeid]);
            UpdateDynamic3DTextLabelText(DrwalLabel[playerid][treeid], -1, dstr); 
        }
        return SetTimerEx("DrwalTree", 100, false, "dd", playerid, treeid), 0;
    }

    playerData[playerid][player_holding] = false;
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if((newkeys & KEY_FIRE) && pJob[playerid] == JOB_DRWAL && !IsPlayerInAnyVehicle(playerid) && !playerData[playerid][player_holding]) {
        new treeid = GetNearestTree(playerid);
        if(treeid != -1) {
            DrwalTree(playerid, treeid);
            playerData[playerid][player_holding] = true;
        }
    }
    return 1;
}