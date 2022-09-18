#include YSI_Coding\y_hooks

#define MAX_RACE_CP     30

new 
    pRaceCreatorName[MAX_PLAYERS][50], 
    Float:pRaceCreatorCP[MAX_PLAYERS][MAX_RACE_CP][3],
    pRaceCreatorModel[MAX_PLAYERS] = -1,
    Float:pRaceCreatorSpawn[MAX_PLAYERS][4],
    bool:pRaceCreatorPoints[MAX_PLAYERS],
    pRaceCreatorCPs[MAX_PLAYERS]
;

stock ResetRaceCreatorData(playerid)
{
    strcat(pRaceCreatorName[playerid], "");
    pRaceCreatorSpawn[playerid] = Float:{0.0, 0.0, 0.0, 0.0};
    pRaceCreatorModel[playerid] = -1;
    pRaceCreatorCPs[playerid] = 0;
    pRaceCreatorPoints[playerid] = false;
    for(new i = 0; i < MAX_RACE_CP; i++) pRaceCreatorCP[playerid][i] = Float:{0.0, 0.0, 0.0};
    return 1;
}

alias:rccreator("rcadd")
CMD:rccreator(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"BLUE"Wynik\n";
    catstr(s, C_WHITE, -1, "Nazwa areny:\t"BLUE"%s\n", (strlen(pRaceCreatorName[playerid]) < 2) ? (""RED"---") : (pRaceCreatorName[playerid]));
    catstr(s, C_WHITE, -1, "Model pojazdu:\t"LY"%s\n", (pRaceCreatorModel[playerid] == -1) ? (""RED"---") : (VehicleNames[pRaceCreatorModel[playerid] - 400]));
    catstr(s, C_WHITE, -1, "Pozycja spawnu:\t"GREEN"%s\n", (pRaceCreatorSpawn[playerid][0] == 0.0) ? (""RED"(NIE)") : (""GREEN"(TAK)"));
    catstr(s, C_WHITE, -1, "Kreator punktów kontrolnych:\t"LY"%d/%d\n", pRaceCreatorCPs[playerid], MAX_RACE_CP);
    return showDialog(playerid, DIALOG_RACE_CREATOR, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Kreator wyœcigu", s, "Wybierz", "Zamknij"), 1;
}
flags:rccreator(HEAD)

stock OnDialogRaceCreator(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_RACE_CREATOR: {
            if(!response) return 1;

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_RACE_CREATOR_NAME, DIALOG_STYLE_INPUT, ""BLUE"Kreator wyœcigu - nazwa", ""WHITE"Wpisz poni¿ej nazwê areny:", "Ustaw", "Cofnij");
                case 1: showDialog(playerid, DIALOG_RACE_CREATOR_VEHICLE, DIALOG_STYLE_LIST, ""BLUE"Kreator wyœcigów - pojazd", ""WHITE"Samoloty\n"WHITE"Helikoptery\n"WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowriders\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"£odzie\n"WHITE"Unikalne", "Wybierz", "Zamknij");

                case 2: {
                    new Float:x, Float:y, Float:z, Float:a;
                    getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);

                    pRaceCreatorSpawn[playerid][0] = x;
                    pRaceCreatorSpawn[playerid][1] = y;
                    pRaceCreatorSpawn[playerid][2] = z;
                    pRaceCreatorSpawn[playerid][3] = a;

                    callcmd::rccreator(playerid);
                }

                case 3: {
                    if(!pRaceCreatorPoints[playerid]) {
                        pRaceCreatorPoints[playerid] = true;
                        pRaceCreatorCPs[playerid] = 0;

                        showGameDraw(playerid, form("~y~CP: ~r~%d/%d~n~~y~ABY DODAC, KLIKNIJ ~r~Y~n~~y~ABY ZAKONCZYC, KLIKNIJ ~r~N", pRaceCreatorCPs[playerid], MAX_RACE_CP));
                    }
                    else msg(playerid, C_RED, "Opcje tworzenia {c}punktów kontrolnych{/c} wy³¹czysz za pomoc¹: {c}klawisza N{/c}.");
                }
            }
            return 1;
        }

        case DIALOG_RACE_CREATOR_NAME: {
            if(!response) return callcmd::rccreator(playerid);
            if(strlen(inputtext) < 5 || strlen(inputtext) > 15) return showDialog(playerid, DIALOG_RACE_CREATOR_NAME, DIALOG_STYLE_INPUT, ""BLUE"Kreator wyœcigu - nazwa", ""WHITE"Wpisz poni¿ej nazwê areny:\n\n"RED"Dozwolona d³ugoœæ nazwy to "WHITE"5-15"RED" znaków.", "Ustaw", "Cofnij");
            
            format(pRaceCreatorName[playerid], 50, "%s", inputtext);
            return callcmd::rccreator(playerid), 1;
        }

        case DIALOG_RACE_CREATOR_VEHICLE: {
            if(!response) return callcmd::rccreator(playerid);

            new List:list = list_new();
            for(new i = 0; i < guiVehicleModels[listitem][0]; i++) AddModelMenuItem(list, guiVehicleModels[listitem][i + 1] + 400, form("%s", VehicleNames[guiVehicleModels[listitem][i + 1]]), true, 0.0, 0.0, -50.0, 1.5);

            new _response[E_MODEL_SELECTION_INFO];
            await_arr(_response) ShowAsyncModelSelectionMenu(playerid, "WYBIERZ POJAZD", list);

            if(_response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) {
                pRaceCreatorModel[playerid] = _response[E_MODEL_SELECTION_MODELID];
                callcmd::rccreator(playerid);
            }
            else showDialog(playerid, DIALOG_RACE_CREATOR_VEHICLE, DIALOG_STYLE_LIST, ""BLUE"Kreator wyœcigów - pojazd", ""WHITE"Samoloty\n"WHITE"Helikoptery\n"WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowriders\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"£odzie\n"WHITE"Unikalne", "Wybierz", "Zamknij");
            return 1;
        }
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(pRaceCreatorPoints[playerid]) {
        if(PRESSED(KEY_YES) && IsPlayerInAnyVehicle(playerid)) {
            new cpid = pRaceCreatorCPs[playerid], Float:x, Float:y, Float:z;
            GetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
           
            pRaceCreatorCP[playerid][cpid][0] = x;
            pRaceCreatorCP[playerid][cpid][1] = y;
            pRaceCreatorCP[playerid][cpid][2] = z;

            pRaceCreatorCPs[playerid] ++;
            showGameDraw(playerid, form("~y~CP: ~r~%d/%d~n~~y~ABY DODAC, KLIKNIJ ~r~Y~n~~y~ABY ZAKONCZYC, KLIKNIJ ~r~N", pRaceCreatorCPs[playerid], MAX_RACE_CP));
        }

        if(PRESSED(KEY_NO) && IsPlayerInAnyVehicle(playerid)) {            
            if(pRaceCreatorCPs[playerid] < 5) return sendError(playerid, ""RED"Musisz mieæ stworzonych przynajmniej "WHITE"5 "RED"punktów kontrolnych.");

            dbstr[0] = EOS;
            format(dbstr, sizeof(dbstr), "INSERT INTO es_fun_rc (name, modelid, position, checkpoints) VALUES ('%s', %d, '%.2f, %.2f, %.2f, %.2f', %d);", pRaceCreatorName[playerid], pRaceCreatorModel[playerid], unpackXYZR(pRaceCreatorSpawn[playerid]), pRaceCreatorCPs[playerid]);
            m_tquery(dbstr, "AddRaceArena", "dsdd", playerid, pRaceCreatorName[playerid], pRaceCreatorModel[playerid], pRaceCreatorCPs[playerid]);
        }
    }
    return 1;
}

function AddRaceArena(playerid, const name[], modelid, cps)
{
    new arenaid = cache_insert_id();
    for(new i = 0; i < cps; i++) { if(pRaceCreatorCP[playerid][i][0] != 0.0) m_pquery_format("INSERT INTO es_fun_rc_cp (arenaid, x, y, z) VALUES (%d, %.2f, %.2f, %.f);", arenaid, unpackXYZ(pRaceCreatorCP[playerid][i])); }

    msg(playerid, C_LBLUE, ""CHAT"[RC]: "LBLUE"Arena {c}%s{/c} zosta³a pomyœlnie dodana! "LY"(%s, %d CPs)", name, GetVehicleName(modelid), cps);
    return ResetRaceCreatorData(playerid), 1;
}