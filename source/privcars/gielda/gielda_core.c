#include YSI_Coding\y_hooks

new 
    Float:gieldaSellPos[][] = {
        {1085.2715, 1352.6527, 10.5474, 269.0447},
        {1085.3651, 1342.4911, 10.5474, 269.2348},
        {1085.3300, 1332.2902, 10.5474, 270.7957},
        {1085.0302, 1321.4763, 10.5474, 270.2165},
        {1084.7301, 1311.6875, 10.5474, 270.9807},
        {1084.7811, 1301.1825, 10.5474, 272.2343},
        {1085.1863, 1290.8831, 10.5474, 268.2484},
        {1085.4695, 1281.5894, 10.5474, 269.9802},
        {1085.5238, 1271.6172, 10.5474, 268.1742},
        {1085.4056, 1260.1506, 10.5474, 268.9643},
        {1085.8129, 1250.0074, 10.5474, 271.3919}
    },

    gieldaSellCP[sizeof(gieldaSellPos)],
    Text3D:gieldaSellLabel[sizeof(gieldaSellPos)],
    gieldaSellOwned[sizeof(gieldaSellPos)]
;

stock Init_Gielda()
{
    for(new i = 0; i < sizeof(gieldaSellPos); i++) {        
        new gstr[256]; catstr(gstr, C_ZAJEBISTY, -1, ""BLUE2"PUNKT SPRZEDA¯Y "LY"#%d\n"ZAJEBISTY"WejdŸ w {c}marker{/c}, by wystawiæ pojazd.", i + 1);
        gieldaSellLabel[i] = CreateDynamic3DTextLabel(gstr, -1, unpackXYZ(gieldaSellPos[i]) + 1.0, 50.0, .worldid=0, .interiorid=0);
        gieldaSellCP[i] = CreateDynamicCP(unpackXYZ(gieldaSellPos[i]), 2.0, 0, 0);
        gieldaSellOwned[i] = -1;
    }
}

stock IsPlayerNearSellPoint(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(gieldaSellPos); i++) {
        if(IsPlayerInDynamicCP(playerid, gieldaSellCP[i]) && IsPlayerInAnyVehicle(playerid)) {
            id = i;
            break;
        }
    }
    return id;
}
stock IsPlayerInGielda(playerid) return IsPlayerInArea(playerid, 1013.5, 1199.5, 1097.5, 1369.5);

stock GetFreeGieldaPoint()
{
    new id = -1;
    for(new gieldaid = 0; gieldaid < sizeof(gieldaSellPos); gieldaid++) {
        if(gieldaSellOwned[gieldaid] == -1) {
            id = gieldaid;
            break;
        }
    }
    return id;
}

stock Task:GieldaSellPanel(playerid, privcarid, sellid)
{
    new Task:t = task_new();
    if(sellid == -1 || privcarid == -1 || !pVehSpawned[privcarid]) return t;

    SetPVarInt(playerid, "gielda_point", sellid);
    SetPVarInt(playerid, "gielda_privcarid", privcarid);    
    return showDialog(playerid, DIALOG_GIELDA_SELL, DIALOG_STYLE_INPUT, ""PINK"Wystawianie pojazdu na sprzeda¿", ""WHITE"Wpisz poni¿ej kwotê, za któr¹ chcesz "RED"wystawiæ pojazd "WHITE"na sprzeda¿:", "Wystaw", "Zamknij"), t;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new sellid = IsPlayerNearSellPoint(playerid), privcarid = pVehID[playerid];
    if(sellid != -1 && privcarid != -1 && gieldaSellOwned[sellid] == -1 && IsPlayerInVehicle(playerid, pVeh[privcarid]) && pVehOwner[privcarid] == playerData[playerid][player_accountID]) {
        setPosition(playerid, true, unpackXYZR(gieldaSellPos[sellid]));
        return task_await(GieldaSellPanel(playerid, privcarid, sellid));
    }
    return 1;
}

stock SetVehicleToSell(playerid, privcarid, modelid)
{
    if(privcarid == -1) return 1;

    new sellid = GetFreeGieldaPoint();
    if(sellid != -1) {
        SetPVarInt(playerid, "gielda_sell_point", sellid);
        CreatePrivCar(playerid, privcarid, false, 0);
    } else {
        m_pquery_format("UPDATE es_privcars SET sell=0, sell_cost='0, 0' WHERE id=%d LIMIT 1;", privcarid);
        msg(playerid, C_CORAL, "Sprzeda¿ twojego pojazdu {c}%s (ID: %d){/c} zosta³a anulowana z powodu {c}braku miejsc{/c} na gie³dzie.", GetVehicleName(modelid), privcarid);
    }
    return 1;
}

stock AddVehicleToGielda(playerid, privcarid)
{
    new sellid = GetPVarInt(playerid, "gielda_sell_point");
    SetVehiclePos(pVeh[privcarid], unpackXYZ(gieldaSellPos[sellid]));
    SetVehicleZAngle(pVeh[privcarid], gieldaSellPos[sellid][3]);
    SetVehicleVirtualWorld(pVeh[privcarid], 0);
    SwitchVehicleEngine(pVeh[privcarid], false);

    gieldaSellOwned[sellid] = privcarid;
    pVehSellPoint[privcarid] = sellid;

    new gstr[256]; catstr(gstr, C_ZAJEBISTY, -1, ""DRED"PUNKT SPRZEDA¯Y "YELLOW"#%d\n"LRED"Koszt sprzeda¿y: {c}%s${/c}\n"LRED"Aby odkupiæ, wpisz: {c}/pbuy %d{/c}", sellid + 1, strdot(pVehSellCost[privcarid]), sellid + 1);
    return UpdateDynamic3DTextLabelText(gieldaSellLabel[sellid], -1, gstr);
}

stock CountVehicleToSell()
{
    new count = 0;
    for(new i = 0; i < sizeof(gieldaSellPos); i++) { if(gieldaSellOwned[i] != -1) count ++; }
    return count;
}

stock CountVehicleSellPlayer(playerid)
{
    new Cache:result = mysql_query(dbconn, form("SELECT * FROM es_privcars WHERE ownerid=%d AND sell=1", playerData[playerid][player_accountID])), count = 0;
    count = cache_num_rows();
    cache_delete(result);
    return count;
}

stock ShowGieldaPanel(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Polecenie\t"MINT"Rezultat\n";
    catstr(s, C_WHITE, -1, "Rynek wystawionych pojazdów:\t"MINT"%d %s\n", CountVehicleToSell(), ConvertTimeFormat(CountVehicleToSell(), "pojazdów", "pojazd", "pojazdy", "pojazdów"));
    catstr(s, C_WHITE, -1, "Lista wystawionych Twoich pojazdów:\t"LY"%d %s\n", CountVehicleSellPlayer(playerid), ConvertTimeFormat(CountVehicleSellPlayer(playerid), "pojazdów", "pojazd", "pojazdy", "pojazdów"));
    catstr(s, C_WHITE, -1, "Teleportuj siê na "PINK"gie³dê\n");
    return showDialog(playerid, DIALOG_GIELDA_MAIN, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Panel gie³dy", s, "Wybierz", "Cofnij"), 1;
}