stock Task:RunPrivCarManage(playerid, privcarid)
{
    new Task:t = task_new();
    if(privcarid == -1) return t;

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Stan pojazdu:\t%s\n", (pVehSpawned[privcarid]) ? (""GREEN"STWORZONY") : (""RED"NIE STWORZONY"));
    
    if(pVehSpawned[privcarid]) {
        new Float:x, Float:y, Float:z; GetVehiclePos(pVeh[privcarid], x, y, z);

        strcat(s, " \n");
        format(s, sizeof(s), "%s"WHITE"Przywo³aj pojazd "RED"(/pth):\t"LBLUE"%s\n", s, getZoneName(x, y, z));
        format(s, sizeof(s), "%s"WHITE"Teleportuj siê do pojazdu "RED"(/ptp):\t"LBLUE"%s\n", s, getZoneName(x, y, z));
        strcat(s, " \n");
        format(s, sizeof(s), "%s"WHITE"Model pojazdu:\t"CORAL"%s [modelid: %d]\n", s, GetVehicleName(pVehModel[privcarid]), pVehModel[privcarid]);
        format(s, sizeof(s), "%s"WHITE"Etykieta pojazdu:\t{%06x}%s\n", s, pVehEtykieta[privcarid][0] >>> 8, pVehEtykietaName[privcarid]);
        format(s, sizeof(s), "%s"WHITE"Klucze od pojazdu:\t"LY"%d %s\n", s, GetKeyCount(privcarid), ConvertTimeFormat(GetKeyCount(privcarid), "osób", "osoba", "osoby", "osób"));
        format(s, sizeof(s), "%s"WHITE"Ulepszenia pojazdu:\t"LY"%d/%d\n", s, CountPojazdUlepszenia(privcarid), MAX_PRIVCAR_ULEPSZENIA);
        format(s, sizeof(s), "%s"WHITE"Rejestracja pojazdu:\t{%06x}%s\n", s, pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]);
        format(s, sizeof(s), "%s"WHITE"Przyczepiane obiekty:\t"RED"%d %s\n", s, pVehObjectCount[privcarid], ConvertTimeFormat(pVehObjectCount[privcarid], "obiektów", "obiekt", "obiekty", "obiektów"));
        strcat(s, " \n");
        format(s, sizeof(s), "%s"WHITE"Pojemnoœæ baku paliwowego:\t"LY"%.2f L\n", s, pVehFuelAmount[privcarid]);

        if(pVehGas[privcarid]) format(s, sizeof(s), "%s"WHITE"Prze³¹cz instalacjê paliwow¹\t%s\n", s, (pVehGasEnabled[privcarid]) ? ("{34ABEB}Gaz") : ("{00AA00}Benzyna"));
        else format(s, sizeof(s), "%s"WHITE"Dodatkowa instalacja gazowa\t"RED"30.000 pkt. respektu + 1.000.000$\n", s);
    }

    pVehManage[playerid] = privcarid;
    return showDialog(playerid, DIALOG_PRIVCAR_MANAGE, DIALOG_STYLE_TABLIST, ""BLUE"Zarz¹dzanie prywatnym pojazdem", s, "Wybierz", "Zamknij"), t;
}

