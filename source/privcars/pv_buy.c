#include YSI_Coding\y_hooks

function OnPlayerBuyPrivCar(playerid, modelid) return CreatePrivCar(playerid, cache_insert_id(), true, PRIVCAR_TELEPORT_PLAYER);
function CreatePrivCar(playerid, privcarid, bool:buy, type)
{
    if(privcarid == -1 || pVehSpawned[privcarid]) return 1;   

    inline onCreateCar()
    {
        if(cache_num_rows() > 0) {
            cache_get_value_name_int(0, "ownerid", pVehOwner[privcarid]);
            cache_get_value_name_int(0, "modelid", pVehModel[privcarid]);
            cache_get_value_name(0, "name", pVehEtykietaName[privcarid], 50);
            cache_get_value_name_float(0, "dist", pVehDist[privcarid]);
            cache_get_value_name_float(0, "fuel_amount", pVehFuelAmount[privcarid]);
            cache_get_value_name_int(0, "register_color", pVehRegisterColor[privcarid]);
            cache_get_value_name(0, "register_name", pVehRegisterName[privcarid]);
            cache_get_value_name_int(0, "sell", pVehSell[privcarid]);
            cache_get_value_name_int(0, "paintjob", pVehPaintJob[privcarid]);
            cache_get_value_name_int(0, "engine", pVehEngine[privcarid]);
            cache_get_value_name_bool(0, "gas", pVehGas[privcarid]);
            cache_get_value_name_bool(0, "gas_enabled", pVehGasEnabled[privcarid]);
            cache_get_value_name_int(0, "sell_cost", pVehSellCost[privcarid]);

            new buffer[128];
            cache_get_value_name(0, "fuel", buffer);
            sscanf(buffer, "p<,>a<f>[2]", pVehFuel[privcarid]);

            cache_get_value_name(0, "position", buffer);
            sscanf(buffer, "p<,>a<f>[4]", pVehPosition[privcarid]);

            cache_get_value_name(0, "etykieta", buffer);
            sscanf(buffer, "p<,>a<d>[4]", pVehEtykieta[privcarid]);

            cache_get_value_name(0, "ulepszenia", buffer);
            sscanf(buffer, form("p<,>a<b>[%d]", sizeof(ulepszeniaPojazdData)), pVehUlepszenie[privcarid]);

            cache_get_value_name(0, "color", buffer);
            sscanf(buffer, "p<,>dd", pVehColor[privcarid][0], pVehColor[privcarid][1]);

            cache_get_value_name(0, "comp", buffer);
            sscanf(buffer, "p<,>a<d>[14]", pVehComponent[privcarid]);

            if(buy) {
                new count = (pVehCount[playerid] == 0) ? (1) : (pVehCount[playerid]);
                new money = PP_COST_MONEY * count;

                giveMoney(playerid, -money);
                addLevelScore(playerid, math_random(30, 50));
                
                pVehCount[playerid] ++;
                if(pVehCount[playerid] == 2) achievement(playerid, ACH_PRIVCAR);

                for(new i = 0; i < 5; i++) {
                    if(pVehHave[playerid][i] == -1) {
                        pVehHave[playerid][i] = privcarid;
                        break;
                    }
                }

                pVehFuel[privcarid][FUEL_AMOUNT] = 30.0;
                pVehEtykieta[privcarid] = {C_GREEN, C_NICK, C_LY, C_VIOLET};
                SavePrivCar(privcarid);

                s[0] = EOS;
                format(s, sizeof(s), ""BLUE2"Zakupi³eœ(aœ) swój "WHITE"%d "BLUE2"prywatny pojazd: "LY"%s"BLUE2".\n", pVehCount[playerid], VehicleNames[pVehModel[privcarid] - 400]);
                format(s, sizeof(s), "%s"BLUE2"Wybrana instalacja paliwowa to: "WHITE"%s\n", s, (pVehEngine[privcarid] == GASOLINE_ENGINE) ? ("Benzyna") : ("Diesel"));
                format(s, sizeof(s), "%s"BLUE2"Unikalny identyfikator pojazdu to: "WHITE"#%d"BLUE2".\n\n", s, privcarid);
                format(s, sizeof(s), "%s"BLUE2"Zap³aci³eœ(aœ) za pojazd: "WHITE"%s$.\n"BLUE2"Zarz¹dzanie pojazdem pod: "WHITE"/pp", s, strdot(money));
                showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""VIOLET2"Zakup prywatnego pojazdu", s, "OK", #);
            }
            format(pVehOwnerName[privcarid], 25, "%s", getNick(playerid));

            pVeh[privcarid] = CreateVehicle(pVehModel[privcarid], unpackXYZR(pVehPosition[privcarid]), pVehColor[privcarid][0], pVehColor[privcarid][1], -1);
            SetVehicleNumberPlate(pVeh[privcarid], form("{%06x}%s", pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]));
            if(pVehPaintJob[privcarid] > 0) ChangeVehiclePaintjob(pVeh[privcarid], pVehPaintJob[privcarid]);
            addVehicleComponent(pVeh[privcarid], pVehComponent[privcarid][0], pVehComponent[privcarid][1], pVehComponent[privcarid][2], pVehComponent[privcarid][3], pVehComponent[privcarid][4], pVehComponent[privcarid][5], pVehComponent[privcarid][6], pVehComponent[privcarid][7], pVehComponent[privcarid][8], pVehComponent[privcarid][9], pVehComponent[privcarid][10], pVehComponent[privcarid][11], pVehComponent[privcarid][12], pVehComponent[privcarid][13]);

            pVehLabel[privcarid] = CreateDynamic3DTextLabel(UpdatePrivCarLabel(privcarid), -1, 0.0, 0.0, -5000.0, 50.0, .attachedvehicle=pVeh[privcarid]);
            LinkVehicleToInterior(pVeh[privcarid], GetPlayerInterior(playerid));
            SetVehicleVirtualWorld(pVeh[privcarid], GetPlayerVirtualWorld(playerid));

            dbstr[0] = EOS;
            format(dbstr, sizeof(dbstr), "SELECT * FROM es_privcars_attachments WHERE privcarid=%d LIMIT 15;", privcarid);
            m_pquery(dbstr, "LoadPrivCarAttachments", "ddb", playerid, privcarid, false);

            pVehSpawned[privcarid] = true;
            VehiclePrivate[pVeh[privcarid]] = true;
            VehiclePrivateID[pVeh[privcarid]] = privcarid;

            if(!pVehSell[privcarid]) {
                msg(playerid, C_MINT, "Stworzy³eœ(aœ) prywatny pojazd {c}%s (ID: %d){/c}. Zarz¹dzanie pod: {c}/pmanage %d{/c}.", VehicleNames[pVehModel[privcarid] - 400], privcarid, GetPrivCarSlot(playerid, privcarid));

                switch(type) {
                    case PRIVCAR_TELEPORT_VEHICLE: {
                        new Float:x, Float:y, Float:z, Float:a;
                        getPosition(playerid, false, x, y, z, a);
                        SetVehiclePosition(pVeh[privcarid], x, y, z, a);
                        PutPlayerInVehicle(playerid, pVeh[privcarid], 0);
                    }
                    case PRIVCAR_TELEPORT_PLAYER: PutPlayerInVehicle(playerid, pVeh[privcarid], 0);
                }
            } 
            else AddVehicleToGielda(playerid, privcarid);
        }
    }
    m_pquery_inline(dbconn, using inline onCreateCar, "SELECT * FROM es_privcars WHERE id=%d LIMIT 1;", privcarid);
    return 1;
}

