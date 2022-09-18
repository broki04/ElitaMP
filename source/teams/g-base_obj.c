#include YSI_Coding\y_hooks

enum e_GangBaseObjects {
    base_obj_name[35],
    base_obj_modelid
};

new 
    GangBaseObjects[][e_GangBaseObjects] = {
        {"Choinka",                 19076},
        {"Ognisko",                 19632},
        {"Smietnik",                1328},
        {"Smietnik",                1343},
        {"Smietnik",                1331},
        {"Smietnik",                1334},
        {"Zwloki policjanta",       3092},
        {"Wrak samochodu",          12957},
        {"Worki z cialami",         16444},
        {"Parkiet disco",           19128},
        {"Parkiet disco",           19129},
        {"Perkusja",                19609},
        {"Zlota plyta",             19617},
        {"BoomBox",                 2226},
        {"Oswietlenie",             19143},
        {"Oswietlenie",             19150},
        {"Oswietlenie",             19152},
        {"Bomba C4",                1252},
        {"Replika snajperki",       2036},
        {"Kapsula z ammo",          3082},
        {"Mina zblizeniowa",        19602},
        {"Skrzynka z amunicja",     2040},
        {"Roslinnosc",              639},
        {"Roslinnosc",              789},
        {"Roslinnosc",              818},
        {"Roslinnosc",              819},
        {"Roslinnosc",              1361},
        {"Roslinnosc",              3532},
        {"Roslinnosc",              3946},
        {"Roslinnosc",              8623},
        {"Roslinnosc",              14468},
        {"Skrzynka z bronia",       3014},
        {"Dynamit",                 1954},
        {"Dildo",                   321},
        {"Dildo",                   323},
        {"Ring bokserski",          14781},
        {"Kibel",                   2521},
        {"Kibel",                   2525},
        {"Kibel",                   2514},
        {"Kibel",                   2738},
        {"Papier do kibla",         19873},
        {"Automat",                 1775},
        {"Automat",                 955},
        {"Automat",                 956},
        {"Automat",                 1776},
        {"Plan budynku",            3111},
        {"Glowa CJ",                18963},
        {"Logo SAMP",               18749},
        {"Laptop SAMP",             19893},
        {"Krowa",                   19833},
        {"Rogata czaszka",          6865}
    },

    GangBaseObjectCount[MAX_GANGBASES],
    GangBaseObjectIndex[MAX_GANGBASES][MAX_BASE_OBJECTS],
    GangBaseObjectID[MAX_GANGBASES][MAX_BASE_OBJECTS],
    GangBaseObjectData[MAX_GANGBASES][MAX_BASE_OBJECTS],
    Float:GangBaseObjectPosition[MAX_GANGBASES][MAX_BASE_OBJECTS][6]
;
stock GetBaseObjectCount(baseid) return GangBaseObjectCount[baseid];

function FetchBaseObjects(gangid, baseid)
{
    if(!cache_num_rows()) return 1;
    GangBaseObjectCount[baseid] = cache_num_rows();

    new id, index, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz;
    for(new i = 0; i < cache_num_rows(); i++) {
        cache_get_value_int(i, "id", id);
        cache_get_value_int(i, "listitem", index);
        cache_get_value_float(i, "x", x);
        cache_get_value_float(i, "y", y);
        cache_get_value_float(i, "z", z);
        cache_get_value_float(i, "rx", rx);
        cache_get_value_float(i, "ry", ry);
        cache_get_value_float(i, "rz", rz);

        GangBaseObjectPosition[baseid][i][0] = x;
        GangBaseObjectPosition[baseid][i][1] = y;
        GangBaseObjectPosition[baseid][i][2] = z;
        GangBaseObjectPosition[baseid][i][3] = rx;
        GangBaseObjectPosition[baseid][i][4] = ry;
        GangBaseObjectPosition[baseid][i][5] = rz;

        GangBaseObjectID[baseid][i] = id;
        GangBaseObjectIndex[baseid][i] = index;
        GangBaseObjectData[baseid][i] = CreateDynamicObject(GangBaseObjects[index][base_obj_modelid], x, y, z, rx, ry, rz, .interiorid=0, .worldid=0);
    }
    return 1;
}

