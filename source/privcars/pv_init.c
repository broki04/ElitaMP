#define MAX_PRIVCAR_SLOTS           3
#define MAX_PRIVCAR_KEYS            5
#define MAX_PRIVCAR_ATTACHMENT      15
#define MAX_PRIVCAR_ULEPSZENIA      3

#define PP_COST_MONEY               100000

enum e_bakData {
    Float:bak_amount,
    bak_money
};

new
    pVeh[MAX_PRIVCARS],
    pVehOwner[MAX_PRIVCARS],
    pVehOwnerName[MAX_PRIVCARS][25],
    Text3D:pVehLabel[MAX_PRIVCARS],
    bool:pVehSpawned[MAX_PRIVCARS],
    pVehModel[MAX_PRIVCARS],
    Float:pVehPosition[MAX_PRIVCARS][4],
    pVehEtykieta[MAX_PRIVCARS][4],
    pVehEtykietaName[MAX_PRIVCARS][50],
    Float:pVehDist[MAX_PRIVCARS],
    pVehKeys[MAX_PRIVCARS][MAX_PRIVCAR_KEYS],
    pVehRegisterColor[MAX_PRIVCARS],
    pVehRegisterName[MAX_PRIVCARS][30],
    pVehComponent[MAX_PRIVCARS][14],
    pVehPaintJob[MAX_PRIVCARS],
    pVehColor[MAX_PRIVCARS][2],

    pVehObjectCount[MAX_PRIVCARS],
    pVehObject[MAX_PRIVCARS][MAX_PRIVCAR_ATTACHMENT],
    pVehObjectID[MAX_PRIVCARS][MAX_PRIVCAR_ATTACHMENT],
    pVehObjectData[MAX_PRIVCARS][MAX_PRIVCAR_ATTACHMENT],
    Float:pVehObjectPos[MAX_PRIVCARS][MAX_PRIVCAR_ATTACHMENT][3],
    Float:pVehObjectRot[MAX_PRIVCARS][MAX_PRIVCAR_ATTACHMENT][3],

    bool:pVehEdit[MAX_PLAYERS], 
    bool:pVehEditType[MAX_PLAYERS],
    Float:pVehEditorPos[MAX_PLAYERS][3], 
    Float:pVehEditorRot[MAX_PLAYERS][3],

    bakData[][e_bakData] = {
        {50.0,      50000},
        {70.0,      100000},
        {100.0,     300000}
    },

    Float:pVehFuel[MAX_PRIVCARS][2],
    pVehEngine[MAX_PRIVCARS],
    bool:pVehGas[MAX_PRIVCARS],
    bool:pVehGasEnabled[MAX_PRIVCARS],
    Float:pVehFuelAmount[MAX_PRIVCARS],

    pVehManage[MAX_PLAYERS], pVehCount[MAX_PLAYERS], pVehID[MAX_PLAYERS],
    pVehKeyData[MAX_PLAYERS][MAX_PRIVCAR_KEYS],
    pVehHave[MAX_PLAYERS][MAX_PRIVCAR_SLOTS + 2],
    pVehFetch[MAX_PLAYERS][MAX_PRIVCAR_SLOTS + 2],

    bool:pVehSell[MAX_PRIVCARS],
    pVehSellPoint[MAX_PRIVCARS],
    pVehSellCost[MAX_PRIVCARS],

    bool:VehiclePrivate[MAX_VEHICLES],
    VehiclePrivateID[MAX_VEHICLES]
;

enum {
    PV_DIALOG_MODEL = 6,
    PV_DIALOG_OBJECTS = 7,
    PV_DIALOG_BAK = 8,
    PV_DIALOG_GAS = 9,
};

#define GASOLINE_ENGINE         0
#define DIESEL_ENGINE           1

#define FUEL_AMOUNT             0
#define GAS_AMOUNT              1

#define FUEL_PB98       0
#define FUEL_ON         1
#define FUEL_LPG        2

