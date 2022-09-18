#include YSI_Coding\y_hooks

enum e_attachmentObjectData {
    attachment_modelid,
    attachment_name[90],
};

new const attachmentObjectData[][e_attachmentObjectData] = {
    {19308,     "Napis TAXI"},
    {359,       "Wyrzutnia rakiet"},
    {2985,      "Minigun"},
    {19601,     "Taran"},
    {11245,     "Flaga"},
    {19917,     "Silnik"},
    {3267,      "Armata"},
    {19620,     "Swiatla policyjne"},
    {18646,     "Lampa policyjna"},
    {19309,     "Napis TAXI"},
    {19310,     "Napis TAXI"},
    {19311,     "Napis TAXI"},
    {1014,      "Spojler"},
    {19128,     "Podloga disco"},
    {19444,     "Podloga XD"},
    {1252,      "Bomba C4"},
    {18694,     "Plomienie"},
    {3524,      "Czaszka na palu"},
    {6865,      "Czaszka"},
    {19314,     "Rogi"},
	{18656, 	"Biale swiatlo"},
	{18657, 	"Czerwone swiatlo"},
	{18658, 	"Niebieskie swiatlo"},
    {2226,      "BoomBox"},
    {1841,      "Maly glosnik"},
    {2231,      "Glosnik"},
    {2985, 		"Minigun"},
    {1254,      "Czaszka"},
    {19602,     "Mina"},
    {1247,      "Gwiazda"},
    {2061,      "Strzaly"},
    {1636,      "Rakieta"},
    {2064,      "Artyleria"},
    {1575,      "Paczka"},
    {2868,      "Swieczka"},
    {1503,      "Mala rampa"},
    {1025,      "Opona"},
    {1327,      "Opona"},
    {2993,      "Flaga"},
    {19843,     "Zardzewialy metal"},
    {920,       "Generator"},
    {19589,     "Smietnik"},
    {1094,      "Pasek"},
    {1090,      "Pasek"}
};

hook function DestroyVehicle(vehicleid)
{
    if(VehiclePrivate[vehicleid]) {
        new privcarid = VehiclePrivateID[vehicleid];
        if(pVeh[privcarid] == vehicleid) {
            for(new i = 0; i < MAX_PRIVCAR_ATTACHMENT; i++) {
                if(IsValidDynamicObject(pVehObjectData[privcarid][i])) DestroyDynamicObject(pVehObjectData[privcarid][i]);
                pVehObjectData[privcarid][i] = -1;
            }
        }

        VehiclePrivate[vehicleid] = false;
        VehiclePrivateID[vehicleid] = -1;
    }
    return continue(vehicleid);
}

stock ShowPrivCarObjects(playerid, privcarid)
{
    if(privcarid == -1 || !pVehSpawned[privcarid]) return 1;

    s[0] = EOS;
    catstr(s, C_BLUE, -1, ""BLUE"Dodaj nowy obiekt "YELLOW"(1.000$, "LY"%d/%d"YELLOW")\n", pVehObjectCount[privcarid], ((pVehUlepszenie[privcarid][ULEPSZENIE_SLOTS]) ? (15) : (10)));

    if(pVehObjectCount[privcarid] > 0) {
        strcat(s, " \n");

        new Cache:result = mysql_query(dbconn, form("SELECT * FROM es_privcars_attachments WHERE privcarid=%d ORDER BY id ASC LIMIT 10;", privcarid));

        new index, x = 0;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_int(i, "listitem", index);

            format(s, sizeof(s), "%s"YELLOW"»  "WHITE"%s "CREAM"(modelid: %d)\n", s, attachmentObjectData[index][attachment_name], attachmentObjectData[index][attachment_modelid]);
            playerData[playerid][player_objectData][x++] = i;
        }
        cache_delete(result);
    }
    return showDialog(playerid, DIALOG_PRIVCAR_ATTACHMENT, DIALOG_STYLE_LIST, ""VIOLET"Przyczepiane obiekty", s, "Wybierz", "Cofnij");
}

stock ShowAttachmentList(playerid, privcarid)
{
    new List:l = list_new();
    for(new i = 0; i < sizeof(attachmentObjectData); i++) AddModelMenuItem(l, attachmentObjectData[i][attachment_modelid], attachmentObjectData[i][attachment_name], true, 0.0, 0.0, -15.0);

    new response[E_MODEL_SELECTION_INFO];
    await_arr(response) ShowAsyncModelSelectionMenu(playerid, "Przyczepiane obiekty", l);

    if(response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) {
        if(getMoney(playerid) < 1000) return valueError(playerid, 1000);

        dbstr[0] = EOS;
        format(dbstr, sizeof(dbstr), "INSERT INTO es_privcars_attachments (privcarid, listitem, x, y, z, rx, ry, rz) VALUES (%d, %d, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);", privcarid, response[E_MODEL_SELECTION_INDEX]);
        m_pquery(dbstr, "OnPrivCarAttachmentBuy", "ddd", playerid, privcarid, response[E_MODEL_SELECTION_INDEX]);
    }
    else OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 5, "");
    return 1;
}

