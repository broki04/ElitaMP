#include YSI_Coding\y_hooks

new pGangMarket[MAX_PLAYERS][MAX_PLAYERS];
hook function ResetPlayerData(playerid) {
    for(new i = 0; i < MAX_PLAYERS; i++) pGangMarket[playerid][i] = -1;
    return continue(playerid);
}

stock Task:ShowMarketManage(playerid, targetid)
{
    new Task:t = task_new();

    new gangid = GetPVarInt(playerid, "player_market_gang");
    if(gangData[gangid][gang_ownerid] == playerData[targetid][player_accountID]) return dialogBox(playerid, C_RED, ""RED"Nie mo�esz z�o�y� oferty do w�a�ciciela zespo�u."), t;
    if(playerData[targetid][player_gang_loan] && playerData[targetid][player_gang_loan_expire] > 0) return dialogBox(playerid, C_RED, ""RED"Nie mo�esz z�o�y� oferty do gracza, kt�ry jest wypo�yczony."), t;

    s[0] = EOS;
    strcat(s, ""WHITE"Wybierz poni�ej odpowiedni rodzaj transakcji:\n");
    strcat(s, ""WHITE"�  "CORAL"Transfer\n");
    //strcat(s, ""WHITE"�  "CORAL"Wypo�yczenie mi�dzy-gangowe\n");

    playerData[playerid][player_saveData][0] = targetid;
    new title[128]; format(title, sizeof(title), ""BLUE"Rynek: "WHITE"%s(%d)", getNick(targetid), targetid);
    return showDialog(playerid, DIALOG_GANG_MARKET_TYPE, DIALOG_STYLE_LIST, title, s, "Wybierz", "Cofnij"), t;
}

