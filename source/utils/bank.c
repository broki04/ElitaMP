#include YSI_Coding\y_hooks

enum e_creditData {
    credit_cost,
    credit_level
};

new 
    bankEnterPickup,
    bankExitPickup,
    bankCheckpoint[4],

    pBank[MAX_PLAYERS],
    pBankTarget[MAX_PLAYERS],
    pBankAmount[MAX_PLAYERS],

    pCredit[MAX_PLAYERS],
    creditData[][e_creditData] = {
        {100000,    5},
        {300000,    10},
        {500000,    15},
        {1000000,   30},
        {3000000,   45},
        {5000000,   50},
        {7000000,   70},
        {10000000,  100}
    }
;

hook function ResetPlayerData(playerid) {
    pBank[playerid] = pCredit[playerid] = pBankAmount[playerid] = 0;
    pBankTarget[playerid] = -1;
    return continue(playerid);
}

stock Init_Bank()
{
    bankEnterPickup = CreateDynamicPickup(19132, 1, 2412.5017, 1123.8384, 10.8503, 0);
    CreateDynamic3DTextLabel(""YELLOW"Premium "GREEN"Bank\n"WHITE"Wejœcie", -1, 2412.5017, 1123.8384, 10.8503 + 0.15, 30.0, .worldid=0);

    bankExitPickup = CreateDynamicPickup(19132, 1, 1412.2805, 1315.8573, 1501.0840, .worldid=VW_BANK, .interiorid=VW_BANK);
    CreateDynamic3DTextLabel(""YELLOW"Premium "GREEN"Bank\n"WHITE"Wyjœcie", -1, 1412.2805, 1315.8573, 1501.0840 + 0.15, 30.0, .worldid=VW_BANK, .interiorid=VW_BANK);

    new const Float:bankPos[][] = {
        {1401.0759, 1320.1687, 1501.0859},
        {1402.9377, 1320.1708, 1501.0840},
        {1407.3685, 1320.1680, 1501.0840},
        {1409.2236, 1320.1692, 1501.0840}
    };

    for(new i = 0; i < sizeof(bankPos); i++) {
        bankCheckpoint[i] = CreateDynamicCP(unpackXYZ(bankPos[i]), 1.0, .worldid=VW_BANK, .interiorid=VW_BANK);
        CreateDynamic3DTextLabel(form(""GOLD"MIEJSCE #%d\n"WHITE"PodejdŸ tutaj, by zarz¹dzaæ swoim "BLUE"kontem bankowym"WHITE".", i + 1), -1, unpackXYZ(bankPos[i]), 30.0, .worldid=VW_BANK, .interiorid=VW_BANK);
    }

    bankCheckpoint[0] = CreateDynamicCP(1401.0759, 1320.1687, 1501.0859, 1.0, .worldid=VW_BANK, .interiorid=VW_BANK);
    bankCheckpoint[1] = CreateDynamicCP(1402.9377, 1320.1708, 1501.0840, 1.0, .worldid=VW_BANK, .interiorid=VW_BANK);
    bankCheckpoint[2] = CreateDynamicCP(1407.3685, 1320.1680, 1501.0840, 1.0, .worldid=VW_BANK, .interiorid=VW_BANK);
    bankCheckpoint[3] = CreateDynamicCP(1409.2236, 1320.1692, 1501.0840, 1.0, .worldid=VW_BANK, .interiorid=VW_BANK);

    CreateDynamicActor(11, 1401.0792, 1321.8352, 1501.0919, 178.9307, .worldid=VW_BANK, .interiorid=VW_BANK);
    CreateDynamicActor(11, 1402.9586, 1321.8352, 1501.0919, 179.8493, .worldid=VW_BANK, .interiorid=VW_BANK);
    CreateDynamicActor(11, 1407.2700, 1321.8348, 1501.0919, 178.7001, .worldid=VW_BANK, .interiorid=VW_BANK);
    CreateDynamicActor(11, 1409.1351, 1321.8361, 1501.0919, 179.1597, .worldid=VW_BANK, .interiorid=VW_BANK);    

    Heist_Init();
}

stock Bank_Pickup(playerid, pickupid)
{
    Heist_Pickup(playerid, pickupid);

    if(pickupid == bankEnterPickup) return Teleport(playerid, false, 1409.53857, 1316.47266, 1501.04822 + 1.0, 0.0, VW_BANK, VW_BANK, true);
    if(pickupid == bankExitPickup) return EnterTeleport(playerid, "bank");
    return 1;
}

