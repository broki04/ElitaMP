alias:tplist("tpmanage")
CMD:tplist(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    return showDialog(playerid, DIALOG_TELEPORT_LIST, DIALOG_STYLE_LIST, ""BLUE"Lista teleportów", ""WHITE"Podstawowe\n"WHITE"Dynamiczne", "Wybierz", "Zamknij");
}
flags:tplist(HEAD)

stock OnDialogTeleportManage(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_TELEPORT_EDIT_NAME: {
            if(!response) return ShowTeleportManage(playerid, playerData[playerid][player_teleportData][0]);

            new dest[90];
            mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 3 || strlen(dest) > 20) return showDialog(playerid, DIALOG_TELEPORT_EDIT_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ nazwê teleportu", ""WHITE"Wpisz poni¿ej now¹ nazwê dla teleportu:\n\n"RED"Limit znaków: "WHITE"3-20.", "Ustaw", "Cofnij");

            new i = playerData[playerid][player_teleportData][0];
            format(teleportData[i][teleport_name], 50, "%s", dest);
            
            m_pquery_format("UPDATE es_teleports SET name='%s' WHERE id=%d LIMIT 1;", dest, i + 1);
            msg(playerid, C_BLUE, "Zmieniono {c}nazwê{/c} teleportu na: {c}%s{/c}.", dest);
            return ShowTeleportManage(playerid, i), 1;        
        }

        case DIALOG_TELEPORT_EDIT_CMD: {
            if(!response) return ShowTeleportManage(playerid, playerData[playerid][player_teleportData][0]);

            new dest[90];
            mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 2 || strlen(dest) > 10) return showDialog(playerid, DIALOG_TELEPORT_EDIT_CMD, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ komendê teleportu", ""WHITE"Wpisz poni¿ej now¹ komendê dla teleportu:\n\n"RED"Limit znaków: "WHITE"2-10.", "Ustaw", "Cofnij");

            new i = playerData[playerid][player_teleportData][0];
            format(teleportData[i][teleport_cmd], 20, "%s", dest);
            
            m_pquery_format("UPDATE es_teleports SET cmd='%s' WHERE id=%d LIMIT 1;", dest, i + 1);
            msg(playerid, C_BLUE, "Zmieniono {c}komendê{/c} teleportu na: {c}%s{/c}.", dest);
            return ShowTeleportManage(playerid, i), 1;
        }

        case DIALOG_TELEPORT_MANAGE: {
            if(!response) return callcmd::tplist(playerid);
            return ShowTeleportManage(playerid, playerData[playerid][player_teleportData][listitem]);
        }

        case DIALOG_TELEPORT_LIST: {
            if(!response) return 1;

            s[0] = EOS;
            s = ""WHITE"Nazwa "LY"(komenda)\t"BLUE"Typ teleportu\n";
            playerData[playerid][player_saveData][0] = listitem;
        
            new x = 0;
            foreach(new i : Teleports) {
                if(teleportData[i][teleport_category] == listitem) {
                    AddDialogListitem(playerid, ""WHITE"%s "LY"(/%s)\t"BLUE"%s\n", teleportData[i][teleport_name], teleportData[i][teleport_cmd], GetTeleportType(teleportData[i][teleport_category]));
                    playerData[playerid][player_teleportData][x++] = i;
                }
            }
            return showDialog(playerid, DIALOG_TELEPORT_MANAGE, DIALOG_STYLE_TABLIST_HEADERS, ""VIOLET"Zarz¹dzanie teleportami", s, "Wybierz", "Cofnij", 10), 1;
        }

        case DIALOG_TELEPORT_CHANGE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_TELEPORT_LIST, 1, playerData[playerid][player_saveData][0], "");

            new i = playerData[playerid][player_teleportData][0];
            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_TELEPORT_EDIT_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ nazwê teleportu", ""WHITE"Wpisz poni¿ej now¹ nazwê dla teleportu:", "Ustaw", "Cofnij");
                case 1: showDialog(playerid, DIALOG_TELEPORT_EDIT_CMD, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ komendê teleportu", ""WHITE"Wpisz poni¿ej now¹ komendê dla teleportu:", "Ustaw", "Cofnij");

                case 2: {
                    new Float:x, Float:y, Float:z, Float:a;
                    getPosition(playerid, false, x, y, z, a);

                    teleportData[i][teleport_x] = x;
                    teleportData[i][teleport_y] = y;
                    teleportData[i][teleport_z] = z;
                    teleportData[i][teleport_a] = a;
                    teleportData[i][teleport_interior] = GetPlayerInterior(playerid);

                    m_pquery_format("UPDATE es_teleports SET x=%f, y=%f, z=%f, a=%f, interior=%d WHERE id=%d LIMIT 1;", x, y, z, a, teleportData[i][teleport_interior], i + 1);
                    ShowTeleportManage(playerid, i);
                }

                case 3: {
                    teleportData[i][teleport_category] = !teleportData[i][teleport_category];
                    m_pquery_format("UPDATE es_teleports SET category=%d WHERE id=%d LIMIT 1;", teleportData[i][teleport_category], i + 1);
                    ShowTeleportManage(playerid, i);
                }

                case 4: {
                    teleportData[i][teleport_spawn] = !teleportData[i][teleport_spawn];
                    m_pquery_format("UPDATE es_teleports SET spawn=%d WHERE id=%d LIMIT 1;", teleportData[i][teleport_spawn], i + 1);
                    ShowTeleportManage(playerid, i);
                }
            }
            return 1;
        }
    }
    return 0;
}

stock ShowTeleportManage(playerid, i)
{
    s[0] = EOS;
    s = ""WHITE"Polecenie\t"LRED"Rezultat\n";
    format(s, sizeof(s), "%s"WHITE"Nazwa teleportu:\t"LRED"%s\n", s, teleportData[i][teleport_name]);
    format(s, sizeof(s), "%s"WHITE"Komenda teleportu:\t"LBLUE"/%s\n", s, teleportData[i][teleport_cmd]);
    format(s, sizeof(s), "%s"WHITE"Koordynaty teleportu:\t"YELLOW"%.2f, %.2f, %.2f, %.2f\n", s, teleportData[i][teleport_x], teleportData[i][teleport_y], teleportData[i][teleport_z], teleportData[i][teleport_a]);
    format(s, sizeof(s), "%s"WHITE"Typ teleportu:\t"BLUE2"%s\n", s, GetTeleportType(teleportData[i][teleport_category]));
    format(s, sizeof(s), "%s"WHITE"Auto-respawn:\t%s\n", s, (teleportData[i][teleport_spawn]) ? (""GREEN"AKTYWNY") : (""RED"NIEAKTYWNY"));

    playerData[playerid][player_teleportData][0] = i;
    return showDialog(playerid, DIALOG_TELEPORT_CHANGE, DIALOG_STYLE_TABLIST_HEADERS, form(""RED"Teleport: "WHITE"%s (/%s)", teleportData[i][teleport_name], teleportData[i][teleport_cmd]), s, "Wybierz", "Cofnij"), 1;
}