stock OnDialogGangs(playerid, dialogid, response, listitem, inputtext[]) 
{
    OnDialogGangCreator(playerid, dialogid, response, listitem, inputtext);
    OnDialogGangMember(playerid, dialogid, response, listitem, inputtext);
    OnDialogSparing(playerid, dialogid, response, listitem, inputtext);
    OnDialogGangAchievements(playerid, dialogid, response, listitem);
    OnDialogGangMarket(playerid, dialogid, response, listitem, inputtext);

    switch(dialogid) {
        case DIALOG_GANG_TOP: {
            if(!response) return callcmd::team(playerid);

            inline onGangTop() {
                s[0] = EOS;
                s = ""BLACK"ID\t"WHITE"Gang\t"CORAL"Wynik\n";

                new id, name[50], tag[20], result;
                for(new i = 0; i < cache_num_rows(); i++) {
                    cache_get_value_int(i, 0, id);
                    cache_get_value(i, 1, name, 50);
                    cache_get_value(i, 2, tag, 20);
                    cache_get_value_int(i, 3, result);

                    switch(listitem) {
                        case 0: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s [%s]\t"CORAL"%s lvl\n", s, i + 1, name, tag, strdot(result));
                        case 1: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s [%s]\t"CORAL"%s zabójstw\n", s, i + 1, name, tag, strdot(result));
                        case 2: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s [%s]\t"CORAL"%s œmierci\n", s, i + 1, name, tag, strdot(result));
                        case 3: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s [%s]\t"CORAL"%s wygranych\n", s, i + 1, name, tag, strdot(result));
                        case 4: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s [%s]\t"CORAL""EURO"%s\n", s, i + 1, name, tag, strdot(result));
                    }
                }

                new top_name[][] = {"poziomów zespo³ów", "zabójstw", "œmierci", "wygranych sparingów", "bud¿etu"};
                new title[128]; format(title, sizeof(title), ""BLUE"Ranking "WHITE"%s", top_name[listitem]);

                inline onTopDialog(pp, dd, rr, ll, string:ii[]) {
                    #pragma unused pp, dd, rr, ll, ii
                    return ShowGangTop(playerid); }
                return Dialog_ShowCallback(playerid, using inline onTopDialog, DIALOG_STYLE_TABLIST_HEADERS, title, s, "OK", #), 1;
            }

            dbstr[0] = EOS;
            switch(listitem) {
                case 0: format(dbstr, sizeof(dbstr), "SELECT id, name, tag, level FROM es_gangs ORDER BY level DESC LIMIT 10;"); 
                case 1: format(dbstr, sizeof(dbstr), "SELECT id, name, tag, kills FROM es_gangs ORDER BY level DESC LIMIT 10;"); 
                case 2: format(dbstr, sizeof(dbstr), "SELECT id, name, tag, deaths FROM es_gangs ORDER BY level DESC LIMIT 10;"); 
                case 3: format(dbstr, sizeof(dbstr), "SELECT id, name, tag, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(sparing, ',', 1), ',', -1) AS UNSIGNED) `liczba` FROM es_gangs ORDER BY `liczba` DESC LIMIT 10;");
                case 4: format(dbstr, sizeof(dbstr), "SELECT id, name, tag, budget FROM es_gangs ORDER BY level DESC LIMIT 10;"); 
            }   
            m_pquery_inline(dbconn, using inline onGangTop, dbstr);
            return 1;
        }

        case DIALOG_GANG_NEWBIE: {
            if(!response) return 1;

            switch(listitem) {
                case 0: ShowGangList(playerid);
                case 1: ShowGangTop(playerid);
                case 2: ShowGangZones(playerid);
                case 3: {
                    if(getMoney(playerid) < GANG_PRICE_MONEY) return valueError(playerid, GANG_PRICE_MONEY);
                    ShowGangCreator(playerid, 1);
                }
                case 4: callcmd::tzaproszenia(playerid);
            }
            return 1;
        }

        case DIALOG_GANG: {
            if(!response) return 1;

            new gangid = pGang[playerid];
            switch(listitem) {
                case 0..3, 7: callcmd::team(playerid);

                case 4: ShowGangList(playerid);
                case 5: ShowGangTop(playerid);
                case 6: ShowGangZones(playerid);

                case GANG_DIALOG_BUDGET: {
                    s[0] = EOS;
                    catstr(s, C_YELLOW, -1, "Twój zespó³ posiada aktualnie: {c}"EURO"%s{/c} na koncie zespo³u!\n\n", strdot(gangData[gangid][gang_budget]));
                    catstr(s, C_LY, -1, "Najlepsz¹ metod¹ na zdobycie euro jest {c}zdobywanie terenów{/c} w ca³ym Los Santos!\n");
                    catstr(s, C_LY, -1, "Pamiêtaj równie¿, ¿e za {c}euro{/c} mo¿esz zakupiæ, np. ulepszenia, bazê, itp.");
                    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Bud¿et zespo³u", s, "OK", #);
                }

                case GANG_DIALOG_SPARING: {                    
                    s[0] = EOS;

                    new x = 0;
                    foreach(new i : Gangs) {
                        if(i != gangid && countGangMembers(i) > 0) {
                            AddDialogListitem(playerid, ""WHITE"»  {%06x}%s [%s] "WHITE"- %d cz³onków\n", getGangColor(i) >>> 8, getGangName(i), getGangTag(i), countGangMembers(i));
                            playerData[playerid][player_gangManage][x++] = i;
                        }
                    }
                    if(!x) return dialogBox(playerid, C_RED, ""RED"¯aden zespó³ nie posiada przynajmniej jednej osoby online.");
                    showDialog(playerid, DIALOG_SPARING_PANEL, DIALOG_STYLE_LIST, ""BLUE"Sparingi", #, "Dalej", "Cofnij", 10);
                }

                case GANG_DIALOG_ACHIEVEMENT: {
                    s[0] = EOS;
                    s = ""WHITE"Osi¹gniêcie\t"BLUE"Stan\n";
                    
                    for(new i = 0; i < sizeof(gangAchievementData); i++) format(s, sizeof(s), "%s"WHITE"%s\t%s\n", s, gangAchievementData[i][gang_achievement_name], (!pGangAchievement[gangid][i]) ? (form("%d/%d", pGangAchievementStatus[gangid][i], gangAchievementData[i][gang_achievement_needed])) : (""BLUE"WYKONANE"));
                    showDialog(playerid, DIALOG_GANG_ACHIEVEMENTS, DIALOG_STYLE_TABLIST_HEADERS, ""VIOLET2"Osi¹gniêcia zespo³u", s, "Wybierz", "Cofnij");
                }

                case GANG_DIALOG_MARKET: {
                    s[0] = EOS;
                    s = ""LGREEN"ID\t"WHITE"Nazwa\t"LY"Liczba osób\n";

                    new x = 0;
                    foreach(new i : Gangs) {
                        if(countGangMembers(i) > 0 && i != pGang[playerid]) {
                            format(s, sizeof(s), "%s"LGREEN"%d.\t{%06x}%s [%s]\t"LY"%d %s\n", s, x + 1, getGangColor(i) >>> 8, getGangName(i), getGangTag(i), countGangMembers(i), ConvertTimeFormat(countGangMembers(i), "osób", "osoba", "osoby", "osób"));
                            pTransferData[playerid][x++] = i;
                        }
                    }
                    if(!x) return dialogBox(playerid, C_RED, "Aktualnie brak aktywnych zespo³ów!");
                    showDialog(playerid, DIALOG_GANG_MARKET, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Rynek transferowy", s, "Wybierz", "Cofnij");
                }

                case GANG_DIALOG_ULEP: {
                    s[0] = EOS;
                    s = ""LY"Ulepszenie\t"YELLOW"Cena\t"GREEN"Sta"RED"tus\n";
                    catstr(s, C_LY, -1, "Zwiêksz sloty zespo³u\t"BLUE2"€%s\t"CORAL"%d slotów\n", strdot(1000), gangData[gangid][gang_slots]);
                    strcat(s, " \n");
                    
                    new tempstr[128];
                    for(new i = 0; i < sizeof(GangUlepszeniaData); i++) {
                        switch(GangUlepszeniaLevel[gangid][i]) {
                            case 0: tempstr = ""GREEN""RED"IIIII\n";
                            case 1: tempstr = ""GREEN"I"RED"IIII\n";
                            case 2: tempstr = ""GREEN"II"RED"III\n";
                            case 3: tempstr = ""GREEN"III"RED"II\n";
                            case 4: tempstr = ""GREEN"IIII"RED"I\n";
                            default: tempstr = ""GREEN"IIIII\n";
                        }
                        catstr(s, C_LY, -1, "%s\t"YELLOW""EURO"%s\t%s\n", GangUlepszeniaData[i][gang_ulep_name], strdot(GangUlepszeniaData[i][gang_ulep_cost]), tempstr);
                    }
                    showDialog(playerid, DIALOG_GANG_ULEPSZENIA, DIALOG_STYLE_TABLIST_HEADERS, ""CORAL"Ulepszenia zespo³u", s, "Wybierz", "Cofnij");
                }
                case GANG_DIALOG_BASE: callcmd::bazy(playerid);

                case GANG_DIALOG_INVITE: {
                    if(!checkRankPermission(playerid, GANG_PERM_INVITE)) return gangRankError(playerid, GANG_PERM_INVITE);
                    if(CountGangMembers(gangid) >= gangData[gangid][gang_slots]) return dialogBox(playerid, C_RED, ""RED"Brak wolnych miejsc. Dokup dodatkowe sloty za "WHITE""#GANG_PRICE_SLOTS" "RED"pkt. zespo³u.");
                    showDialog(playerid, DIALOG_GANG_INVITE, DIALOG_STYLE_INPUT, ""GREEN"Zaproœ do zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz zaprosiæ do zespo³u:", "Wyœlij", "Cofnij");
                }

                case GANG_DIALOG_MANAGE: {
                    if(!checkRankPermission(playerid, GANG_PERM_MEMBERS)) return gangRankError(playerid, GANG_PERM_MEMBERS);
                    inline onGangMembers()
                    {
                        s[0] = EOS;
                        s = ""BLACK"X\t"WHITE"Nick "LY"(userid)\t"BLUE"Ranga\t"GOLD"Ostatnia wizyta\n";

                        new userid, nick[25], rank[25], rank_color, date[65], x = 0;
                        for(new i = 0; i < cache_num_rows(); i++) {
                            cache_get_value_name_int(i, "id", userid);
                            cache_get_value_name(i, "nick", nick);
                            cache_get_value_name(i, "gang_rank", rank);
                            cache_get_value_name_int(i, "gang_rank_color", rank_color);
                            cache_get_value_name(i, "date_last", date);

                            format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s "LY"(userid: %s)\t{%06x}%s\t"GOLD"%s\n", s, i + 1, nick, strdot(userid), rank_color >>> 8, rank, date);
                            playerData[playerid][player_gangManage][x++] = userid;
                        }
                        return showDialog(playerid, DIALOG_GANG_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Zarz¹dzanie cz³onkami zespo³u", s, "Wybierz", "Cofnij");
                    }
                    m_pquery_inline(dbconn, using inline onGangMembers, "SELECT * FROM es_accounts WHERE gang=%d ORDER BY id ASC LIMIT %d;", gangid, gangData[gangid][gang_slots]);
                }

                case GANG_DIALOG_SETTINGS: {
                    if(playerData[playerid][player_accountID] != gangData[gangid][gang_ownerid]) return 1;
                
                    s[0] = EOS;
                    format(s, sizeof(s), "{%06x}1. \t"WHITE"Zmieñ nazwê zespo³u\n", getGangColor(gangid) >>> 8);
                    format(s, sizeof(s), "%s{%06x}2. \t"WHITE"Zmieñ tag zespo³u\n", s, getGangColor(gangid) >>> 8);
                    format(s, sizeof(s), "%s{%06x}3. \t"WHITE"Zmieñ kolor zespo³u\n", s, getGangColor(gangid) >>> 8);
                    format(s, sizeof(s), "%s{%06x}4. \t"WHITE"Oddawanie zespo³u\n", s, getGangColor(gangid) >>> 8);
                    format(s, sizeof(s), "%s{%06x}5. \t"WHITE"Usuwanie zespo³u\n", s, getGangColor(gangid) >>> 8);
                    showDialog(playerid, DIALOG_GANG_SETTINGS, DIALOG_STYLE_LIST, ""YELLOW"Zarz¹dzanie zespo³em", s, "Wybierz", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_GANG_ULEPSZENIA: {
            if(!response) return callcmd::team(playerid);
            if(!checkRankPermission(playerid, GANG_PERM_ULEP)) return gangRankError(playerid, GANG_PERM_ULEP);

            new gangid = pGang[playerid];
            switch(listitem) {
                case 1: OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_ULEP, "");

                case 0: {
                    if(!checkRankPermission(playerid, GANG_PERM_ULEP)) return gangRankError(playerid, GANG_PERM_ULEP);
                    if(gangData[gangid][gang_budget] < 1000) return dialogBox(playerid, C_RED, "Twojemu zespo³owi brakuje {c}€%s{/c}, by móc zwiêkszyæ iloœæ slotów zespo³u.", strdot(1000 - gangData[gangid][gang_budget]));
                    if(gangData[gangid][gang_slots] >= MAX_GANG_SLOTS) return dialogBox(playerid, C_RED, ""RED"Twój zespó³ posiada ju¿ maksymaln¹ liczbê slotów.");

                    gangData[gangid][gang_slots] ++;
                    giveGangBudget(gangid, -1000);
                    CheckGangAchievement(gangid, GANG_ACHIEVEMENT_SLOTS);

                    inline onLoadGang() return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_ULEP, ""), 1;
                    m_pquery_inline(dbconn, using inline onLoadGang, "UPDATE es_gangs SET slots=%d WHERE id=%d LIMIT 1;", gangData[gangid][gang_slots], gangid);
                }

                default: {
                    new ulepid = listitem - 2;
                    if(GangUlepszeniaLevel[gangid][ulepid] >= 5) return dialogBox(playerid, C_RED, "Ulepszenie zespo³u {c}%s{/c} jest ju¿ ulepszone maksymalnie!", GangUlepszeniaData[ulepid][gang_ulep_name]);
                    if(gangData[gangid][gang_budget] < GangUlepszeniaData[ulepid][gang_ulep_cost]) return dialogBox(playerid, C_RED, "Twojemu zespo³owi brakuje {c}"EURO"%s{/c}, by zakupiæ to ulepszenie!", strdot(GangUlepszeniaData[ulepid][gang_ulep_cost] - gangData[gangid][gang_budget]));

                    GangUlepszeniaLevel[gangid][ulepid] ++;
                    giveGangBudget(gangid, -GangUlepszeniaData[ulepid][gang_ulep_cost]);
                    m_pquery_format("UPDATE es_gangs SET ulepszenia='%s' WHERE id=%d LIMIT 1;", GetGangUlepszenia(gangid), gangid);

                    foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ ulepszy³ {c}\"%s\"{/c} do {c}%d{/c} poziomu! "LY"("EURO"%s)", getGangTag(gangid), GangUlepszeniaData[ulepid][gang_ulep_name], GangUlepszeniaLevel[gangid][ulepid], strdot(GangUlepszeniaData[ulepid][gang_ulep_cost]));
                    OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_ULEP, "");
                }
            }
            return 1;
        }

        case DIALOG_GANG_LIST: {
            if(!response) return callcmd::team(playerid);
            return task_await(ShowGangInformation(playerid, playerData[playerid][player_gangData][listitem]));
        }

        case DIALOG_GANG_SETTINGS: {
            if(!response) return callcmd::team(playerid);

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_GANG_RENAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana nazwy zespo³u", ""WHITE"Wpisz poni¿ej now¹ nazwê zespo³u:", "Ustaw", "Cofnij");
                case 1: showDialog(playerid, DIALOG_GANG_RETAG, DIALOG_STYLE_INPUT, ""GOLD"Zmiana tagu zespo³u", ""WHITE"Wpisz poni¿ej nowy tag zespo³u:", "Ustaw", "Cofnij");
                case 3: showDialog(playerid, DIALOG_GANG_REOWNER, DIALOG_STYLE_INPUT, ""VIOLET"Oddawanie w³aœciciela zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz oddaæ swój zespó³:", "Oddaj", "Cofnij");
                case 4: showDialog(playerid, DIALOG_GANG_DELETE, DIALOG_STYLE_MSGBOX, ""RED"Usuwanie zespo³u", ""WHITE"Czy napewno chcesz usun¹æ swój zespó³?\n"WHITE"Kliknij poni¿ej odpowiedni przycisk.", "Usuñ", "Cofnij");

                case 2: {
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
                    showDialog(playerid, DIALOG_GANG_RECOLOR, DIALOG_STYLE_LIST, ""PINK"Zmiana koloru zespo³u", #, "Ustaw", "Cofnij", 10);
                }
            }
            return 1;
        }

        case DIALOG_GANG_DELETE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");

            new gangid = pGang[playerid];
            if(pGangSpar[gangid] || gangData[gangid][gang_battleZone] != -1) return dialogBox(playerid, C_RED, ""RED"Nie mo¿esz teraz usun¹æ zespo³u."); 

            foreach(new i : GangZones) {
                if(pGangZoneOwner[i] == gangid) {
                    pGangZoneOwner[i] = -1;
                    UpdateDynamic3DTextLabelText(pGangZoneLabel[i], -1, UpdateGangZoneLabel(i));

                    HideZoneForAll(pGangZone[i]);
                    ShowZoneForAll(pGangZone[i], setAlpha(C_GREY, 160));
                }
            }

            foreach(new i : GangMember[gangid]) {
                HidePlayerProgressBar(i, GangBar[playerid]);
                for(new x = 0; x < 3; x++) TextDrawHideForPlayer(i, gangData[gangid][gang_draw][x]);
               
                msg(i, getGangColor(gangid), "{c}(%s)*{/c}  W³aœciciel zespo³u {c}%s(%d){/c} usun¹³ zespó³!", getGangTag(gangid), getNick(playerid), playerid);
                pGang[i] = -1;
            }
            Iter_Clear(GangMember[gangid]);

            for(new i = 0; i < 3; i++) TextDrawDestroy(gangData[gangid][gang_draw][i]);

            m_pquery_format("UPDATE es_accounts SET gang=-1 WHERE gang=%d LIMIT %d;", gangid, gangData[gangid][gang_slots]);
            m_pquery_format("DELETE FROM es_gangs WHERE id=%d LIMIT 1;", gangid);
            m_pquery_format("UPDATE es_gangs_zones SET ownerid=-1 WHERE ownerid=%d", gangid);

            static const resetGangData[e_gangData];
            gangData[gangid] = resetGangData;
            return 1;
        }

        case DIALOG_GANG_REOWNER: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");

            new id;
            if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_GANG_REOWNER, DIALOG_STYLE_INPUT, ""VIOLET"Oddawanie w³aœciciela zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz oddaæ swój zespó³:\n\n"RED"Nieprawid³owe ID gracza.", "Oddaj", "Cofnij");
            if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_GANG_REOWNER, DIALOG_STYLE_INPUT, ""VIOLET"Oddawanie w³aœciciela zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz oddaæ swój zespó³:\n\n"RED"Podany gracz nie jest dostêpny.", "Oddaj", "Cofnij");
           
            new gangid = pGang[playerid];
            if(pGang[id] != gangid) return showDialog(playerid, DIALOG_GANG_REOWNER, DIALOG_STYLE_INPUT, ""VIOLET"Oddawanie w³aœciciela zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz oddaæ swój zespó³:\n\n"RED"Ten gracz nie nale¿y do Twojego zespo³u.", "Oddaj", "Cofnij");

            gangData[gangid][gang_ownerid] = playerData[id][player_accountID];
            m_pquery_format("UPDATE es_gangs SET ownerid=%d WHERE id=%d LIMIT 1;", gangData[gangid][gang_ownerid], gangid);

            for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                pGangRankPerm[playerid][i] = false;
                pGangRankPerm[id][i] = true;
            }

            m_pquery_format("UPDATE es_accounts SET gang_rank_permission='%s' WHERE id=%d LIMIT 1;", GetPlayerGangPerms(playerid), playerData[playerid][player_accountID]);
            m_pquery_format("UPDATE es_accounts SET gang_rank_permission='%s' WHERE id=%d LIMIT 1;", GetPlayerGangPerms(id), playerData[id][player_accountID]);
            
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Gracz {c}%s(%d){/c} mianowa³(a) {c}%s(%d){/c} nowym w³aœcicielem zespo³u!", getGangTag(gangid), getNick(playerid), playerid, getNick(id), id);
            return 1;
        }

        case DIALOG_GANG_RENAME: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 5, 25)) return showDialog(playerid, DIALOG_GANG_RENAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana nazwy zespo³u", ""WHITE"Wpisz poni¿ej now¹ nazwê zespo³u:\n\n"RED"Dozwolona d³ugoœæ nazwy to: "WHITE"5-25 "RED"znaków.", "Ustaw", "Cofnij");

            new gangid = pGang[playerid];
            format(gangData[gangid][gang_name], 90, "%s", dest);
            m_pquery_format("UPDATE es_gangs SET name='%s' WHERE id=%d LIMIT 1;", dest, gangid);
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zmieni³ nazwê na: {c}%s{/c}.", getGangTag(gangid), dest);
            
            UpdateGangBox(gangid);
            UpdateGangZones(gangid);
            return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");
        }

        case DIALOG_GANG_RETAG: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 2, 5)) return showDialog(playerid, DIALOG_GANG_RETAG, DIALOG_STYLE_INPUT, ""GOLD"Zmiana tagu zespo³u", ""WHITE"Wpisz poni¿ej nowy tag zespo³u:\n\n"RED"Dozwolona d³ugoœæ tagu to: "WHITE"2-5 "RED"znaków.", "Ustaw", "Cofnij");

            new gangid = pGang[playerid];
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zmieni³ tag na: {c}%s{/c}.", getGangTag(gangid), dest);
            format(gangData[gangid][gang_tag], 10, "%s", dest);
            m_pquery_format("UPDATE es_gangs SET tag='%s' WHERE id=%d LIMIT 1;", dest, gangid);
            
            UpdateGangBox(gangid);
            UpdateGangZones(gangid);
            return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");
        }

        case DIALOG_GANG_RECOLOR: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");

            if(listitem != INVALID_LISTITEM) {
                new gangid = pGang[playerid];
                gangData[gangid][gang_color] = KoloryGraczy[listitem];
                m_pquery_format("UPDATE es_gangs SET color=%d WHERE id=%d LIMIT 1;", KoloryGraczy[listitem], gangid);

                foreach(new i : GangMember[gangid]) { TextDrawHideForPlayer(i, gangData[gangid][gang_draw][0]), TextDrawHideForPlayer(i, gangData[gangid][gang_draw][1]); }
                TextDrawColor(gangData[gangid][gang_draw][0], getGangColor(gangid));
                TextDrawColor(gangData[gangid][gang_draw][1], getGangColor(gangid));
                
                foreach(new i : GangMember[gangid]) {
                    TextDrawShowForPlayer(i, gangData[gangid][gang_draw][0]), TextDrawShowForPlayer(i, gangData[gangid][gang_draw][1]);
                    msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zmieni³ swój kolor na taki, jak ta wiadomoœæ.", getGangTag(gangid));
                    UpdateGangBar(i, gangid);
                }

                UpdateGangBox(gangid);
                UpdateGangZones(gangid);
                OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SETTINGS, "");
            }
            return 1;
        }

        case DIALOG_GANG_INVITE_LIST: {
            if(!response) return 1;

            new gangid = pGangInvites[playerid][listitem];
            if(gangid == -1) return sendError(playerid, ""RED"W tym slocie nie ma ¿adnego zaproszenia.");

            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Czy chcesz do³¹czyæ do zespo³u: {%06x}%s [%s]"WHITE"?\n"WHITE"Wszystkie inne zaproszenia zostan¹ automatycznie odrzucone.", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
            inline onDialogInvite(pid, ddialogid, rresponse, llistitem, string:inputtexte[])
            {
                #pragma unused pid, ddialogid, llistitem, inputtexte
                if(!rresponse) {
                    pGangInvites[playerid][listitem] = -1;
                    KillTimer(pGangInviteTimer[playerid]);
                    
                    foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Gracz {c}%s(%d){/c} odrzuci³(a) zaproszenie do zespo³u.", getGangTag(gangid), getNick(playerid), playerid); }
                    msg(playerid, C_BLUE, "Odrzuci³eœ(aœ) zaproszenie od zespo³u: {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid));
                } else {
                    pGang[playerid] = gangid;
                    KillTimer(pGangInviteTimer[playerid]);

                    format(pGangRankName[playerid], 50, "%s", gangData[gangid][gang_rankDefault]);
                    pGangRankColor[playerid] = KoloryGraczy[random(sizeof(KoloryGraczy))];
                    for(new i = 0; i < 5; i++) pGangInvites[playerid][i] = -1;

                    new permstr[256];
                    for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                        pGangRankPerm[playerid][i] = false;
                        format(permstr, sizeof(permstr), "%s%s%d", permstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);
                    }
                    m_pquery_format("UPDATE es_accounts SET gang=%d, gang_rank='%s', gang_rank_color=%d, gang_rank_permission='%s' WHERE id=%d LIMIT 1;", pGang[playerid], pGangRankName[playerid], pGangRankColor[playerid], permstr, playerData[playerid][player_accountID]);
                    LoadPlayerGang(playerid, true);
                    msgAll(getGangColor(gangid), "Zespó³ {c}%s [%s]{/c} sprowadzi³ {c}%s(%d) {/c}za: {c}€%s{/c}.", getGangName(gangid), getGangTag(gangid), getNick(playerid), playerid, strdot(GetPlayerBudget(playerid)));
                }
            }

            Dialog_ShowCallback(playerid, using inline onDialogInvite, DIALOG_STYLE_MSGBOX, ""GOLD"Zaproszenie", s, "Akceptuj", "Odrzuæ");
            return 1;
        }

        case DIALOG_GANG_INVITE: {
            if(!response) return callcmd::team(playerid);

            new id;
            if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_GANG_INVITE, DIALOG_STYLE_INPUT, ""GREEN"Zaproœ do zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz zaprosiæ do zespo³u:\n\n"RED"Nieprawid³owe ID gracza.", "Wyœlij", "Cofnij");
            if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_GANG_INVITE, DIALOG_STYLE_INPUT, ""GREEN"Zaproœ do zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz zaprosiæ do zespo³u:\n\n"RED"Ten gracz nie jest po³¹czony z serwerem.", "Wyœlij", "Cofnij");
            if(pGang[id] != -1) return showDialog(playerid, DIALOG_GANG_INVITE, DIALOG_STYLE_INPUT, ""GREEN"Zaproœ do zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz zaprosiæ do zespo³u:\n\n"RED"Ten gracz jest ju¿ w jakimœ zespole.", "Wyœlij", "Cofnij");
            
            new gangid = pGang[playerid], count = 0;
            for(new i = 0; i < 5; i++) {
                if(pGangInvites[id][i] == -1) {
                    pGangInvites[id][i] = gangid;
                    break;
                }
                else count ++;
            }
            if(count >= 5) return showDialog(playerid, DIALOG_GANG_INVITE, DIALOG_STYLE_INPUT, ""GREEN"Zaproœ do zespo³u", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz zaprosiæ do zespo³u:\n\n"RED"Ten gracz nie ma wolnych slotów na zaproszenia.", "Wyœlij", "Cofnij");

            pGangInviteTimer[id] = SetTimerEx("DelayedGangInvite", 1000 * 60 * 3, false, "dd", id, gangid);
            msg(playerid, getGangColor(gangid), "Zaprosi³eœ(aœ) gracza {c}%s(%d){/c} do swojego zespo³u.", getNick(id), id);

            msg(id, getGangColor(gangid), "Otrzyma³eœ(aœ) zaproszenie do zespo³u: {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid));
            msg(id, getGangColor(gangid), "Masz {c}3 minuty{/c} na akceptacjê - komenda: {c}/tzaproszenia{/c}.");
            return 1;
        }

        case DIALOG_GANG_MEMBERS: {
            if(!response) return callcmd::team(playerid);
            return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_gangManage][listitem]));
        }
    }
    return 0;
}