stock IsPlayerNearBankCP(playerid)
{
    new id = -1;
    for(new i = 0; i < 4; i++) {
        if(IsPlayerInDynamicCP(playerid, bankCheckpoint[i])) {
            id = i;
            break;
        }
    }
    return id;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new bankid = IsPlayerNearBankCP(playerid);
    if(bankid != -1 && checkpointid == bankCheckpoint[bankid]) return ShowBankPanel(playerid);
    return 1;
}

stock ShowBankPanel(playerid)
{
    s[0] = EOS;
    catstr(s, C_CORAL, -1, "Aktualny stan konta: {c}%s${/c}\n", strdot(pBank[playerid]));
    strcat(s, " \n");
    catstr(s, C_LBLUE, -1, "  {c}"ARROW"{/c} Wp³ata na konto\n");
    catstr(s, C_LBLUE, -1, "  {c}"ARROW"{/c} Wyp³ata z konta\n");
    catstr(s, C_LBLUE, -1, "  {c}"ARROW"{/c} Przelew tradycyjny\n");
    catstr(s, C_LBLUE, -1, "  {c}"ARROW"{/c} Kredyty\n");
    return showDialog(playerid, DIALOG_BANK_PANEL, DIALOG_STYLE_LIST, ""BLUE2"Premium Bank", s, "Wybierz", "Zamknij"), 1;
}