function OnPrivCarAttachmentBuy(playerid, privcarid, index)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(pVeh[privcarid], x, y, z);

    new objectid = CreateDynamicObject(attachmentObjectData[index][attachment_modelid], x, y, z, 0.0, 0.0, 0.0, .worldid=0, .interiorid=0);
    AttachDynamicObjectToVehicle(objectid, pVeh[privcarid], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    new cache = pVehObjectCount[privcarid];
    pVehObjectData[privcarid][cache] = objectid;
    pVehObjectID[privcarid][cache] = cache_insert_id();
    pVehObject[privcarid][cache] = index;
    pVehObjectCount[privcarid] ++;

    pVehEditorPos[playerid] = Float:{0.0, 0.0, 0.0};
    pVehEditorRot[playerid] = Float:{0.0, 0.0, 0.0};

    giveMoney(playerid, -1000);
    ShowPlayerObjectMenu(playerid, privcarid, cache);
    return msg(playerid, C_GREEN, "Zakupi³eœ(aœ) dodatek {c}%s (modelid: %d){/c} do swojego pojazdu.", attachmentObjectData[index][attachment_name], attachmentObjectData[index][attachment_modelid]), 1;
}

function LoadPrivCarAttachments(playerid, privcarid, bool:update)
{
    if(!cache_num_rows()) return 1;

    pVehObjectCount[privcarid] = cache_num_rows();

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

        if(update) {
            if(pVehObjectData[privcarid][i] != -1) {
                for(new j = 0; j < pVehObjectCount[privcarid]; j++) {
                    if(IsValidDynamicObject(pVehObjectData[privcarid][i])) {
                        DestroyDynamicObject(pVehObjectData[privcarid][i]);
                    }
                }
            }
        }

        pVehObjectID[privcarid][i] = id;
        pVehObject[privcarid][i] = index;

        pVehObjectPos[privcarid][i][0] = x;
        pVehObjectPos[privcarid][i][1] = y;
        pVehObjectPos[privcarid][i][2] = z;

        pVehObjectRot[privcarid][i][0] = rx;
        pVehObjectRot[privcarid][i][1] = ry;
        pVehObjectRot[privcarid][i][2] = rz;

        pVehObjectData[privcarid][i] = CreateDynamicObject(attachmentObjectData[index][attachment_modelid], unpackXYZ(pVehPosition[privcarid]), 0.0, 0.0, 0.0, .worldid=0, .interiorid=0);
        AttachDynamicObjectToVehicle(pVehObjectData[privcarid][i], pVeh[privcarid], x, y, z, rx, ry, rz);
    }
    return 1;
}

stock ShowPlayerObjectMenu(playerid, privcarid, i)
{
    new index = pVehObject[privcarid][i];
    SetPVarInt(playerid, "cache_attachment", i);
    SetPVarInt(playerid, "privcarid_attachment", privcarid);

    s[0] = EOS;
    strcat(s, ""WHITE"1. \t"GREEN"Edytuj po³o¿enie obiektu\n");
    strcat(s, ""WHITE"2. \t"GREEN"Edytuj rotacjê obiektu\n");
    strcat(s, ""WHITE"3. \t"GREEN"Usuñ obiekt\n");

    new title[90];
    format(title, sizeof(title), ""YELLOW"%s "WHITE"(modelid: %d)", attachmentObjectData[index][attachment_name], attachmentObjectData[index][attachment_modelid]);
    return showDialog(playerid, DIALOG_PRIVCAR_ATTACH_SELECT, DIALOG_STYLE_LIST, title, s, "Wybierz", "Cofnij"), 1;
}

stock OnPrivCarAttachmentDialog(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_PRIVCAR_ATTACHMENT: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid])); 

            new privcarid = pVehManage[playerid];
            if(privcarid == -1) return 1;

            switch(listitem) {
                case 0: {
                    if(pVehObjectCount[privcarid] >= ((pVehUlepszenie[privcarid][ULEPSZENIE_SLOTS]) ? (15) : (10))) return sendError(playerid, ""RED"Posiadasz ju¿ maksymaln¹ liczbê obiektów na pojeŸdzie.");
                    if(getMoney(playerid) < 1000) return valueError(playerid, 1000);
                    return ShowAttachmentList(playerid, privcarid), 1;
                }

                default: return ShowPlayerObjectMenu(playerid, privcarid, playerData[playerid][player_objectData][listitem - 2]);
            }            
            return 1;
        }

        case DIALOG_PRIVCAR_ATTACH_SELECT: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 10, "");

            new cache = GetPVarInt(playerid, "cache_attachment"), privcarid = GetPVarInt(playerid, "privcarid_attachment");
            switch(listitem) {
                case 0, 1: {
                    for(new i = 0; i < 3; i++) {
                        pVehEditorPos[playerid][i] = pVehObjectPos[privcarid][cache][i];
                        pVehEditorRot[playerid][i] = pVehObjectRot[privcarid][cache][i];
                    }

                    SetEditorAttachment(playerid, (listitem == 0) ? (true) : (false));
                    TogglePlayerControllable(playerid, false);
                }

                case 2: {
                    new Cache:result = mysql_query(dbconn, form("DELETE FROM es_privcars_attachments WHERE id=%d LIMIT 1;", pVehObjectID[privcarid][cache]));

                    new objectid = pVehObjectData[privcarid][cache], index = pVehObject[privcarid][cache];
                    if(IsValidDynamicObject(objectid)) DestroyDynamicObject(objectid);

                    pVehObjectCount[privcarid] --;
                    pVehObject[privcarid][cache] = -1;

                    giveMoney(playerid, 300);
                    msg(playerid, C_VIOLET, "Sprzeda³eœ(aœ) dodatek {c}%s{/c}. Zwrócono {c}300${/c}.", attachmentObjectData[index][attachment_name]);

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "SELECT * FROM es_privcars_attachments WHERE privcarid=%d LIMIT 15;", privcarid);
                    m_pquery(dbstr, "LoadPrivCarAttachments", "ddb", playerid, privcarid, true);

                    cache_delete(result);
                }
            }
            return 1;
        }
    }
    return 0;
}
