enum e_teleportData {
    teleport_name[50],
    teleport_cmd[20],
    teleport_category,
    Float:teleport_x, 
    Float:teleport_y,
    Float:teleport_z, 
    Float:teleport_a,
    teleport_interior,
    bool:teleport_spawn
};

new teleportData[MAX_TELEPORTS][e_teleportData], Iterator:Teleports<MAX_TELEPORTS>;

stock Init_Teleports()
{
    Iter_Init(Teleports);

    inline onLoadTeleports()
    {
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name(i, "name", teleportData[i][teleport_name]);
            cache_get_value_name(i, "cmd", teleportData[i][teleport_cmd]);
            cache_get_value_name_int(i, "category", teleportData[i][teleport_category]);
            cache_get_value_name_float(i, "x", teleportData[i][teleport_x]);
            cache_get_value_name_float(i, "y", teleportData[i][teleport_y]);
            cache_get_value_name_float(i, "z", teleportData[i][teleport_z]);
            cache_get_value_name_float(i, "a", teleportData[i][teleport_a]);
            cache_get_value_name_int(i, "interior", teleportData[i][teleport_interior]);
            cache_get_value_name_bool(i, "spawn", teleportData[i][teleport_spawn]);

            if(teleportData[i][teleport_spawn]) {
                CreateDynamicPickup(1247, 1, teleportData[i][teleport_x], teleportData[i][teleport_y], teleportData[i][teleport_z], .worldid=0, .interiorid=0);
                CreateDynamic3DTextLabel(form(""NAME" "WHITE"- "LY"%s "LRED"(/%s)\n\n"GREEN"Pamiêtaj, aby zaprosiæ swoich znajomych na serwer!\n"RED"Zachêcamy do grania, jest sporo atrakcji!", teleportData[i][teleport_name], teleportData[i][teleport_cmd]), -1, teleportData[i][teleport_x], teleportData[i][teleport_y], teleportData[i][teleport_z], 45.0);
            }
            Iter_Add(Teleports, i);
        }

        if(!Iter_Count(Teleports)) return Init_Teleports();
        printf(" >>> [TELES]: Wczytanych teleportów: %d", Iter_Count(Teleports));
    }

    m_pquery_inline(dbconn, using inline onLoadTeleports, "SELECT * FROM es_teleports ORDER BY id ASC;");
    return 1;
}

stock CheckPlayerTeleport(const cmd[]) 
{
    new id = -1;
    foreach(new i : Teleports) {
        if(strmatch(cmd, teleportData[i][teleport_cmd], true)) {
            id = i;
            break;
        }
    }
    return id;
}

stock RandomSpawn(playerid) 
{
    new i = Iter_Random(Teleports);
    if(!teleportData[i][teleport_spawn]) return RandomSpawn(playerid);
    return Teleport(playerid, false, teleportData[i][teleport_x], teleportData[i][teleport_y], teleportData[i][teleport_z], teleportData[i][teleport_a], teleportData[i][teleport_interior], 0), 1;
}

stock EnterTeleport(playerid, const cmd[])
{
    new i = CheckPlayerTeleport(cmd);
    if(i == -1) return 1;
    return Teleport(playerid, false, teleportData[i][teleport_x], teleportData[i][teleport_y], teleportData[i][teleport_z], teleportData[i][teleport_a], teleportData[i][teleport_interior], 0), 1;
}
