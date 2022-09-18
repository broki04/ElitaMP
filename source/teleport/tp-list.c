alias:teles("teleporty", "teleports")
CMD:teles(playerid) return showDialog(playerid, DIALOG_TELEPORT_CATEGORY, DIALOG_STYLE_LIST, ""PINK"Teleporty - wybierz kategoriê", ""WHITE"Podstawowe\n"WHITE"Dynamiczne", "Wybierz", "Zamknij");

stock OnDialogTeleport(playerid, dialogid, response, listitem, inputtext[]) 
{
    OnDialogTeleportCreator(playerid, dialogid, response, listitem, inputtext);
    OnDialogTeleportManage(playerid, dialogid, response, listitem, inputtext);

    switch(dialogid) {
        case DIALOG_TELEPORT_SELECT: {
            if(!response) return callcmd::teles(playerid);
            return EnterTeleport(playerid, teleportData[playerData[playerid][player_saveData][listitem]][teleport_cmd]);
        }

        case DIALOG_TELEPORT_CATEGORY: {
            if(!response) return 1;
            return ShowTeleports(playerid, listitem), 1;
        }
    }
    return 0;
}

stock ShowTeleports(playerid, category) 
{
    s[0] = EOS;

    new x = 0;
    foreach(new i : Teleports) {
        if(teleportData[i][teleport_category] == category) {
            AddDialogListitem(playerid, ""WHITE"»  "BLUE"%s "LBLUE"(/%s)\n", teleportData[i][teleport_name], teleportData[i][teleport_cmd]);
            playerData[playerid][player_saveData][x++] = i;
        }
    }
    return showDialog(playerid, DIALOG_TELEPORT_SELECT, DIALOG_STYLE_LIST, ""YELLOW"Wybierz teleport", s, "Teleportuj", "Cofnij", 10);
}