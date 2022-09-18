public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(checkIP(inputtext) || SQLInjection(inputtext)) return 0;
    playSound(playerid, 30802 - response);

    OnBusinessResponse(playerid, dialogid, response, listitem, inputtext);
    OnBankDialog(playerid, dialogid, response, listitem, inputtext);
    OnFishDialog(playerid, dialogid, response, listitem);
    OnDialogAccount(playerid, dialogid, response, inputtext);
    OnDialogAdminDuty(playerid, dialogid, response, listitem, inputtext);
    OnDialogTeleport(playerid, dialogid, response, listitem, inputtext);
    OnDialogGangs(playerid, dialogid, response, listitem, inputtext);
    OnDialogHud(playerid, dialogid, response, listitem);
    OnDialogSettings(playerid, dialogid, response, listitem, inputtext);
    OnDialogOwnRank(playerid, dialogid, response, listitem, inputtext);
    OnDialogHelp(playerid, dialogid, response, listitem);
    OnDialogBanList(playerid, dialogid, response, listitem, inputtext);
    OnDialogQuest(playerid, dialogid, response, listitem);
    OnDialogPrivCars(playerid, dialogid, response, listitem, inputtext);
    OnDialogAdminPanel(playerid, dialogid, response, listitem, inputtext);
    OnDialogJob(playerid, dialogid, response, listitem);
    OnDialogHeist(playerid, dialogid, response, inputtext);
    OnDialogUlepszenia(playerid, dialogid, response, listitem);
    OnDialogRaceCreator(playerid, dialogid, response, listitem, inputtext);
    OnDialogRaport(playerid, dialogid, response, listitem, inputtext);

    switch(dialogid) {
        case DIALOG_UNUSED: return 1;

        case DIALOG_LAKIER_COLOR_A: {
            if(!response) return 1;

            if(listitem != INVALID_LISTITEM) {
                SetPVarInt(playerid, "lakier_color_one", listitem);

                s[0] = EOS;
                for(new i = 0; i < sizeof(VehicleColoursTableRGBA); i++) format(s, sizeof(s), "%s{%06x}#%d\n", s, VehicleColoursTableRGBA[i] >>> 8, i + 1);
                return showDialog(playerid, DIALOG_LAKIER_COLOR_B, DIALOG_STYLE_LIST, ""YELLOW"Wybierz drugi kolor", s, "Dalej", "Zamknij", 10);
            }
            return 1;
        }

        case DIALOG_LAKIER_COLOR_B: {
            if(!response) {
                s[0] = EOS;
                for(new i = 0; i < sizeof(VehicleColoursTableRGBA); i++) format(s, sizeof(s), "%s{%06x}#%d\n", s, VehicleColoursTableRGBA[i] >>> 8, i + 1);
                return showDialog(playerid, DIALOG_LAKIER_COLOR_A, DIALOG_STYLE_LIST, ""YELLOW"Wybierz pierwszy kolor", s, "Dalej", "Zamknij", 10);
            }
            return callcmd::lakier(playerid, form("%d %d", GetPVarInt(playerid, "lakier_color_one"), listitem));
        }

        case DIALOG_KOLORY: {
            if(!response) return 1;
            if(listitem != INVALID_LISTITEM) SetPlayerColor(playerid, KoloryGraczy[listitem]);
            return 1;
        }

        case DIALOG_INTERIORY: {
            if(!response) return 1;

            if(listitem != INVALID_LISTITEM) {
                msg(playerid, C_MINT, "Teleportowa³eœ(aœ) siê do interioru: {c}%s (interiorid: %d){/c}.", interiorDataList[listitem][int_name], interiorDataList[listitem][int_interior]);
                Teleport(playerid, false, unpackXYZ(interiorDataList[listitem][int_pos]), 0.0, interiorDataList[listitem][int_interior], 0, true);
            }
            return 1;
        }

        case DIALOG_POJAZDY: {
            if(!response) return 1;

            new List:list = list_new();
            for(new i = 0; i < guiVehicleModels[listitem][0]; i++) AddModelMenuItem(list, guiVehicleModels[listitem][i + 1] + 400, form("%s", VehicleNames[guiVehicleModels[listitem][i + 1]]), true, 0.0, 0.0, -50.0, 1.5);

            new _response[E_MODEL_SELECTION_INFO];
            await_arr(_response) ShowAsyncModelSelectionMenu(playerid, "WYBIERZ POJAZD", list);

            if(_response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) callcmd::v(playerid, form("%s", VehicleNames[_response[E_MODEL_SELECTION_MODELID] - 400]));
            else showDialog(playerid, DIALOG_POJAZDY, DIALOG_STYLE_LIST, ""BLUE"Kategoria pojazdów", ""WHITE"Samoloty\n"WHITE"Helikoptery\n"WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowriders\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"£odzie\n"WHITE"Unikalne", "Wybierz", "Zamknij");
            return 1;
        }
    }
    return 0;
}
