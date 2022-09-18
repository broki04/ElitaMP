new 
    pTeleportCreatorName[MAX_PLAYERS][50],
    pTeleportCreatorCMD[MAX_PLAYERS][20],
    pTeleportCreatorType[MAX_PLAYERS],
    Float:pTeleportCreatorPos[MAX_PLAYERS][4],
    bool:pTeleportCreatorSpawn[MAX_PLAYERS],
    pTeleportCreatorInterior[MAX_PLAYERS]
;

stock ResetTeleportData(playerid) 
{
    pTeleportCreatorPos[playerid] = Float:{0.0, 0.0, 0.0, 0.0};
    pTeleportCreatorSpawn[playerid] = false;
    pTeleportCreatorInterior[playerid] = 0;
    pTeleportCreatorType[playerid] = 0;

    strcat(pTeleportCreatorName[playerid], "");
    strcat(pTeleportCreatorCMD[playerid], "");
    return 1;
}

stock GetTeleportType(typeid) 
{
    new name[20];
    switch(typeid) {
        case 0: name = "Podstawowy";
        case 1: name = "Dynamiczny";
    }
    return name;
}

alias:tpadd("tpcreator")
CMD:tpadd(playerid) 
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(Iter_Free(Teleports) == -1) return dialogBox(playerid, C_RED, ""RED"Wyczerpano limit teleportów.");

    s[0] = EOS;

    s = ""WHITE"Polecenie\t"LY"Rozwi¹zanie\n";
    format(s, sizeof(s), "%s"WHITE"Nazwa teleportu:\t"LY"%s\n", s, (strlen(pTeleportCreatorName[playerid]) < 2) ? ("BRAK") : (form("%s", pTeleportCreatorName[playerid])));
    format(s, sizeof(s), "%s"WHITE"Komenda teleportu:\t"GREEN"/%s\n", s, (strlen(pTeleportCreatorCMD[playerid]) < 2) ? ("BRAK") : (form("%s", pTeleportCreatorCMD[playerid])));
    format(s, sizeof(s), "%s"WHITE"Typ teleportu:\t"BLUE2"%s\n", s, GetTeleportType(pTeleportCreatorType[playerid]));
    format(s, sizeof(s), "%s"WHITE"Koordynaty:\t"LRED"%s\n", s, (pTeleportCreatorPos[playerid][0] == 0.0) ? ("BRAK") : (form("%.2f, %.2f, %.2f, %.2f", unpackXYZR(pTeleportCreatorPos[playerid]))));
    format(s, sizeof(s), "%s"WHITE"Auto-respawn:\t"YELLOW"%s\n", s, (pTeleportCreatorSpawn[playerid]) ? ("TAK") : ("NIE"));
    strcat(s, " \n");
    strcat(s, ""RED"Stwórz teleport");
    return showDialog(playerid, DIALOG_TELEPORT_CREATOR, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Kreator teleportu", s, "Wybierz", "Zamknij");
}
flags:tpadd(HEAD)