stock OnDialogPrivCarManage(playerid, dialogid, response, listitem, inputtext[])
{
    OnPrivCarAttachmentDialog(playerid, dialogid, response, listitem);
    
    switch(dialogid) {
        case DIALOG_PRIVCAR_MANAGE: {
            if(!response) return 1;

            new privcarid = pVehManage[playerid];
            switch(listitem) {
                case 1, 4, 11: task_await(RunPrivCarManage(playerid, privcarid)); 
                
                case 0: {
                    if(!pVehSpawned[privcarid]) CreatePrivCar(playerid, privcarid, false, 0);
                    else DestroyPrivCar(privcarid);
                }

                case 2, 3: {
                    new slotid = GetPrivCarSlot(playerid, privcarid);
                    if(slotid == -1) return 1;

                    if(listitem == 2) callcmd::pth(playerid, form("%d", slotid));
                    else callcmd::ptp(playerid, form("%d", slotid));
                }

                case 5: showDialog(playerid, DIALOG_PRIVCAR_MODEL, DIALOG_STYLE_LIST, ""BLUE"Zmiana modelu pojazdu", ""WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowridery\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"Unikalne\n", "Dalej", "Cofnij");

                case 6: {
                    s[0] = EOS;
                    strcat(s, ""WHITE"Edytuj nag³ówek pojazdu "LY"(3.000$)\n");
                    strcat(s, " \n");
                    format(s, sizeof(s), "%s"WHITE"Edytuj: {%06x}%s\n", s, pVehEtykieta[privcarid][0] >>> 8, pVehEtykietaName[privcarid]);
                    format(s, sizeof(s), "%s"WHITE"Edytuj: {%06x}nick w³aœciciela\n", s, pVehEtykieta[privcarid][1] >>> 8);
                    format(s, sizeof(s), "%s"WHITE"Edytuj: {%06x}informacje o w³aœcicielu\n", s, pVehEtykieta[privcarid][2] >>> 8);
                    format(s, sizeof(s), "%s"WHITE"Edytuj: {%06x}przebieg pojazdu\n", s, pVehEtykieta[privcarid][3] >>> 8);
                    showDialog(playerid, DIALOG_PRIVCAR_ETYKIETA, DIALOG_STYLE_LIST, ""BLUE"Etykieta pojazdu", s, "Wybierz", "Cofnij");
                }

                case 7: {
                    s[0] = EOS;
                    strcat(s, ""BROWN"Daj komuœ kluczyki\n");
                    
                    if(GetKeyCount(privcarid) > 0) {
                        strcat(s, " \n");

                        new x = 0;
                        for(new i = 0; i < MAX_PRIVCAR_KEYS; i++) {
                            if(pVehKeys[privcarid][i] != -1) {
                                new userid = pVehKeys[privcarid][i];
                                format(s, sizeof(s), "%s"ORANGE"»  "WHITE"%s(%d)\n", s, getNick(userid), userid);
                                pVehKeyData[playerid][x++] = userid;
                            }
                        }
                    }                    
                    showDialog(playerid, DIALOG_PRIVCAR_KEYS, DIALOG_STYLE_LIST, ""BLUE2"Klucze od pojazdu", s, "Wybierz", "Cofnij");
                }

                case 8: ShowPrivCarUlepszenia(playerid, privcarid);
                case 9: {
                    s[0] = EOS;
                    format(s, sizeof(s), ""WHITE"Zmieñ nazwê rejestracji: {%06x}%s\n", pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]);
                    format(s, sizeof(s), "%s"WHITE"Zmieñ kolor rejestracji: {%06x}%s\n", s, pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]);
                    showDialog(playerid, DIALOG_PRIVCAR_PLATE, DIALOG_STYLE_LIST, ""ORANGE"Rejestracja pojazdu", s, "Wybierz", "Cofnij");
                }

                case 10: ShowPrivCarObjects(playerid, privcarid);
                case 12: {
                    s[0] = EOS;
                    s = ""WHITE"Pojemnoœæ baku\t"ORANGE"Kwota\n";
                    for(new i = 0; i < sizeof(bakData); i++) format(s, sizeof(s), "%s{%06x}%.2f L\t"ORANGE"%s$\n", s, (pVehFuelAmount[privcarid] == bakData[i][bak_amount]) ? (C_GREEN >>> 8) : (C_RED >>> 8), bakData[i][bak_amount], strdot(bakData[i][bak_money]));

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ii

                        if(!rr) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));
                        if(getMoney(playerid) < bakData[ll][bak_money]) return valueError(playerid, bakData[ll][bak_money]);
                        if(pVehFuelAmount[privcarid] >= bakData[ll][bak_amount]) return sendError(playerid, ""RED"Posiadasz ju¿ {c}tak¹ sam¹/wiêksz¹{/c} pojemnoœæ baku na paliwo.");
                        giveMoney(playerid, -bakData[ll][bak_money]);

                        pVehFuelAmount[privcarid] = bakData[ll][bak_amount];
                        m_pquery_format("UPDATE es_privcars SET fuel_amount=%.1f WHERE id=%d LIMIT 1;", pVehFuelAmount[privcarid], privcarid);
                        task_await(RunPrivCarManage(playerid, pVehManage[playerid]));
                        return msg(playerid, C_LGREEN, "Zakupi³eœ(aœ) bak o pojemnoœci {c}%.1f L{/c} za: {c}%s${/c}.", bakData[ll][bak_amount], strdot(bakData[ll][bak_money])), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Pojemnoœæ baku paliwowego", s, "Zakup", "Cofnij");
                } 
                
                case 13: {
                    if(!pVehGas[privcarid]) {
                        if(pVehEngine[privcarid] == DIESEL_ENGINE) return dialogBox(playerid, C_RED, "Nie mo¿esz zamontowaæ {c}instalacji gazowej{/c}, bo masz {c}silnik diesla{/c}!");
                        if(getMoney(playerid) < 1000000) return valueError(playerid, 1000000);
                        giveMoney(playerid, -1000000);

                        pVehGas[privcarid] = true;
                        pVehGasEnabled[privcarid] = false;
                        m_pquery_format("UPDATE es_privcars SET gas=1, gas_enabled=0, fuel='%f, %f' WHERE id=%d LIMIT 1;", pVehFuel[privcarid][0], pVehFuel[privcarid][1], privcarid);

                        msg(playerid, -1, " ");
                        msg(playerid, C_BROWN, "Zakupi³eœ(aœ) {c}instalacjê gazow¹{/c} do pojazdu za: {c}%s${/c}.", strdot(1000000));
                        msg(playerid, C_BROWN, "Opcja prze³¹czania siê miêdzy {c}instalacjami{/c} jest dostêpna w {c}warsztatach{/c} samochodowych!");
                    } else {
                        if(pVehFuel[privcarid][GAS_AMOUNT] <= 3.0) return dialogBox(playerid, C_RED, "Zatankuj prywatny pojazd {c}gazem{/c}, by móc siê prze³¹czaæ!");
                        pVehGasEnabled[privcarid] = !pVehGasEnabled[privcarid];
                    }
                    task_await(RunPrivCarManage(playerid, pVehManage[playerid]));
                }
            }
            return 1;
        }

        case DIALOG_PRIVCAR_MODEL: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));

            new List:list = list_new();
            for(new i = 0; i < guiPrivCarModels[listitem][0]; i++) AddModelMenuItem(list, guiPrivCarModels[listitem][i + 1] + 400, form("%s", VehicleNames[guiPrivCarModels[listitem][i + 1]]), true, 0.0, 0.0, -50.0, 1.5);

            new _response[E_MODEL_SELECTION_INFO];
            await_arr(_response) ShowAsyncModelSelectionMenu(playerid, "WYBIERZ MODEL POJAZDU", list);

            if(_response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) {
                dbstr[0] = EOS;
                format(dbstr, sizeof(dbstr), "UPDATE es_privcars SET modelid=%d WHERE id=%d LIMIT 1;", _response[E_MODEL_SELECTION_MODELID], pVehManage[playerid]);
                m_pquery(dbstr, "ChangePrivCarModel", "ddd", playerid, pVehManage[playerid], _response[E_MODEL_SELECTION_MODELID]);
            }
            else OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 5, "");
            return 1;
        }

        case DIALOG_PRIVCAR_PLATE: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_PRIVCAR_PLATE_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana napisu na rejestracji", ""WHITE"Wpisz poni¿ej napis, który bêdzie widnieæ na rejestracji:", "Ustaw", "Cofnij");
                case 1: {
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(KoloryGraczy); i++) format(s, sizeof(s), "%s{%06x}#%d\n", s, KoloryGraczy[i] >>> 8, i + 1);
                    showDialog(playerid, DIALOG_PRIVCAR_PLATE_COLOR, DIALOG_STYLE_LIST, ""VIOLET2"Wybierz kolor rejestracji", s, "Ustaw", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_PRIVCAR_PLATE_NAME: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 9, "");

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 3, 10)) return showDialog(playerid, DIALOG_PRIVCAR_PLATE_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana napisu na rejestracji", ""WHITE"Wpisz poni¿ej napis, który bêdzie widnieæ na rejestracji:\n\n"RED"Dozwolona d³ugoœæ rejestracji to: "WHITE"3-10 "RED"znaków.", "Ustaw", "Cofnij");
            
            new privcarid = pVehManage[playerid];
            format(pVehRegisterName[privcarid], 50, "%s", dest);

            SetVehicleNumberPlate(pVeh[privcarid], form("{%06x}%s", pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]));
            SetVehicleToRespawn(pVeh[privcarid]);

            SetTimerEx("DelayedPrivCarPut", 1500, false, "dd", playerid, privcarid);
            msg(playerid, pVehRegisterColor[privcarid], "Zmieni³eœ(aœ) napis na rejestracji na: {c}%s{/c}.", dest);
            return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 9, ""), 1;
        }

        case DIALOG_PRIVCAR_HEADER: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 6, "");
            if(getMoney(playerid) < 3000) return valueError(playerid, 3000);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 3 || strlen(dest) > 20) return showDialog(playerid, DIALOG_PRIVCAR_HEADER, DIALOG_STYLE_INPUT, ""YELLOW"Edycja nag³ówka pojazdu", ""WHITE"Wpisz poni¿ej nazwê dla nag³ówka pojazdu:\n\n"RED"Przekroczono limit znaków - dozwolony limit to: "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");
            giveMoney(playerid, -3000);

            new privcarid = pVehManage[playerid];
            format(pVehEtykietaName[privcarid], 50, "%s", dest);
            m_pquery_format("UPDATE es_privcars SET name='%s' WHERE id=%d LIMIT 1;", dest, privcarid);
            UpdateDynamic3DTextLabelText(pVehLabel[privcarid], -1, UpdatePrivCarLabel(privcarid));
            return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 6, ""), 1;
        }

        case DIALOG_PRIVCAR_PLATE_COLOR: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 8, "");

            new privcarid = pVehManage[playerid];
            pVehRegisterColor[privcarid] = KoloryGraczy[listitem];
            m_pquery_format("UPDATE es_privcars SET register_color=%d WHERE id=%d LIMIT 1;", KoloryGraczy[listitem], privcarid);
            
            SetVehicleNumberPlate(pVeh[privcarid], form("{%06x}%s", pVehRegisterColor[privcarid] >>> 8, pVehRegisterName[privcarid]));
            SetVehicleToRespawn(pVeh[privcarid]);

            SetTimerEx("DelayedPrivCarPut", 1500, false, "dd", playerid, privcarid);
            msg(playerid, KoloryGraczy[listitem], "Zmieni³eœ(aœ) {c}kolor{/c} swojej rejestracji pojazdu na taki, jak {c}ta wiadomoœæ{/c}.");
            return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 8, ""), 1;
        }

        case DIALOG_PRIVCAR_ETYKIETA_COLOR: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 6, "");

            if(listitem != INVALID_LISTITEM) {
                new i = GetPVarInt(playerid, "etykieta_listitem"), privcarid = pVehManage[playerid];
                pVehEtykieta[privcarid][i] = KoloryGraczy[listitem];
                m_pquery_format("UPDATE es_privcars SET etykieta='%s' WHERE id=%d LIMIT 1;", GetEtykietaColor(privcarid), privcarid);
                UpdateDynamic3DTextLabelText(pVehLabel[privcarid], -1, UpdatePrivCarLabel(privcarid));
                return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 6, ""), 1;
            }
        }

        case DIALOG_PRIVCAR_ETYKIETA: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));

            switch(listitem) {
                case 1: OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 6, "");
                case 0: showDialog(playerid, DIALOG_PRIVCAR_HEADER, DIALOG_STYLE_INPUT, ""YELLOW"Edycja nag³ówka pojazdu", ""WHITE"Wpisz poni¿ej nazwê dla nag³ówka pojazdu:", "Ustaw", "Cofnij");
                
                default: {
                    SetPVarInt(playerid, "etykieta_listitem", listitem - 2);
                    
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
                    showDialog(playerid, DIALOG_PRIVCAR_ETYKIETA_COLOR, DIALOG_STYLE_LIST, ""BLUE"Wybierz kolor etykiety", #, "Ustaw", "Cofnij", 10);
                }
            }
            return 1;
        }
    }
    return 0;
}
function DelayedPrivCarPut(playerid, privcarid) return PutPlayerInVehicle(playerid, pVeh[privcarid], 0);