stock OnDialogGangMarket(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_GANG_MARKET: {
            if(!response) return callcmd::team(playerid);

            s[0] = EOS;
            s = ""BLACK"ID\t"WHITE"Nick\t"LY"Ranga\t"MINT"Cena\n";

            SetPVarInt(playerid, "player_market_listitem", listitem);
            SetPVarInt(playerid, "player_market_gang", pTransferData[playerid][listitem]);

            new gangid = pTransferData[playerid][listitem], x = 0;
            foreach(new i : GangMember[gangid]) {
                if(!IsPlayerConnected(i) || gangData[gangid][gang_ownerid] == playerData[i][player_accountID]) continue;
                format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s(%d)\t{%06x}%s\t"MINT"�%s\n", s, i + 1, getNick(i), i, pGangRankColor[i] >>> 8, pGangRankName[i], strdot(GetPlayerBudget(i)));
                pGangMarket[playerid][x++] = i;
            }
            if(!x) return dialogBox(playerid, C_RED, ""RED"Brak aktywnych cz�onk�w zespo�u.");

            new title[128]; format(title, sizeof(title), "{%06x}%s [%s]", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            return showDialog(playerid, DIALOG_GANG_MARKET_LIST, DIALOG_STYLE_TABLIST_HEADERS, title, s, "Wybierz", "Cofnij");
        }

        case DIALOG_GANG_MARKET_TYPE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG_MARKET, 1, GetPVarInt(playerid, "player_market_listitem"), "");

            new targetid = playerData[playerid][player_saveData][0], title[128];
            format(title, sizeof(title), ""BLUE"%s "WHITE"%s(%d)", (listitem == 1) ? ("Transfer") : ("Wypo�yczenie"), getNick(targetid), targetid);
            
            switch(listitem) {
                case 0: task_await(ShowMarketManage(playerid, targetid));
                case 1: {
                    s[0] = EOS;
                    format(s, sizeof(s), ""WHITE"Wpisz poni�ej odpowiedni� kwot�, za kt�r� chcesz zakupi� tego gracza?\n"WHITE"Aktualna warto�� rynkowa to: "YELLOW"�%s"WHITE".", strdot(GetPlayerBudget(targetid)));
                    showDialog(playerid, DIALOG_GANG_MARKET_TRANSFER, DIALOG_STYLE_INPUT, title, s, "Wy�lij", "Cofnij");
                }
                case 2: showDialog(playerid, DIALOG_GANG_MARKET_LOAN, DIALOG_STYLE_INPUT, title, ""WHITE"Wpisz poni�ej okres wypo�yczenia w przedziale "YELLOW"1-30 "WHITE"dni.", "Wy�lij", "Cofnij");
            }
            return 1;
        }

        case DIALOG_GANG_MARKET_TRANSFER: {
            if(!response) return task_await(ShowMarketManage(playerid, playerData[playerid][player_saveData][0]));

            new targetid = playerData[playerid][player_saveData][0], amount, cost = GetPlayerBudget(targetid);
            if(sscanf(inputtext, "d", amount)) {
                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Wpisz poni�ej odpowiedni� kwot�, za kt�r� chcesz zakupi� tego gracza?\n"WHITE"Aktualna warto�� rynkowa to: "YELLOW"�%s"WHITE".\n\n"RED"Koszt musi by� podany w liczbach!", strdot(cost));
               
                new title[128]; format(title, sizeof(title), ""BLUE"Transfer "WHITE"%s(%d)", getNick(targetid), targetid);
                return showDialog(playerid, DIALOG_GANG_MARKET_TRANSFER, DIALOG_STYLE_INPUT, title, s, "Wy�lij", "Cofnij");
            }

            if(amount < cost) {
                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Wpisz poni�ej odpowiedni� kwot�, za kt�r� chcesz zakupi� tego gracza?\n"WHITE"Aktualna warto�� rynkowa to: "YELLOW"�%s"WHITE".", strdot(cost));

                new title[128]; format(title, sizeof(title), ""BLUE"Transfer "WHITE"%s(%d)", getNick(targetid), targetid);
                return showDialog(playerid, DIALOG_GANG_MARKET_TRANSFER, DIALOG_STYLE_INPUT, title, s, "Wy�lij", "Cofnij");
            }

            new gangid = pGang[playerid];
            if(amount < gangData[gangid][gang_budget]) {
                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Wpisz poni�ej odpowiedni� kwot�, za kt�r� chcesz zakupi� tego gracza?\n"WHITE"Aktualna warto�� rynkowa to: "YELLOW"�%s"WHITE".\n\n"RED"Nie posiadasz takiej kwoty! Brakuje Wam: {c}�%s{/c}.", strdot(cost), strdot(amount - gangData[gangid][gang_budget]));

                new title[128]; format(title, sizeof(title), ""BLUE"Transfer "WHITE"%s(%d)", getNick(targetid), targetid);
                return showDialog(playerid, DIALOG_GANG_MARKET_TRANSFER, DIALOG_STYLE_INPUT, title, s, "Wy�lij", "Cofnij");
            }

            if(pGangTransferID[targetid] != -1) {
                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Wpisz poni�ej odpowiedni� kwot�, za kt�r� chcesz zakupi� tego gracza?\n"WHITE"Aktualna warto�� rynkowa to: "YELLOW"�%s"WHITE".\n\n"RED"Ten gracz otrzyma� ju� ofert� transferu do innego zespo�u.", strdot(cost));

                new title[128]; format(title, sizeof(title), ""BLUE"Transfer "WHITE"%s(%d)", getNick(targetid), targetid);
                return showDialog(playerid, DIALOG_GANG_MARKET_TRANSFER, DIALOG_STYLE_INPUT, title, s, "Wy�lij", "Cofnij");
            }
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Tw�j zesp� z�o�y�(a) ofert� transferow� {c}%s(%d){/c} za: {c}�%s{/c}.", getGangTag(gangid), getNick(targetid), targetid, strdot(amount));

            s[0] = EOS;
            catstr(s, C_WHITE, -1, "Otrzyma�e�(a�) ofert� transferu do zespo�u: {%06x}%s [%s]{/c}.\n", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            catstr(s, C_WHITE, -1, "Kwota transferu wynosi: {%06x}�%s{/c}.\n", getGangColor(gangid) >>> 8, strdot(amount));
            catstr(s, C_CORAL, -1, "Kliknij poni�ej odpowiedni przycisk, kt�rym {c}zaakceptujesz/odrzucisz{/c} ofert�!");

            pGangTransferID[targetid] = gangid;
            pGangTransferCost[targetid] = amount;
            pGangTransferTimer[targetid] = SetTimerEx("TransferResetGang", 1000 * 20, false, "ddb", targetid, gangid, false);

            new title[150]; format(title, sizeof(title), "{%06x}Oferta transferowa od: "WHITE"%s [%s]", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            return showDialog(targetid, DIALOG_GANG_TRANSFER_CONFIRM, DIALOG_STYLE_MSGBOX, title, s, "Akceptuj", "Odrzu�"), 1;
        }

        case DIALOG_GANG_TRANSFER_CONFIRM: {
            new gangid = pGangTransferID[playerid];
            if(!response) {
                pGangTransferID[playerid] = -1; pGangTransferCost[playerid] = 0;
                foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Gracz {c}%s(%d){/c} odrzuci�(a) ofert� transferow�!", getGangTag(gangid), getNick(playerid), playerid);
                return msg(playerid, C_CHAT, "Odrzuci�e�(a�) ofert� transferow� od zespo�u: {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid)), 1;
            }

            new old_gangid = pGang[playerid];
            for(new i = 0; i < 3; i++) TextDrawHideForPlayer(playerid, gangData[old_gangid][gang_draw][i]);
            Iter_Remove(GangMember[old_gangid], playerid);
            HidePlayerProgressBar(playerid, GangBar[playerid]);
            UpdateGangBox(old_gangid);

            pGang[playerid] = gangid;
            format(pGangRankName[playerid], 50, "%s", gangData[gangid][gang_rankDefault]);
            pGangRankColor[playerid] = KoloryGraczy[random(sizeof(KoloryGraczy))];
            for(new i = 0; i < 5; i++) pGangInvites[playerid][i] = -1;

            new permstr[256];
            for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                pGangRankPerm[playerid][i] = false;
                format(permstr, sizeof(permstr), "%s%s%d", permstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);
            }

            giveGangBudget(gangid, -pGangTransferCost[playerid]);
            giveGangBudget(old_gangid, pGangTransferCost[playerid]);

            msgAll(getGangColor(gangid), "Zesp� {c}%s [%s]{/c} sprowadzi� do siebie {c}%s(%d){/c} za: {c}�%s{/c}.", getGangName(gangid), getGangTag(gangid), getNick(playerid), playerid, strdot(pGangTransferCost[playerid]));
            m_pquery_format("UPDATE es_accounts SET gang=%d, gang_rank='%s', gang_rank_color=%d, gang_rank_permission='%s' WHERE id=%d LIMIT 1;", pGang[playerid], pGangRankName[playerid], pGangRankColor[playerid], permstr, playerData[playerid][player_accountID]);
            LoadPlayerGang(playerid, true);

            pGangTransferID[playerid] = -1;
            pGangTransferCost[playerid] = 0;
            return KillTimer(pGangTransferTimer[playerid]);
        }

        case DIALOG_GANG_MARKET_LOAN: {
            if(!response) return task_await(ShowMarketManage(playerid, playerData[playerid][player_saveData][0]));
    
            new time, gangid = pGang[playerid], targetid = playerData[playerid][player_saveData][0], title[150]; 
            format(title, sizeof(title), ""BLUE"Wypo�yczenie "WHITE"%s(%d)", getNick(targetid), targetid);

            if(sscanf(inputtext, "d", time)) return showDialog(playerid, DIALOG_GANG_MARKET_LOAN, DIALOG_STYLE_INPUT, title, ""WHITE"Wpisz poni�ej okres wypo�yczenia w przedziale "YELLOW"1-30 "WHITE"dni.\n\n"RED"Nieprawid�owe parametry.", "Wy�lij", "Cofnij");
            if(time < 1 || time > 30) return showDialog(playerid, DIALOG_GANG_MARKET_LOAN, DIALOG_STYLE_INPUT, title, ""WHITE"Wpisz poni�ej okres wypo�yczenia w przedziale "YELLOW"1-30 "WHITE"dni.\n\n"RED"Dozwolony przedzia� d�ugo�ci wypo�yczenia to: "WHITE"1-30 "RED"dni.", "Wy�lij", "Cofnij");
            if(playerData[targetid][player_gang_loan]) return showDialog(playerid, DIALOG_GANG_MARKET_LOAN, DIALOG_STYLE_INPUT, title, ""WHITE"Wpisz poni�ej okres wypo�yczenia w przedziale "YELLOW"1-30 "WHITE"dni.\n\n"RED"Ten gracz jest na wypo�yczeniu w tym gangu!", "Wy�lij", "Cofnij");
            
            s[0] = EOS;
            catstr(s, C_WHITE, -1, "Otrzyma�e�(a�) propozycj� wypo�yczenia do zespo�u: {%06x}%s [%s]{/c}\n.", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            catstr(s, C_WHITE, -1, "Wypo�yczenie w przypadku akceptacji b�dzie trwa�o: {%06x}%d{/c} %s.\n", getGangColor(gangid) >>> 8, time, ConvertTimeFormat(time, "dni", "dzie�", "dni", "dni"));
            catstr(s, C_CORAL, -1, "Kliknij poni�ej odpowiedni przycisk, kt�rym {c}zaakceptujesz/odrzucisz{/c} ofert�!");

            pGangTransferID[targetid] = gangid;
            pGangTransferCost[targetid] = time;
            pGangTransferTimer[targetid] = SetTimerEx("TransferResetGang", 1000 * 20, false, "ddb", targetid, gangid, true);

            format(title, sizeof(title), "{%06x}Oferta wypo�yczenia od: "WHITE"%s [%s]", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            return showDialog(targetid, DIALOG_GANG_LOAN_CONFIRM, DIALOG_STYLE_MSGBOX, title, s, "Akceptuj", "Odrzu�"), 1;
        }

        case DIALOG_GANG_LOAN_CONFIRM: {
            new gangid = pGangTransferID[playerid];
            if(!response) {
                pGangTransferID[playerid] = -1; pGangTransferCost[playerid] = 0;
                foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Gracz {c}%s(%d){/c} odrzuci�(a) ofert� wypo�yczenia!", getGangTag(gangid), getNick(playerid), playerid);
                return msg(playerid, C_CHAT, "Odrzuci�e�(a�) ofert� wypo�yczenia od zespo�u: {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid)), 1;
            }
            
            new permstr[256];
            for(new i = 0; i < sizeof(gangRankPermissions); i++) format(permstr, sizeof(permstr), "%s%s%i", permstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);

            inline onGangLoan() 
            {
                permstr[0] = EOS;
                for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                    pGangRankPerm[playerid][i] = false;
                    format(permstr, sizeof(permstr), "%s%s%i", permstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);
                }

                new old_gangid = pGang[playerid], time = pGangTransferCost[playerid];
                for(new i = 0; i < 3; i++) TextDrawHideForPlayer(playerid, gangData[old_gangid][gang_draw][i]);
                Iter_Remove(GangMember[old_gangid], playerid);
                HidePlayerProgressBar(playerid, GangBar[playerid]);
                UpdateGangBox(old_gangid);

                playerData[playerid][player_gang_loan] = true;
                pGang[playerid] = gangid;

                format(pGangRankName[playerid], 50, "%s", gangData[gangid][gang_rankDefault]);
                pGangRankColor[playerid] = KoloryGraczy[random(sizeof(KoloryGraczy))];
                for(new i = 0; i < 5; i++) pGangInvites[playerid][i] = -1;

                LoadPlayerGang(playerid, false);
                msgAll(getGangColor(gangid), "Gang {c}%s [%s]{/c} wypo�yczy�(a) gracza {c}%s(%d){/c} na okres {c}%d {/c}%s.", getGangName(gangid), getGangTag(gangid), getNick(playerid), playerid, time, ConvertTimeFormat(time, "dni", "dzie�", "dni", "dni"));
                m_pquery_format("UPDATE es_accounts SET player_gang_loan=%d WHERE id=%d LIMIT 1;", time, playerData[playerid][player_accountID]);
            }
            m_tquery_inline(dbconn, using inline onGangLoan, "INSERT INTO es_gangs_loan (uid, gangid, old_gangid, rank_name, rank_color, rank_perm, expire) VALUES (%d, %d, %d, '%s', %d, '%s' NOW());", playerData[playerid][player_accountID], gangid, pGang[playerid], pGangRankName[playerid], pGangRankColor[playerid] >>> 8, permstr);
            return 1;
        }

        case DIALOG_GANG_MARKET_LIST: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_MARKET, "");
            return task_await(ShowMarketManage(playerid, pGangMarket[playerid][listitem]));
        }
    }
    return 0;
}

function TransferResetGang(playerid, gangid, bool:loan)
{
    msg(playerid, C_CHAT, "Nie zd��y�e�(a�) zaakceptowa� oferty %s od zespo�u: {c}%s [%s]{/c}.", (loan) ? ("wypo�yczenia") : ("transferu"), getGangName(gangid), getGangTag(gangid));
    foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Gracz {c}%s(%d){/c} nie zd��y�(a) zaakceptowa� oferty %s!", getGangTag(gangid), getNick(playerid), playerid, (loan) ? ("wypo�yczenia") : ("transferu"));

    pGangTransferID[playerid] = -1;
    pGangTransferCost[playerid] = 0;
    return KillTimer(pGangTransferTimer[playerid]);
}