alias:pbuy("psell", "pkup", "pzakup")
CMD:pbuy(playerid, params[])
{
    if(!IsPlayerInGielda(playerid)) return sendError(playerid, ""RED"Nie znajdujesz si� na gie�dzie pojazd�w.");
    if(pVehCount[playerid] >= ((pUlepszenia[playerid][ULEPSZENIE_PRIVCAR]) ? (5) : (3))) return sendError(playerid, ""RED"Posiadasz ju� maksymaln� liczb� prywatnych pojazd�w."); 

    new sellid;
    if(sscanf(params, "d", sellid)) return syntaxError(playerid, "/pbuy [numer punktu sprzeda�y]");
    if(sellid < 1 || sellid > sizeof(gieldaSellPos)) return sendError(playerid, ""RED"Wprowadzono z�y numer odpowiadaj�cy punktowi sprzeda�y.");
    if(gieldaSellOwned[sellid - 1] == -1) return sendError(playerid, ""RED"Nikt nie sprzedaje pojazdu w tym punkcie sprzeda�owym!");

    new privcarid = gieldaSellOwned[sellid - 1];
    SetPVarInt(playerid, "gielda_buy_sellid", sellid - 1);
    SetPVarInt(playerid, "gielda_buy_privcarid", privcarid);
    if(pVehOwner[privcarid] == playerData[playerid][player_accountID]) return sendError(playerid, ""RED"Nie mo�esz zakupi� pojazdu, kt�ry wystawi�e�(a�).");

    s[0] = EOS;
    catstr(s, C_CREAM, -1, "Pojazd {c}%s{/c} nale�y do: {c}%s{/c}.\n", GetVehicleName(pVehModel[privcarid]), pVehOwnerName[privcarid]);
    catstr(s, C_CREAM, -1, "W�a�ciciel przejecha� tym pojazdem {c}%.1f km{/c} ��cznie.\n", pVehDist[privcarid]);
    catstr(s, C_CREAM, -1, "Samoch�d posiada instalacj� {c}%s{/c} - pojemno�� baku: {c}%.1f L{/c}.\n\n", GetEngineName(pVehEngine[privcarid]), pVehFuelAmount[privcarid]);

    catstr(s, C_ZAJEBISTY, -1, "Koszt zakupu pojazdu wyniesie Ci�: {c}%s${/c}.\n", strdot(pVehSellCost[privcarid]));
    catstr(s, C_ZAJEBISTY, -1, "Kliknij poni�ej {c}odpowiedni przycisk{/c}, by m�c zakupi� ten {c}prywatny pojazd{/c}!");
    return showDialog(playerid, DIALOG_GIELDA_BUY, DIALOG_STYLE_MSGBOX, ""CORAL"Zakup pojazdu", s, "Zakup", "Anuluj"), 1;
}

stock OnDialogGieldaBuy(playerid, dialogid, response)
{
    switch(dialogid) {
        case DIALOG_GIELDA_BUY: {
            if(!response) return 1;

            new sellid = GetPVarInt(playerid, "gielda_buy_sellid"), privcarid = GetPVarInt(playerid, "gielda_buy_privcarid");
            if(getMoney(playerid) < pVehSellCost[privcarid]) return valueError(playerid, pVehSellCost[privcarid]);
            if(gieldaSellOwned[sellid] == -1) return dialogBox(playerid, C_RED, ""RED"Ten punkt sprzeda�owy nie jest przez nikogo zaj�ty!");
            if(pVehOwner[privcarid] == playerData[playerid][player_accountID]) return sendError(playerid, ""RED"Nie mo�esz zakupi� pojazdu, kt�ry wystawi�e�(a�).");
            if(pVehCount[playerid] >= ((pUlepszenia[playerid][ULEPSZENIE_PRIVCAR]) ? (5) : (3))) return sendError(playerid, ""RED"Posiadasz ju� maksymaln� liczb� prywatnych pojazd�w."); 

            inline onSellCar()
            {
                giveMoney(playerid, -pVehSellCost[privcarid]);

                new id = CheckPlayerOnline(pVehOwner[privcarid]);
                giveMoney(id, pVehSellCost[privcarid]);

                pVehCount[playerid] ++;
                pVehCount[id] --;

                s[0] = EOS;
                catstr(s, C_LY, -1, "Zakupi�e�(a�) prywatny pojazd od: {c}%s(%d){/c}.\n", getNick(id), id);
                catstr(s, C_LY, -1, "Koszt zakupu: {c}%s${/c}.\n", strdot(pVehSellCost[privcarid]));
                catstr(s, C_LRED, -1, "Zarz�dzanie prywatnym pojazdem pod: {c}/pmanage %d{/c}.", GetPrivCarSlot(playerid, privcarid));
                showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""LGREEN"Odkup prywatnego pojazdu", s, "OK", #);

                s[0] = EOS;
                catstr(s, C_LY, -1, "Gracz {c}%s(%d){/c} odkupi�(a) od Ciebie prywatny pojazd!\n", getNick(playerid), playerid);
                catstr(s, C_LY, -1, "Otrzymujesz za pojazd: {c}%s${/c}.", strdot(pVehSellCost[privcarid]));
                showDialog(id, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""LGREEN"Odkup prywatnego pojazdu", s, "OK", #);

                for(new i = 0; i < 5; i++) {
                    if(pVehHave[id][i] == privcarid) {
                        pVehHave[id][i] = -1;
                        break;
                    }
                }

                for(new i = 0; i < 5; i++) {
                    if(pVehHave[playerid][i] == -1) {
                        pVehHave[playerid][i] = privcarid;
                        break;
                    }
                }

                pVehOwner[privcarid] = playerData[playerid][player_accountID];
                UpdateDynamic3DTextLabelText(pVehLabel[privcarid], -1, UpdatePrivCarLabel(privcarid));
                SwitchVehicleEngine(pVeh[privcarid], true);

                pVehSell[privcarid] = false;
                pVehSellCost[privcarid] = 0;
                gieldaSellOwned[sellid] = -1;

                new gstr[256]; catstr(gstr, C_ZAJEBISTY, -1, ""BLUE2"PUNKT SPRZEDA�Y "LY"#%d\n"ZAJEBISTY"Wejd� w {c}marker{/c}, by wystawi� pojazd.", sellid);
                UpdateDynamic3DTextLabelText(gieldaSellLabel[sellid], -1, gstr);
            }
            m_pquery_inline(dbconn, using inline onSellCar, "UPDATE es_privcars SET ownerid=%d, sell=0, sell_cost='0, 0' WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID], privcarid);
            return 1;
        }
    }
    return 0;
}