stock DestroyPrivCar(privcarid)
{
    if(!pVehSpawned[privcarid]) return 1;

    if(IsValidVehicle(pVeh[privcarid])) DestroyVehicle(pVeh[privcarid]);
    if(IsValidDynamic3DTextLabel(pVehLabel[privcarid])) DestroyDynamic3DTextLabel(pVehLabel[privcarid]);

    pVehSpawned[privcarid] = false;
    return SavePrivCar(privcarid), 1;
}

stock SavePrivCar(privcarid)
{
    GetVehicleColor(pVeh[privcarid], pVehColor[privcarid][0], pVehColor[privcarid][1]);

    new comp[90];
    for(new i = 0; i < 14; i++) {
        pVehComponent[privcarid][i] = GetVehicleComponentInSlot(pVeh[privcarid], i);
        format(comp, sizeof(comp), "%s%s%i", comp, i == 0 ? ("") : (","), pVehComponent[privcarid][i]);
    }

    dbstr[0] = EOS;
    format(dbstr, sizeof(dbstr), "UPDATE es_privcars SET etykieta='%s', name='%s', dist=%f, fuel='%f, %f', color='%d,%d', comp='%s', paintjob=%d WHERE id=%d LIMIT 1;", 
        GetEtykietaColor(privcarid), 
        pVehEtykietaName[privcarid], 
        pVehDist[privcarid], 
        pVehFuel[privcarid][0], pVehFuel[privcarid][1], 
        pVehColor[privcarid][0], 
        pVehColor[privcarid][1], 
        comp,
        pVehPaintJob[privcarid],
        privcarid
    );
    return m_pquery(dbstr);
}

stock UpdatePrivCarLabel(privcarid)
{
    new str[256], ostr[90];
    if(pVehGas[privcarid] && pVehGasEnabled[privcarid]) format(ostr, sizeof(ostr), "{34ABEB}(Gaz, %.2f L)", pVehFuel[privcarid][GAS_AMOUNT]);
    else format(ostr, sizeof(ostr), "{%06x}(%s, %.2f L)", GetEngineColor(pVehEngine[privcarid]) >>> 8, GetEngineName(pVehEngine[privcarid]), pVehFuel[privcarid][FUEL_AMOUNT]);

    format(str, sizeof(str), "{%06x}%s\n{%06x}%s {%06x}[%s, %d]\n{%06x}%.2f km %s", 
        pVehEtykieta[privcarid][0] >>> 8, pVehEtykietaName[privcarid],
        pVehEtykieta[privcarid][1] >>> 8, pVehOwnerName[privcarid],
        pVehEtykieta[privcarid][2] >>> 8, VehicleNames[pVehModel[privcarid] - 400], privcarid,
        pVehEtykieta[privcarid][3] >>> 8, pVehDist[privcarid], ostr
    );
    return str;
}