stock ShowBaseObjectList(playerid, gangid, baseid)
{
    new List:l = list_new();
    for(new i = 0; i < sizeof(GangBaseObjects); i++) AddModelMenuItem(l, GangBaseObjects[i][base_obj_modelid], GangBaseObjects[i][base_obj_name], true, 0.0, 0.0, -15.0);
    
    new response[E_MODEL_SELECTION_INFO];
    await_arr(response) ShowAsyncModelSelectionMenu(playerid, "Obiekty do bazy", l);

    if(response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) {
        if(GangBaseObjectCount[baseid] >= MAX_BASE_OBJECTS) return dialogBox(playerid, C_RED, "Baza twojego zespo³u posiada ju¿ maksymaln¹ iloœæ obiektów!");
        if(gangData[gangid][gang_budget] < BASE_OBJECT_COST) return dialogBox(playerid, C_RED, "Twojego zespo³u nie staæ na to! Potrzebujecie jeszcze: {c}"EURO"%s{/c}.", strdot(BASE_OBJECT_COST - gangData[gangid][gang_budget]));

        dbstr[0] = EOS;
        format(dbstr, sizeof(dbstr), "INSERT INTO es_gangs_basesobj (baseid, listitem, x, y, z, rx, ry, rz) VALUES (%d, %d, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);", baseid, response[E_MODEL_SELECTION_INDEX]);
        m_pquery(dbstr, "OnBaseObjectBuy", "dddd", playerid, gangid, baseid, response[E_MODEL_SELECTION_INDEX]);   
    }
    else OnDialogResponse(playerid, DIALOG_BASE_PANEL, 1, 6, "");
    return 1;
}

function OnBaseObjectBuy(playerid, gangid, baseid, index)
{
    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);

    new 
        objectid = CreateDynamicObject(GangBaseObjects[index][base_obj_modelid], x, y, z, 0.0, 0.0, 0.0, .worldid=0, .interiorid=0),
        cache = GangBaseObjectCount[baseid]
    ;

    GangBaseObjectData[baseid][cache] = objectid;
    GangBaseObjectID[baseid][cache] = cache_insert_id();
    GangBaseObjectIndex[baseid][cache] = index;
    GangBaseObjectCount[baseid] ++;

    SetPVarInt(playerid, "player_baseid", baseid);
    ShowBaseObjectsMenu(playerid, baseid, cache);

    foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zakupi³ obiekt {c}\"%s\"{/c} do bazy! {c}("EURO"%s){/c}", getGangTag(gangid), GangBaseObjects[index][base_obj_name], strdot(BASE_OBJECT_COST));
    return giveGangBudget(gangid, -BASE_OBJECT_COST), 1;
}

function ShowBaseObjectShop(playerid, baseid)
{
    s[0] = EOS;
    catstr(s, C_BLUE2, -1, "Dodaj nowy obiekt "YELLOW"("EURO"%s, {c}%d/%d{/c})\n", strdot(BASE_OBJECT_COST), GetBaseObjectCount(baseid), MAX_BASE_OBJECTS);
    
    if(GetBaseObjectCount(baseid) > 0) {
        strcat(s, " \n");

        new Cache:result = mysql_query(dbconn, form("SELECT * FROM es_gangs_basesobj WHERE baseid=%d LIMIT %d;", baseid, MAX_BASE_OBJECTS));

        new index, x = 0;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "listitem", index);
            
            format(s, sizeof(s), "%s"YELLOW""ARROW"  "WHITE"%s "CREAM"(modelid: %d)\n", s, GangBaseObjects[index][base_obj_name], GangBaseObjects[index][base_obj_modelid]);
            playerData[playerid][player_objectData][x++] = i;
        }
        cache_delete(result);
    }
    return showDialog(playerid, DIALOG_BASE_OBJECTS, DIALOG_STYLE_LIST, ""YELLOW"Obiekty w bazie", s, "Wybierz", "Cofnij"), 1;
}

