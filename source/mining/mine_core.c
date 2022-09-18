#include YSI_Coding\y_hooks
#define MAX_MINES       100

enum e_mineData {
    mine_name[30],
    mine_color,
    mine_money
};

new 
    mineData[][e_mineData] = {
        {"Wêgiel",      0x4A4A4AFF,     100},
        {"MiedŸ",       0xB87333FF,     300},
        {"Srebro",      0xC0C0C0FF,     400},
        {"Ametyst",     0x9966CCFF,     500},
        {"Szmaragd",    0x19A56FFF,     700},
        {"Rubin",       0xFF6666FF,     500},
        {"Z³oto",       0xFFD700FF,     800},
        {"Diament",     0xB9F2FFFF,     1000},
        {"Uran",        0x2CFA1FFF,     1000}
    },

    bool:MineCreated[MAX_MINES],
    MineObject[MAX_MINES][4],
    Text3D:MineLabel[MAX_MINES],
    MineLevel[MAX_MINES],
    MineType[MAX_MINES],
    MineMiners[MAX_MINES],
    MineCounter[MAX_MINES],
    MineCount = 0,

    bool:MinePickaxe[MAX_PLAYERS],
    MinePickaxeLevel[MAX_PLAYERS],
    Float:MinePickaxeHealth[MAX_PLAYERS][2],
    bool:MineBag[MAX_PLAYERS], 
    MineBagLevel[MAX_PLAYERS], 
    MinePoints[MAX_PLAYERS],
    bool:MineStarted[MAX_PLAYERS], 
    MineWeight[MAX_PLAYERS][sizeof(mineData)],
    bool:MineSell[MAX_PLAYERS],
    MineSellNagroda[MAX_PLAYERS],
    MineSellVehicle[MAX_PLAYERS],
    Text3D:MineSellLabel[MAX_PLAYERS],
    MineSellIcon[MAX_PLAYERS],
    MineSellCP[MAX_PLAYERS],
    MineSellTimer[MAX_PLAYERS],
    MinePickaxeName[MAX_PLAYERS][50]
;

#define PICKAXE_HEALTH_STATUS   0
#define PICKAXE_HEALTH_LIMIT    1

hook function ResetPlayerData(playerid)
{
    strcat(MinePickaxeName[playerid], "");
    MineStarted[playerid] = false;
    MineSell[playerid] = false;
    MineSellNagroda[playerid] = 0;
    MineSellTimer[playerid] = 0;
    MinePickaxe[playerid] = false;
    MinePickaxeHealth[playerid] = Float:{0.0, 0.0};
    MineBag[playerid] = false;
    MineBagLevel[playerid] = MinePoints[playerid] = 0;
    for(new i = 0; i < sizeof(mineData); i++) MineWeight[playerid][i] = 0;
    return continue(playerid);
}

stock Init_Mining()
{
    MineCount = 0;
    if(OreDay == -1) OreDay = random(sizeof(mineData));
    printf(" >>> [MINE]: ¯y³a dnia: %s", mineData[OreDay][mine_name]);

    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_MINES; i++) {
        if(IsValidDynamic3DTextLabel(MineLabel[i])) DestroyDynamic3DTextLabel(MineLabel[i]);
        for(new j = 0; j < 4; j++) { if(IsValidDynamicObject(MineObject[i][j])) DestroyDynamicObject(MineObject[i][j]); }
    
        mineReload:
        GetRandomCoord(x, y, z);
        if(z >= 45.0 || IsPosInWater(x, y, z)) goto mineReload;

        MineCreated[i] = true;
        MineType[i] = random(sizeof(mineData));
        MineLevel[i] = math_random(16, 20);
        MineMiners[i] = math_random(3, 8);
        MineCounter[i] = 0;

        MineObject[i][0] = CreateDynamicObject(868, x, y, z, -27.899991, -43.599994, -84.300003, 0, 0, -1, 300.00, 300.00); 
        MineObject[i][1] = CreateDynamicObject(3930, x + 0.41, y - 0.51, z + 0.22, 0.000000, 0.000000, 0.000000, 0, 0, -1, 300.00, 300.00); 
        MineObject[i][2] = CreateDynamicObject(3930, x - 0.5, y + 0.14, z + 0.73, 0.000000, 0.000000, 77.200004, 0, 0, -1, 300.00, 300.00); 
        MineObject[i][3] = CreateDynamicObject(3930, x - 0.73, y - 0.63, z + 0.45, 0.000000, 0.000000, 77.200004, 0, 0, -1, 300.00, 300.00);

        SetDynamicObjectMaterial(MineObject[i][1], 0, -1, "none", "none", RGBAToARGB(mineData[MineType[i]][mine_color]));
        SetDynamicObjectMaterial(MineObject[i][2], 0, -1, "none", "none", RGBAToARGB(mineData[MineType[i]][mine_color]));
        SetDynamicObjectMaterial(MineObject[i][3], 0, -1, "none", "none", RGBAToARGB(mineData[MineType[i]][mine_color]));

        MineLabel[i] = CreateDynamic3DTextLabel(" ", -1, x, y, z, 30.0, .worldid=0, .interiorid=0);
        UpdateDynamic3DTextLabelText(MineLabel[i], -1, UpdateMineLabel(i));

        MineCount ++;
    }

    printf(" >>> [MINE]: Wczytanych ¿y³ górniczych: %d", MineCount);
    return msgAll(C_BROWN, ""PYTAJNIK""BROWN"Wszystkie {c}¿y³y górniczy{/c} zmieni³y swoje miejsca!"), 1;
}