stock GetEngineColor(engineid)
{
    new color;
    switch(engineid) {
        case GASOLINE_ENGINE:   color = 0x00AA00FF;
        case DIESEL_ENGINE:     color = 0xB87333FF;
        default:                color = 0x000000FF;
    }
    return color;
}

stock GetEngineName(engineid)
{
    new name[10];
    switch(engineid) {
        case GASOLINE_ENGINE:   name = "Benzyna";
        case DIESEL_ENGINE:     name = "Diesel";
        default:                name = "---";
    }
    return name;
}

stock ShowEngineSelect(playerid)
{
    s[0] = EOS;
    catstr(s, C_ORANGE, -1, "Wybierz poni¿ej {c}wersjê silnikow¹{/c} prywatnego pojazdu:\n");
    catstr(s, C_GREEN, -1, "{c}"ARROW"{/c}  "GREEN"Benzyna\n");
    catstr(s, C_BROWN, -1, "{c}"ARROW"{/c}  "BROWN"Diesel\n");
    strcat(s, " \n");
    catstr(s, C_LY, -1, "PóŸniejsza zmiana typu silnika {c}nie bêdzie{/c} mo¿liwa!\n");
    catstr(s, C_LY, -1, "Wybieraj¹c {c}silnik benzynowy{/c}, istnieje opcja zainstalowania {c}instalacji gazowej{/c}.");
    return showDialog(playerid, DIALOG_PRIVCAR_ENGINE, DIALOG_STYLE_LIST, ""YELLOW"Silnik", s, "Wybierz", "Cofnij");
}

stock OnDialogPrivCarBuy(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_PRIVCAR_ENGINE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_NEWBIE, 1, 1, "");

            switch(listitem) {
                case 1, 2: {
                    new Float:x, Float:y, Float:z, Float:a, modelid = playerData[playerid][player_saveData][0];
                    getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "INSERT INTO es_privcars (ownerid, modelid, position, register_color, date_created, engine) VALUES (%d, %d, '%f, %f, %f, %f', %d, '%s', %d);", playerData[playerid][player_accountID], modelid, x, y, z, a, KoloryGraczy[random(sizeof(KoloryGraczy))], getLocalDate(), (listitem == 1) ? (GASOLINE_ENGINE) : (DIESEL_ENGINE));
                    m_pquery(dbstr, "OnPlayerBuyPrivCar", "d", playerid);
                }
                default: ShowEngineSelect(playerid);
            }
            return 1;
        }

        case DIALOG_PRIVCAR_BUY: {
            if(!response) return callcmd::pp(playerid);

            if(listitem == 0 && !pDriveLicense[playerid][KAT_A]) return dialogBox(playerid, C_RED, "Nie posiadasz zdanego {c}egzaminu{/c} na {c}kat. A{/c}!");
            if(listitem > 0 && !pDriveLicense[playerid][KAT_B]) return dialogBox(playerid, C_RED, "Nie posiadasz zdanego {c}egzaminu{/c} na {c}kat. B{/c}!");

            new List:list = list_new();
            for(new i = 0; i < guiPrivCarModels[listitem][0]; i++) AddModelMenuItem(list, guiPrivCarModels[listitem][i + 1] + 400, form("%s", VehicleNames[guiPrivCarModels[listitem][i + 1]]), true, 0.0, 0.0, -50.0, 1.5);

            new _response[E_MODEL_SELECTION_INFO];
            await_arr(_response) ShowAsyncModelSelectionMenu(playerid, "PRYWATNY POJAZD", list);

            if(_response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) {
                playerData[playerid][player_saveData][0] = _response[E_MODEL_SELECTION_MODELID];
                ShowEngineSelect(playerid);
            }
            else OnDialogResponse(playerid, DIALOG_PRIVCAR_NEWBIE, 1, 1, "");
            return 1;
        }
    }
    return 0;
}

hook OnVehicleMod(playerid, vehicleid, componentid)
{
    if(VehiclePrivate[vehicleid] && VehiclePrivateID[vehicleid] != -1) {
        new privcarid = VehiclePrivateID[vehicleid];
        if(privcarid != -1 && pVehSpawned[privcarid]) SavePrivCar(privcarid);
    }
    return 1;
}

hook OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
    if(VehiclePrivate[vehicleid] && VehiclePrivateID[vehicleid] != -1) {
        new privcarid = VehiclePrivateID[vehicleid];
        if(privcarid != -1 && pVehSpawned[privcarid]) {
            pVehPaintJob[privcarid] = paintjobid;
            SavePrivCar(privcarid);
        }
    }
    return 1;
}