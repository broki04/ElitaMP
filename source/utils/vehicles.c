new serverVehicle[MAX_SERVER_VEH];
new serverVehicleCount = 0;
new Float:serverVehiclePosition[MAX_SERVER_VEH][4];

stock Init_Vehicles()
{
    inline onAddVehicles()
    {
        new id, modelid, Float:x, Float:y, Float:z, Float:a;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", id);
            cache_get_value_name_int(i, "modelid", modelid);
            cache_get_value_name_float(i, "x", x);
            cache_get_value_name_float(i, "y", y);
            cache_get_value_name_float(i, "z", z);
            cache_get_value_name_float(i, "a", a);

            serverVehiclePosition[i][0] = x;
            serverVehiclePosition[i][1] = y;
            serverVehiclePosition[i][2] = z;
            serverVehiclePosition[i][3] = a;

            serverVehicle[i] = CreateVehicle(modelid, x, y, z, a, -1, -1, -1);
            SetVehicleNumberPlate(serverVehicle[i], ""GOLD"Elita");
            AddVehicleComponent(serverVehicle[i], 1010);

            serverVehicleCount ++;
        }
        return printf(" >>> [VEHICLE]: Wczytanych pojazdów na mapie: %d", serverVehicleCount), 1;
    }

    m_tquery_inline(dbconn, using inline onAddVehicles, "SELECT * FROM es_vehicles ORDER BY id ASC LIMIT %d;", MAX_SERVER_VEH);
    return 1;
}

alias:addvehicle("addveh")
CMD:addvehicle(playerid)
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);
    if(serverVehicleCount >= MAX_SERVER_VEH) return sendError(playerid, "Limit pojazdów na mapie osi¹gniêty!");
    if(!IsPlayerInAnyVehicle(playerid)) return sendError(playerid, "Musisz byæ w jakimœ pojeŸdzie.");

    new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid), Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, true, x, y, z, a);

    inline onAddVehicle() {        
        if(IsValidVehicle(vehicleid)) DestroyVehicle(vehicleid);

        new i = cache_insert_id() - 1;
        serverVehicle[i] = CreateVehicle(modelid, x, y, z, a, -1, -1, -1);
        SetVehicleNumberPlate(serverVehicle[i], ""GOLD"Elita");
        AddVehicleComponent(serverVehicle[i], 1010);
    
        serverVehicleCount ++;
        return msg(playerid, C_CHAT, "Dodano {c}%s (ID: %d){/c} do listy pojazdów! "LY"(%d/%d)", VehicleNames[modelid - 400], modelid, serverVehicleCount, MAX_SERVER_VEH), 1;
    }
    return m_pquery_inline(dbconn, using inline onAddVehicle, "INSERT INTO es_vehicles (modelid, x, y, z, a) VALUES (%d, %f, %f, %f, %f);", modelid, x, y, z, a), 1;
}
flags:addvehicle(HEAD)