function ChangePrivCarModel(playerid, privcarid, modelid)
{
    if(privcarid == -1 || modelid == -1) return 1;
    pVehModel[privcarid] = modelid;
    
    DestroyPrivCar(privcarid);
    SetTimerEx("CreatePrivCar", 300, false, "ddbd", playerid, privcarid, false, PRIVCAR_TELEPORT_VEHICLE);
    task_await(RunPrivCarManage(playerid, privcarid));
    return msg(playerid, C_GOLD, "Zmieni³eœ(aœ) swój model pojazdu na: {c}%s (ID: %d){/c}.", GetVehicleName(modelid), modelid), 1;
}

CMD:pmanage(playerid, params[])
{
    if(pVehCount[playerid] == 0) return sendError(playerid, ""RED"Nie masz prywatnych pojazdów.");

    new slotid;
    if(sscanf(params, "d", slotid)) return syntaxError(playerid, "/pmanage [id slotu]");
    if(slotid < 1 || slotid > 5) return sendError(playerid, ""RED"Dostêpne sloty na serwerze to: "WHITE"1-5"RED".");
    if(pVehHave[playerid][slotid - 1] == -1) return sendError(playerid, ""RED"Nie masz ¿adnego prywatnego pojazdu w tym slocie.");
    if(pVehSell[slotid - 1]) return sendError(playerid, ""RED"Ten pojazd zosta³ wystawiony na sprzeda¿!");
    return task_await(RunPrivCarManage(playerid, pVehHave[playerid][slotid - 1]));
}