stock OnDialogTeleportCreator(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_TELEPORT_NAME: {
            if(!response) return callcmd::tpadd(playerid);
            if(strlen(inputtext) < 3 || strlen(inputtext) > 25) return showDialog(playerid, DIALOG_TELEPORT_NAME, DIALOG_STYLE_INPUT, ""RED"Teleport - nazwa", ""WHITE"Wybierz nazwê teleportu:\n\n"RED"Dozwolona iloœæ znaków: "WHITE"3-25.", "Ustaw", "Cofnij");

            format(pTeleportCreatorName[playerid], 50, "%s", inputtext);
            return callcmd::tpadd(playerid);
        }

        case DIALOG_TELEPORT_COMMAND: {
            if(!response) return callcmd::tpadd(playerid);
            if(strlen(inputtext) < 2 || strlen(inputtext) > 10) return showDialog(playerid, DIALOG_TELEPORT_COMMAND, DIALOG_STYLE_INPUT, ""RED"Teleport - komenda", ""WHITE"Wybierz komendê do teleportu:\n\n"RED"Dozwolona iloœæ znaków: "WHITE"2-10.", "Ustaw", "Cofnij");

            format(pTeleportCreatorCMD[playerid], 20, "%s", inputtext);
            return callcmd::tpadd(playerid);
        }

        case DIALOG_TELEPORT_CREATOR: {
            if(!response) return 1;

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_TELEPORT_NAME, DIALOG_STYLE_INPUT, ""RED"Teleport - nazwa", ""WHITE"Wybierz nazwê teleportu:", "Ustaw", "Cofnij");
                case 1: showDialog(playerid, DIALOG_TELEPORT_COMMAND, DIALOG_STYLE_INPUT, ""RED"Teleport - komenda", ""WHITE"Wybierz komendê do teleportu:", "Ustaw", "Cofnij");
                
                case 2: {
                    pTeleportCreatorType[playerid] = !pTeleportCreatorType[playerid];
                    callcmd::tpadd(playerid);
                }

                case 3: {
                    new Float:x, Float:y, Float:z, Float:a;
                    getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);

                    pTeleportCreatorInterior[playerid] = GetPlayerInterior(playerid);

                    pTeleportCreatorPos[playerid][0] = x;
                    pTeleportCreatorPos[playerid][1] = y;
                    pTeleportCreatorPos[playerid][2] = z;
                    pTeleportCreatorPos[playerid][3] = a;

                    callcmd::tpadd(playerid);
                }

                case 4: {
                    pTeleportCreatorSpawn[playerid] = !pTeleportCreatorSpawn[playerid];
                    callcmd::tpadd(playerid);
                }

                case 5: callcmd::tpadd(playerid);

                case 6: {
                    if(strlen(pTeleportCreatorName[playerid]) < 2) return dialogBox(playerid, C_RED, ""RED"Nie wprowadzono nazwy teleportu.");
                    if(strlen(pTeleportCreatorCMD[playerid]) < 2) return dialogBox(playerid, C_RED, ""RED"Nie wprowadzono komendy teleportu.");
                    if(pTeleportCreatorPos[playerid][0] == 0.0) return dialogBox(playerid, C_RED, ""RED"Nie ustawiono koordynatów teleportu.");

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "INSERT INTO es_teleports (name, cmd, category, x, y, z, a, interior, spawn) VALUES ('%s', '%s', %d, %f, %f, %f, %f, %d, %d);", pTeleportCreatorName[playerid], pTeleportCreatorCMD[playerid], pTeleportCreatorType[playerid], unpackXYZR(pTeleportCreatorPos[playerid]), pTeleportCreatorInterior[playerid], pTeleportCreatorSpawn[playerid]);
                    m_tquery(dbstr, "AddTeleport", "dss", playerid, pTeleportCreatorName[playerid], pTeleportCreatorCMD[playerid]);
                }
            }
            return 1;
        }
    }
    return 0;
}

function AddTeleport(playerid, const name[], const cmd[])
{
    new teleportid = (cache_insert_id() - 1);
    Iter_Add(Teleports, teleportid);

    format(teleportData[teleportid][teleport_name], 50, "%s", pTeleportCreatorName[playerid]);
    format(teleportData[teleportid][teleport_cmd], 20, "%s", pTeleportCreatorCMD[playerid]);
    
    teleportData[teleportid][teleport_category] = pTeleportCreatorType[playerid];
    teleportData[teleportid][teleport_spawn] = pTeleportCreatorSpawn[playerid];
    teleportData[teleportid][teleport_interior] = pTeleportCreatorInterior[playerid];

    teleportData[teleportid][teleport_x] = pTeleportCreatorPos[playerid][0];
    teleportData[teleportid][teleport_y] = pTeleportCreatorPos[playerid][1];
    teleportData[teleportid][teleport_z] = pTeleportCreatorPos[playerid][2];
    teleportData[teleportid][teleport_a] = pTeleportCreatorPos[playerid][3];

    printf(" >>> [TELES]: Dodano nowy teleport %s (/%s) przez %s(%d) - x: %.2f, y: %.2f, z: %.2f, a: %.2f", pTeleportCreatorName[playerid], pTeleportCreatorCMD[playerid], getNick(playerid), playerid, unpackXYZR(pTeleportCreatorPos[playerid]));
    msg(playerid, C_VIOLET, "Dodano nowy teleport - {c}%s (/%s - ID: %d){/c}.", pTeleportCreatorName[playerid], pTeleportCreatorCMD[playerid], teleportid + 1);
    return ResetTeleportData(playerid), 1;
}