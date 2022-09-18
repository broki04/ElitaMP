alias:pp("pv", "privcar")
CMD:pp(playerid)
{
    s[0] = EOS;
    strcat(s, ""RED"1. \t"WHITE"Informacje o prywatnych pojazdach\n");
    format(s, sizeof(s), "%s"RED"2. \t"WHITE"Zakup prywatny pojazd "GREEN"(%d/%d)\n", s, pVehCount[playerid], (pUlepszenia[playerid][ULEPSZENIE_PRIVCAR]) ? (5) : (3));
    strcat(s, ""RED"3. \t"WHITE"Gie³da prywatnych pojazdów\n");

    if(pVehCount[playerid] > 0) {
        strcat(s, " \n");
        inline onFetchPrivCars() {
            new privcarid, modelid, Float:dist, Float:fuel, engine, bool:gas, bool:gas_enabled, x = 0;
            for(new i = 0; i < cache_num_rows(); i++) {
                cache_get_value_name_int(i, "id", privcarid);
                cache_get_value_name_int(i, "modelid", modelid);
                cache_get_value_name_float(i, "dist", dist);
                cache_get_value_name_float(i, "fuel", fuel);
                cache_get_value_name_int(i, "engine", engine);
                cache_get_value_name_bool(i, "gas", gas);
                cache_get_value_name_bool(i, "gas_enabled", gas_enabled);

                if(gas && gas_enabled) format(s, sizeof(s), "%s"WHITE"»  "GREEN"%s "LY"(#%d, %.2f km, {34ABEB}Gaz - %.2f L)\n", s, VehicleNames[modelid - 400], privcarid, dist, fuel);
                else format(s, sizeof(s), "%s"WHITE"»  "GREEN"%s "LY"(#%d, %.2f km, {%06x}%s - %.2f L)\n", s, VehicleNames[modelid - 400], privcarid, dist, GetEngineColor(engine) >>> 8, GetEngineName(engine), fuel);
                pVehFetch[playerid][x++] = privcarid;
            }
            showDialog(playerid, DIALOG_PRIVCAR_NEWBIE, DIALOG_STYLE_LIST, ""BLUE"Prywatne pojazdy", s, "Wybierz", "Zamknij");
        }
        m_pquery_inline(dbconn, using inline onFetchPrivCars, "SELECT * FROM es_privcars WHERE ownerid=%d AND sell=0 LIMIT %d;", playerData[playerid][player_accountID], pVehCount[playerid]);
    }
    else showDialog(playerid, DIALOG_PRIVCAR_NEWBIE, DIALOG_STYLE_LIST, ""BLUE"Prywatne pojazdy", s, "Wybierz", "Zamknij");
    return 1;
}

stock Task:RunPrivCarList(playerid, bool:tpHere)
{   
    new Task:t = task_new();
    SetPVarInt(playerid, "privcar_teleport_type", tpHere);

    inline onPrivCarList() {
        s[0] = EOS;

        new privcarid, modelid, Float:dist, Float:fuel, engine, bool:gas, bool:gas_enabled, x = 0;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", privcarid);
            cache_get_value_name_int(i, "modelid", modelid);
            cache_get_value_name_float(i, "dist", dist);
            cache_get_value_name_float(i, "fuel", fuel);
            cache_get_value_name_int(i, "engine", engine);
            cache_get_value_name_bool(i, "gas", gas);
            cache_get_value_name_bool(i, "gas_enabled", gas_enabled);

            if(gas && gas_enabled) format(s, sizeof(s), "%s"WHITE"»  "GREEN"%s "LY"(#%d, %.2f km, {34ABEB}Gaz - %.2f L)\n", s, VehicleNames[modelid - 400], privcarid, dist, fuel);
            else format(s, sizeof(s), "%s"WHITE"»  "GREEN"%s "LY"(#%d, %.2f km, {%06x}%s - %.2f L)\n", s, VehicleNames[modelid - 400], privcarid, dist, GetEngineColor(engine) >>> 8, GetEngineName(engine), fuel);
            pVehFetch[playerid][x++] = privcarid;
        }
        showDialog(playerid, DIALOG_PRIVCAR_TELEPORT, DIALOG_STYLE_LIST, ""BLUE"Lista prywatnych pojazdów", s, "Wybierz", "Zamknij");
    }
    m_tquery_inline(dbconn, using inline onPrivCarList, "SELECT * FROM es_privcars WHERE ownerid=%d AND sell=0 LIMIT %d;", playerData[playerid][player_accountID], pVehCount[playerid]);
    return t;
}

alias:pth("ptu")
CMD:pth(playerid, params[])
{
    if(pVehCount[playerid] == 0) return sendError(playerid, ""RED"Nie masz prywatnych pojazdów.");

    new slotid;
    if(sscanf(params, "d", slotid)) return task_await(RunPrivCarList(playerid, true));
    if(slotid < 1 || slotid > 5) return sendError(playerid, ""RED"Dostêpne sloty na serwerze to: "WHITE"1-5"RED".");
    if(pVehHave[playerid][slotid - 1] == -1) return sendError(playerid, ""RED"Nie masz ¿adnego prywatnego pojazdu w tym slocie.");
    
    new privcarid = pVehHave[playerid][slotid - 1];
    if(!pVehSpawned[privcarid]) return CreatePrivCar(playerid, privcarid, false, 2);
    if(pVehSell[privcarid]) return sendError(playerid, ""RED"Ten pojazd jest wystawiony na sprzeda¿!");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);
    SetVehiclePosition(pVeh[privcarid], x, y, z, a);
    return PutPlayerInVehicle(playerid, pVeh[privcarid], 0), 1;
}

alias:ptp("ptt")
CMD:ptp(playerid, params[])
{
    if(pVehCount[playerid] == 0) return sendError(playerid, ""RED"Nie masz prywatnych pojazdów.");

    new slotid;
    if(sscanf(params, "d", slotid)) return task_await(RunPrivCarList(playerid, false));
    if(slotid < 1 || slotid > 5) return sendError(playerid, ""RED"Dostêpne sloty na serwerze to: "WHITE"1-5"RED".");
    if(pVehHave[playerid][slotid - 1] == -1) return sendError(playerid, ""RED"Nie masz ¿adnego prywatnego pojazdu w tym slocie.");
    
    new privcarid = pVehHave[playerid][slotid - 1];
    if(!pVehSpawned[privcarid]) return CreatePrivCar(playerid, privcarid, false, 1);
    if(pVehSell[privcarid]) return sendError(playerid, ""RED"Ten pojazd jest wystawiony na sprzeda¿!");
    return PutPlayerInVehicle(playerid, pVeh[privcarid], 0), 1;
}