stock OnBankDialog(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BANK_PAYMENT: {
            if(!response) return ShowBankPanel(playerid);

            new bankid = playerData[playerid][player_saveData][0];
            if(strmatch(inputtext, "all", true)) {
                switch(bankid) {
                    case 0: {
                        new amount = getMoney(playerid);
                        pBank[playerid] = amount;
    
                        giveMoney(playerid, -amount);
                        msg(playerid, C_CORAL, "%s {c}%s${/c} na konto bankowe!", (bankid == 0) ? ("Wp³aci³eœ(aœ)") : ("Wyp³aci³eœ(aœ)"), strdot(amount));
                    }

                    case 1: {
                        new amount = pBank[playerid];
                        pBank[playerid] = 0;
        
                        giveMoney(playerid, amount);
                        msg(playerid, C_CORAL, "%s {c}%s${/c} z konta bankowego!", (bankid == 0) ? ("Wp³aci³eœ(aœ)") : ("Wyp³aci³eœ(aœ)"), strdot(amount));
                    }
                }

                m_pquery_format("UPDATE es_accounts SET bank=%d WHERE id=%d LIMIT 1;", pBank[playerid], playerData[playerid][player_accountID]);
                ShowBankPanel(playerid);
            } else {
                new amount = strval(inputtext);
                if(!isNumeric(inputtext) || amount < 1 || (bankid == 0 && amount > (getMoney(playerid))) || (bankid == 1 && amount > pBank[playerid])) {
                    s[0] = EOS;
                    catstr(s, C_LGREEN, -1, "Wpisz poni¿ej kwotê, któr¹ chcesz {c}%s{/c} na konto:\n"LGREEN"Wpisz {c}all{/c} aby wyp³aciæ ca³¹ kwotê.\n\n", (bankid == 0) ? ("wp³aciæ") : ("wyp³aciæ"));
                    catstr(s, C_LRED, -1, "Wprowadzona kwota jest nieprawid³owa!");
                    return showDialog(playerid, DIALOG_BANK_PAYMENT, DIALOG_STYLE_INPUT, ""GREEN"Zarz¹dzanie maj¹tkiem", s, "Wykonaj", "Cofnij");
                } 

                switch(bankid) {
                    case 0: {
                        pBank[playerid] += amount;
                        giveMoney(playerid, -amount);
                        msg(playerid, C_CORAL, "%s {c}%s${/c} na konto bankowe!", (bankid == 0) ? ("Wp³aci³eœ(aœ)") : ("Wyp³aci³eœ(aœ)"), strdot(amount));
                    }

                    case 1: {
                        pBank[playerid] -= amount;
                        giveMoney(playerid, amount);
                        msg(playerid, C_CORAL, "%s {c}%s${/c} z konta bankowego!", (bankid == 0) ? ("Wp³aci³eœ(aœ)") : ("Wyp³aci³eœ(aœ)"), strdot(amount));
                    }
                }

                m_pquery_format("UPDATE es_accounts SET bank=%d WHERE id=%d LIMIT 1;", pBank[playerid], playerData[playerid][player_accountID]);
                ShowBankPanel(playerid);
            }
            return 1;
        }

        case DIALOG_BANK_PANEL: {
            if(!response) return 1;

            switch(listitem) {
                case 0, 1: ShowBankPanel(playerid);    

                case 2, 3: {
                    playerData[playerid][player_saveData][0] = listitem - 2;

                    s[0] = EOS;
                    catstr(s, C_LGREEN, -1, "Wpisz poni¿ej kwotê, któr¹ chcesz {c}%s{/c} na konto:\n"LGREEN"Wpisz {c}all{/c} aby wyp³aciæ ca³¹ kwotê.", (listitem == 2) ? ("wp³aciæ") : ("wyp³aciæ"));
                    return showDialog(playerid, DIALOG_BANK_PAYMENT, DIALOG_STYLE_INPUT, ""GREEN"Zarz¹dzanie maj¹tkiem", s, "Wykonaj", "Cofnij");
                }

                case 4: {
                    if(pBank[playerid] < 1) return dialogBox(playerid, C_RED, "Nie mo¿esz przeprowadziæ przelewu!");

                    s[0] = EOS;
                    s = ""CORAL"Kategoria\t"WHITE"Rezultat\n";
                    catstr(s, C_CORAL, -1, "Wybrany gracz:\t"WHITE"%s\n", (pBankTarget[playerid] != -1) ? (form("%s(%d)", getNick(pBankTarget[playerid]), pBankTarget[playerid])) : ("---"));
                    catstr(s, C_CORAL, -1, "Kwota przelewu:\t"WHITE"%s$\n", strdot(pBankAmount[playerid]));
                    strcat(s, " \n");
                    catstr(s, C_LRED, -1, "Wykonaj przelew tradycyjny");
                    showDialog(playerid, DIALOG_BANK_PRZELEW, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Przelew tradycyjny", s, "Wybierz", "Cofnij");
                }

                case 5: {
                    if(pCredit[playerid] > 0) {
                        s[0] = EOS;
                        catstr(s, C_GITOWY, -1, "Posiadasz zaci¹gniêty kredyt na: {c}%s${/c}.\n", strdot(pCredit[playerid]));
                        catstr(s, C_GITOWY, -1, "Procent sp³acania kredytu wynosi zawsze {c}0.2{/c} procent.\n\n");
                        catstr(s, C_LY, -1, "Od momentu zaci¹gniêcia kredytu, serwer automatycznie pobiera ten procent z zarobku!");

                        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                            #pragma unused pp, dd, rr, ll, ii
                            return ShowBankPanel(playerid), 1; }
                        return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""VIOLET"Kredyt", s, "OK", #);
                    }

                    s[0] = EOS;
                    s = ""CORAL"Kwota\t"WHITE"Zdolnoœæ kredytowa\n";
                    for(new i = 0; i < sizeof(creditData); i++) format(s, sizeof(s), "%s"CORAL"%s$\t%s%d lvl\n", s, strdot(creditData[i][credit_cost]), (playerData[playerid][player_level] >= creditData[i][credit_level]) ? (GREEN) : (RED), creditData[i][credit_level]);
                    showDialog(playerid, DIALOG_BANK_CREDIT, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Kredyt", s, "Wybierz", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_BANK_CREDIT: {
            if(!response) return ShowBankPanel(playerid);
            if(playerData[playerid][player_level] < creditData[listitem][credit_level]) return dialogBox(playerid, C_RED, "Nie posiadasz {c}%d{/c} poziomu, by móc wybraæ t¹ kwotê kredytu!", creditData[listitem][credit_level]);

            s[0] = EOS;
            catstr(s, C_FAJNY, -1, "Czy napewno chcesz zaci¹gn¹æ kredyt na {c}%s${/c}?\n", strdot(creditData[listitem][credit_cost]));
            catstr(s, C_FAJNY, -1, "Od momentu zaci¹gniêcia kredytu, 2 procent od zarobku bêdzie przekazane na sp³atê kredytu!\n");
            catstr(s, C_LRED, -1, "Kliknij poni¿ej odpowiedni przycisk, je¿eli chcesz zaci¹gn¹æ kredyt.");

            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) return OnDialogResponse(playerid, DIALOG_BANK_PANEL, 1, 5, "");

                giveMoney(playerid, creditData[listitem][credit_cost]);
                pCredit[playerid] = creditData[listitem][credit_cost];

                m_pquery_format("UPDATE es_accounts SET credit=%d WHERE id=%d LIMIT 1;", pCredit[playerid], playerData[playerid][player_accountID]);
                return msg(playerid, C_FAJNY, "Zaci¹gn¹³eœ(aœ) {c}%s${/c} kredytu! Wylosowany procent: {c}2.0{/c} proc.", strdot(creditData[listitem][credit_cost])), 1;
            }

            Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE"Zaci¹gniêcie kredytu", s, "Zaci¹gnij", "Cofnij");
            return 1;
        }

        case DIALOG_BANK_TARGET: {
            if(!response) return OnDialogResponse(playerid, DIALOG_BANK_PANEL, 1, 4, "");

            new targetid = playerData[playerid][player_saveData][listitem];
            if(!IsPlayerConnected(targetid) || targetid == playerid) return ShowBankPanel(playerid);
            
            pBankTarget[playerid] = targetid;
            return OnDialogResponse(playerid, DIALOG_BANK_PANEL, 1, 4, ""), 1;
        }

        case DIALOG_BANK_KWOTA: {
            if(!response) return OnDialogResponse(playerid, DIALOG_BANK_PANEL, 1, 4, "");

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1 || amount > pBank[playerid]) return showDialog(playerid, DIALOG_BANK_KWOTA, DIALOG_STYLE_INPUT, ""GOLD"Przelew tradycyjny - kwota", ""WHITE"Wpisz poni¿ej kwotê, któr¹ chcesz przelaæ:\n\n"LRED"Wprowadzona kwota jest nieprawid³owa!", "Ustaw", "Cofnij");
            
            pBankAmount[playerid] = amount;
            return OnDialogResponse(playerid, DIALOG_BANK_PANEL, 1, 4, ""), 1;
        }

        case DIALOG_BANK_PRZELEW: {
            if(!response) return ShowBankPanel(playerid);

            switch(listitem) {
                case 2: ShowBankPanel(playerid);

                case 0: {
                    new x = 0;
                    foreach(new i : Player) {
                        if(i == playerid) continue;
                        AddDialogListitem(playerid, "  "WHITE""ARROW" {%06x}%s(%d)\n", GetPlayerColor(i) >>> 8, getNick(i), i);
                        playerData[playerid][player_saveData][x++] = i;
                    }
                    if(!x) return dialogBox(playerid, C_RED, "Aktualnie brak graczy do wykonania przelewu."); 
                    showDialog(playerid, DIALOG_BANK_TARGET, DIALOG_STYLE_LIST, ""BLUE2"Przelew tradycyjny - wybierz gracza", #, "Wybierz", "Cofnij", 10);
                }
                case 1: showDialog(playerid, DIALOG_BANK_KWOTA, DIALOG_STYLE_INPUT, ""GOLD"Przelew tradycyjny - kwota", ""WHITE"Wpisz poni¿ej kwotê, któr¹ chcesz przelaæ:", "Ustaw", "Cofnij");

                case 3: {
                    if(pBankTarget[playerid] == -1) return dialogBox(playerid, C_RED, "Nie wybrano gracza, który otrzyma przelew.");
                    if(pBankAmount[playerid] < 1) return dialogBox(playerid, C_RED, "Nie ustalono kwoty, która zostaje przelana!");

                    new targetid = pBankTarget[playerid], amount = pBankAmount[playerid];
                    pBank[playerid] -= amount;
                    pBank[targetid] += amount;

                    m_pquery_format("UPDATE es_accounts SET bank=%d WHERE id=%d LIMIT 1;", pBank[playerid], playerData[playerid][player_accountID]);
                    m_pquery_format("UPDATE es_accounts SET bank=%d WHERE id=%d LIMIT 1;", pBank[targetid], playerData[targetid][player_accountID]);

                    msg(playerid, C_ORANGE, "Przela³eœ(aœ) {c}%s${/c} graczowi: {c}%s(%d){/c}.", strdot(amount), getNick(targetid), targetid);
                    msg(targetid, C_ORANGE, "Gracz {c}%s(%d){/c} przela³(a) Ci: {c}%s${/c}.", getNick(playerid), playerid, strdot(amount));

                    ShowBankPanel(playerid);
                }
            }
            return 1;
        }
    }
    return 0;
}