stock GetMineStatus(mineid)
{
    new name[35];
    switch(MineLevel[mineid]) {
        case 16..20:    name = "{73ff77}||||||{363636}";
        case 11..15:    name = "{abff73}|||||{363636}|";
        case 7..10:     name = "{d7ff73}||||{363636}||";
        case 4..6:      name = "{ffe373}|||{363636}|||";
        case 2, 3:      name = "{ff9f73}||{363636}||||";
        default:        name = "{ff7373}|{363636}|||||";
    }
    return name;
}

stock UpdateMineLabel(mineid)
{
    new mstr[256];
    format(mstr, sizeof(mstr), "{%06x}%s "WHITE"[%s"WHITE"]\n", mineData[MineType[mineid]][mine_color] >>> 8, mineData[MineType[mineid]][mine_name], GetMineStatus(mineid));
    format(mstr, sizeof(mstr), "%s"WHITE"Wydobywaæ mog¹ jednoczeœnie "ORANGE"%d "WHITE"%s!\n", mstr, MineMiners[mineid], ConvertTimeFormat(MineMiners[mineid], "osób", "osoba", "osoby", "osób"));
    format(mstr, sizeof(mstr), "%s"WHITE"Aby wykopaæ, kliknij: "LY"SPACJA + Y\n", mstr);
    return mstr;
}

stock GetNearestOre(playerid)
{
    new mineid = -1, Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_MINES; i++) {
        if(MineCreated[i]) {
            GetDynamicObjectPos(MineObject[i][0], x, y, z);
            if(IsPlayerInRangeOfPoint(playerid, 2.0, x, y, z)) {
                mineid = i;
                break;
            }
        }
    }
    return mineid;
}

stock GetBagAmount(playerid)
{
    new amount;
    switch(MineBagLevel[playerid]) {
        case 1: amount = 100;
        case 2: amount = 200;
        case 3: amount = 300;
        case 4: amount = 400;
        default: amount = 500;
    }
    return amount;
}

stock GetBagWeight(playerid)
{
    new amount = 0;
    for(new i = 0; i < sizeof(mineData); i++) amount += MineWeight[playerid][i];
    return amount;
}

stock GetMineName(mineid)
{
    new name[35];
    switch(mineid) {
        case 0: name = "wêgla";
        case 1: name = "miedzi";
        case 2: name = "srebra";
        case 3: name = "ametystu";
        case 4: name = "szmaragdu";
        case 5: name = "rubinu";
        case 6: name = "z³ota";
        case 7: name = "diamentu";
        case 8: name = "uranu";
    }
    return name;
}

stock GetOreCost(playerid)
{
    new money = 0;
    for(new i = 0; i < sizeof(mineData); i++) money += mineData[i][mine_money] * MineWeight[playerid][i] + ((OreDay == i) ? (1000) : (0));
    return money;
}

stock GetMineWeight(playerid)
{
    new mstr[256];
    for(new i = 0; i < sizeof(mineData); i++) format(mstr, sizeof(mstr), "%s%s%i", mstr, i == 0 ? ("") : (","), MineWeight[playerid][i]);
    return mstr;
}

stock GetPickaxeLevel(playerid)
{
    new name[35];
    switch(MinePickaxeLevel[playerid]) {
        case 1:     name = "I{363636}IIII";
        case 2:     name = "II{363636}III";
        case 3:     name = "III{363636}II";
        case 4:     name = "IIII{363636}I";
        default:    name = "IIIII{363636}";
    }
    return name;
}