stock ShowBaseObjectsMenu(playerid, baseid, i)
{
    new index = GangBaseObjectIndex[baseid][i];
    SetPVarInt(playerid, "player_baseid_cache", i);
    SetPVarInt(playerid, "player_baseid_sqlID", baseid);

    s[0] = EOS;
    strcat(s, ""WHITE"1. \t"GREEN"Edytuj po³o¿enie obiektu\n");
    strcat(s, ""WHITE"2. \t"GREEN"Usuñ obiekt\n");

    new title[90];
    format(title, sizeof(title), ""YELLOW"%s "WHITE"(modelid: %d)", GangBaseObjects[index][base_obj_name], GangBaseObjects[index][base_obj_modelid]);
    return showDialog(playerid, DIALOG_BASE_OBJECT_MENU, DIALOG_STYLE_LIST, title, s, "Wybierz", "Cofnij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BASE_OBJECT_MENU: {
            if(!response) return OnDialogResponse(playerid, DIALOG_BASE_PANEL, 1, 6, "");

            new cache = GetPVarInt(playerid, "player_baseid_cache"), baseid = GetPVarInt(playerid, "player_baseid_sqlID"), gangid = pGang[playerid];
            switch(listitem) {
                case 0: EditDynamicObject(playerid, GangBaseObjectData[baseid][cache]);
                case 1: {
                    inline onDeleteObject() {
                        new objectid = GangBaseObjectData[baseid][cache], index = GangBaseObjectIndex[baseid][cache];
                        if(IsValidDynamicObject(objectid)) DestroyDynamicObject(objectid);

                        GangBaseObjectCount[baseid] --;
                        GangBaseObjectIndex[baseid][cache] = -1;

                        giveGangBudget(gangid, 1000);
                        foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój gang sprzeda³ obiekt {c}%s{/c}. Zwrócono "EURO"{c}%s{/c}na konto.", getGangTag(gangid), GangBaseObjects[index][base_obj_name], strdot(1000));
                        SetTimerEx("ShowBaseObjectShop", 250, false, "dd", playerid, baseid);
                    }
                    m_tquery_inline(dbconn, using inline onDeleteObject, "DELETE FROM es_gangs_basesobj WHERE id=%d LIMIT 1;", GangBaseObjectID[baseid][cache]);
                }
            }
            return 1;
        }

        case DIALOG_BASE_OBJECTS: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

            new baseid = playerData[playerid][player_saveData][0], gangid = pGang[playerid];
            switch(listitem) {
                case 0: {
                    if(GangBaseObjectCount[baseid] >= MAX_BASE_OBJECTS) return dialogBox(playerid, C_RED, "Baza twojego zespo³u posiada ju¿ maksymaln¹ iloœæ obiektów!");
                    if(gangData[gangid][gang_budget] < BASE_OBJECT_COST) return dialogBox(playerid, C_RED, "Twojego zespo³u nie staæ na to! Potrzebujecie jeszcze: {c}"EURO"%s{/c}.", strdot(BASE_OBJECT_COST - gangData[gangid][gang_budget]));
                    return ShowBaseObjectList(playerid, gangid, baseid), 1;
                }
                case 1: return OnDialogResponse(playerid, DIALOG_BASE_PANEL, 1, 6, "");
                default: return ShowBaseObjectsMenu(playerid, baseid, playerData[playerid][player_objectData][listitem - 2]);
            }
            return 1;
        }
    }
    return 0;
}

hook OnPlayerEditDynObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    if(pGang[playerid] != -1 && GetPVarInt(playerid, "player_baseid_cache") != -1 && GetPVarInt(playerid, "player_baseid_sqlID") != -1) {
        new cache = GetPVarInt(playerid, "player_baseid_cache"), baseid = GetPVarInt(playerid, "player_baseid_sqlID");
   
        switch(response) {
            case EDIT_RESPONSE_CANCEL: {
                SetDynamicObjectPos(objectid, GangBaseObjectPosition[baseid][cache][0], GangBaseObjectPosition[baseid][cache][1], GangBaseObjectPosition[baseid][cache][2]);
                SetDynamicObjectRot(objectid, GangBaseObjectPosition[baseid][cache][3], GangBaseObjectPosition[baseid][cache][4], GangBaseObjectPosition[baseid][cache][5]);
            }

            case EDIT_RESPONSE_FINAL: {
                GangBaseObjectPosition[baseid][cache][0] = x;
                GangBaseObjectPosition[baseid][cache][1] = y;
                GangBaseObjectPosition[baseid][cache][2] = z;
                GangBaseObjectPosition[baseid][cache][3] = rx;
                GangBaseObjectPosition[baseid][cache][4] = ry;
                GangBaseObjectPosition[baseid][cache][5] = rz;

                SetDynamicObjectPos(objectid, GangBaseObjectPosition[baseid][cache][0], GangBaseObjectPosition[baseid][cache][1], GangBaseObjectPosition[baseid][cache][2]);
                SetDynamicObjectRot(objectid, GangBaseObjectPosition[baseid][cache][3], GangBaseObjectPosition[baseid][cache][4], GangBaseObjectPosition[baseid][cache][5]);

                m_pquery_format("UPDATE es_gangs_basesobj SET x=%f, y=%f, z=%f, rx=%f, ry=%f, rz=%f WHERE id=%d LIMIT 1;", GangBaseObjectPosition[baseid][cache][0], GangBaseObjectPosition[baseid][cache][1], GangBaseObjectPosition[baseid][cache][2], GangBaseObjectPosition[baseid][cache][3], GangBaseObjectPosition[baseid][cache][4], GangBaseObjectPosition[baseid][cache][5], GangBaseObjectID[baseid][cache]);
            }
        }
    }
    return 1;
}