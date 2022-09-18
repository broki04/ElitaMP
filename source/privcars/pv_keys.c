stock GetKeyCount(privcarid)
{
    new count = 0;
    for(new i = 0; i < MAX_PRIVCAR_KEYS; i++) { if(pVehKeys[privcarid][i] != -1) count ++; }
    return count;
}

stock FindKeyPlayer(privcarid, playerid)
{
    new bool:find = false;
    for(new i = 0; i < MAX_PRIVCAR_KEYS; i++) {
        if(pVehKeys[privcarid][i] == playerid) {
            find = true;
            break;
        }
    }
    return find;
}

CMD:pkeys(playerid)
{
    if(pVehCount[playerid] == 0) return sendError(playerid, ""RED"Nie masz prywatnych pojazdów.");
    if(pVehID[playerid] == -1) return sendError(playerid, ""RED"Musisz byæ w prywatnym pojeŸdzie.");

    new privcarid = pVehID[playerid];
    if(pVehOwner[privcarid] != playerData[playerid][player_accountID]) return sendError(playerid, ""RED"Nie mo¿esz zarz¹dzaæ kluczykami pojazdu, który nie jest Twój.");
    if(!IsPlayerInVehicle(playerid, pVeh[privcarid])) return sendError(playerid, ""RED"Nie znajdujesz siê w prywatnym pojeŸdzie.");

    pVehManage[playerid] = privcarid;
    return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 7, ""), 1;
}

stock OnDialogPrivCarKeys(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_PRIVCAR_KEYS: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));

            new privcarid = pVehManage[playerid];
            switch(listitem) {
                case 0: {
                    if(GetKeyCount(privcarid) >= MAX_PRIVCAR_KEYS) return dialogBox(playerid, C_RED, ""RED"Wszystkie klucze od Twojego pojazdu s¹ u¿ywane.");
                    showDialog(playerid, DIALOG_PRIVCAR_GIVE_KEYS, DIALOG_STYLE_INPUT, ""BROWN"Nadawanie klucza", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz daæ kluczyki do pojazdu:", "Wyœlij", "Cofnij");
                }
                case 1: OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 7, "");

                default: {
                    new keyid = pVehKeys[privcarid][listitem - 2];
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ll, ii
                        if(!rr) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 7, "");

                        pVehKeys[privcarid][listitem - 2] = -1;

                        msg(playerid, C_GREEN, "Zabra³eœ(aœ) klucze do pojazdu graczowi: {c}%s(%d){/c}.", getNick(keyid), keyid);
                        msg(keyid, C_GREEN, "Gracz {c}%s(%d){/c} zabra³(a) Tobie klucze do pojazdu!", getNick(playerid), playerid);

                        OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 7, "");
                    }

                    s[0] = EOS;
                    catstr(s, C_BLUE2, -1, "Czy chcesz zabraæ kluczyki graczowi {c}%s(%d){/c}?\n", getNick(keyid), keyid);
                    catstr(s, C_BLUE2, -1, "Bêdziesz móg³ ponownie mu daæ kluczyki, u¿ywaj¹c komendy: {c}/pkeys{/c}.");

                    new title[128]; catstr(title, C_GOLD, -1, "Zarz¹dzanie kluczami: {c}%s(%d){/c}", getNick(keyid), keyid);
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, title, s, "Zabierz", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_PRIVCAR_GIVE_KEYS: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 7, "");
        
            new id;
            if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_PRIVCAR_GIVE_KEYS, DIALOG_STYLE_INPUT, ""BROWN"Nadawanie klucza", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz daæ kluczyki do pojazdu:\n\n"RED"Nieprawid³owe ID gracza.", "Wyœlij", "Cofnij");
            if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_PRIVCAR_GIVE_KEYS, DIALOG_STYLE_INPUT, ""BROWN"Nadawanie klucza", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz daæ kluczyki do pojazdu:\n\n"RED"Ten gracz nie jest po³¹czony z serwerem.", "Wyœlij", "Cofnij");
            if(id == playerid) return showDialog(playerid, DIALOG_PRIVCAR_GIVE_KEYS, DIALOG_STYLE_INPUT, ""BROWN"Nadawanie klucza", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz daæ kluczyki do pojazdu:\n\n"RED"Nieprawid³owe ID gracza.", "Wyœlij", "Cofnij");

            new privcarid = pVehManage[playerid];
            if(FindKeyPlayer(privcarid, id)) return showDialog(playerid, DIALOG_PRIVCAR_GIVE_KEYS, DIALOG_STYLE_INPUT, ""BROWN"Nadawanie klucza", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz daæ kluczyki do pojazdu:\n\n"RED"Ten gracz posiada ju¿ klucz do Twojego pojazdu.", "Wyœlij", "Cofnij");

            for(new i = 0; i < MAX_PRIVCAR_KEYS; i++) {
                if(pVehKeys[privcarid][i] == -1) {
                    pVehKeys[privcarid][i] = id;
                    break;
                }
            }

            msg(playerid, C_BLUE2, "Da³eœ(aœ) klucze do swojego pojazdu: {c}%s(%d){/c}. Zarz¹dzanie kluczami pod: {c}/pkeys{/c}.", getNick(id), id);
            return msg(id, C_BLUE2, "Otrzyma³eœ(aœ) dostêp do pojazdu: {c}%s{/c} od gracza: {c}%s(%d){/c}.", pVehEtykietaName[privcarid], getNick(playerid), playerid), 1;
        }
    }
    return 0;
}