new const guiPrivCarModels[][] = {
	{14,	48, 61, 62, 63, 68, 71, 81, 109, 110, 121, 122, 123, 181, 186}, //motors and bikes
	{4,		39, 80, 133, 155}, //cabriolets
	{26,	22, 82, 98, 99, 209, 124, 178, 55, 3, 14, 182, 43, 114, 200, 13, 115, 40, 143, 205, 59, 131, 8, 152, 78, 56, 154}, //industrials
	{8,		12, 134, 135, 136, 166, 167, 175, 176}, //lowriders
	{13,	0, 24, 44, 70, 89, 95, 100, 105, 156, 157, 168, 173, 179}, //offroad
	{16,	7, 16, 20, 27, 31, 33, 37, 38, 90, 128, 144, 196, 197, 198, 199, 201}, //public
	{33,	45, 104, 1, 118, 127, 142, 107, 162, 185, 19, 126, 204, 66, 92, 74, 146, 117, 10, 151, 116, 67, 26, 36, 147, 5, 180, 160, 150, 189, 140, 91, 129, 21}, //saloon
	{20,	2, 11, 15, 29, 51, 75, 77, 94, 96, 102, 103, 106, 141, 158, 159, 165, 187, 189, 202, 203}, //sport
	{21,	85, 57, 83, 108, 132, 86, 6, 130, 34, 145, 188, 171, 172, 23, 42, 28, 9, 174, 125, 183, 139} //unicals
};

#define PRIVCAR_TELEPORT_PLAYER         1
#define PRIVCAR_TELEPORT_VEHICLE        2

stock Init_PrivCars()
{
    for(new i = 0; i < MAX_VEHICLES; i++) {
        VehiclePrivate[i] = false;
        VehiclePrivateID[i] = -1;
    }

    for(new i = 0; i < MAX_PRIVCARS; i++) {
        pVehSpawned[i] = pVehSell[i] = false;
        pVehModel[i] = pVehOwner[i] = pVeh[i] = -1;
        pVehLabel[i] = Text3D:-1;
        pVehDist[i] = pVehFuel[i][0] = pVehFuel[i][1] = 0.0;
        pVehPosition[i] = Float:{0.0, 0.0, 0.0, 0.0};
        pVehFuelAmount[i] = 30.0;
        pVehRegisterColor[i] = C_GREEN;
        pVehObjectCount[i] = 0;
        pVehSellPoint[i] = -1;
        pVehSellCost[i] = 0;
        pVehEngine[i] = -1;
        pVehGas[i] = pVehGasEnabled[i] = false;

        for(new x = 0; x < MAX_PRIVCAR_ATTACHMENT; x++) {
            pVehObjectData[i][x] = pVehObjectID[i][x] = pVehObject[i][x] = -1;
            pVehObjectPos[i][x] = Float:{0.0, 0.0, 0.0};
            pVehObjectRot[i][x] = Float:{0.0, 0.0, 0.0};
        }
        for(new x = 0; x < MAX_PRIVCAR_KEYS; x++) pVehKeys[i][x] = -1;
    }
}

stock ResetPrivCarData(playerid)
{
    pVehCount[playerid] = 0;
    pVehID[playerid] = pVehManage[playerid] = -1;
    
    pVehHave[playerid] = {-1, -1, -1, -1, -1};
    pVehFetch[playerid] = {-1, -1, -1, -1, -1};
    pVehKeyData[playerid] = {-1, -1, -1, -1, -1};
}

stock GetPrivCarSlot(playerid, privcarid)
{
    new slotid = -1;
    for(new i = 0; i < 5; i++) {
        if(pVehHave[playerid][i] == privcarid) {
            slotid = i + 1;
            break;
        }
    }
    return slotid;
}

stock GetEtykietaColor(privcarid)
{
    new str[256];
    for(new i = 0; i < 4; i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pVehEtykieta[privcarid][i]);
    return str;
}

stock LoadPlayerPrivCars(playerid)
{
    inline onLoadPrivCars() {
        new id, bool:sell, modelid, count = 0;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", id);
            cache_get_value_name_int(i, "modelid", modelid);
            cache_get_value_name_bool(i, "sell", sell);

            if(sell) SetVehicleToSell(playerid, id, modelid);
            pVehHave[playerid][count] = id;
            count ++;
        }

        pVehCount[playerid] = count;
        if(pVehCount[playerid] > 0) msg(playerid, C_VIOLET, "Posiadasz {c}%d{/c} %s. Zarz¹dzanie pod: {c}/pp{/c}.", pVehCount[playerid], ConvertTimeFormat(pVehCount[playerid], "prywatnych pojazdów", "prywatny pojazd", "prywatne pojazdy", "prywatnych pojazdów"));
    }
    m_pquery_inline(dbconn, using inline onLoadPrivCars, "SELECT * FROM es_privcars WHERE ownerid=%d LIMIT %d;", playerData[playerid][player_accountID], (pUlepszenia[playerid][ULEPSZENIE_PRIVCAR]) ? (5) : (3));
    return 1;
}

stock CountServerPrivCars()
{
    new count = 0;
    inline onCheckCount() count = cache_num_rows();
    m_tquery_inline(dbconn, using inline onCheckCount, "SELECT * FROM es_privcars");
    return count;
}