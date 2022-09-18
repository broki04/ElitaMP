stock CountAdminList()
{
    new Cache:result = mysql_query(dbconn, "SELECT admin FROM es_accounts WHERE admin > 0"), count = 0;
    count = cache_num_rows();
    cache_delete(result);
    return count;
}

alias:apanel("adminpanel")
CMD:apanel(playerid)
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"LY"Rezultat\n";
    format(s, sizeof(s), "%s"WHITE"Rangi administracyjne\t"LY"%d %s\n", s, CountAdminList(), ConvertTimeFormat(CountAdminList(), "osób", "osoba", "osoby", "osób"));
    strcat(s, " \n");
    format(s, sizeof(s), "%s"WHITE"Mo¿liwoœæ prowadzenia napadów:\t%s\n", s, (bankHeistEnabled) ? (""GREEN"W£¥CZONE") : (""RED"WY£¥CZONE"));
    format(s, sizeof(s), "%s"WHITE"Wydarzenie rybackie:\t%s\n", s, (fishEvent) ? (""GREEN"W£¥CZONE") : (""RED"WY£¥CZONE"));
    format(s, sizeof(s), "%s"WHITE"Wydarzenie górnicze:\t%s\n", s, (oreEvent) ? (""GREEN"W£¥CZONE") : (""RED"WY£¥CZONE"));
    return showDialog(playerid, DIALOG_ADMIN_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""RED"Panel administratora", s, "Wybierz", "Zamknij"), 1;
}
flags:apanel(HEAD)

stock OnDialogAdminPanel(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_ADMIN_PANEL: {
            if(!response) return 1;

            switch(listitem) {
                case 1: callcmd::apanel(playerid);

                case 0: {
                    s = ""WHITE"Nick (userid)\t"RED"Ranga\t"ORANGE"Ostatnia wizyta\n";
                    strcat(s, ""RED"Nadaj rangê administracyjn¹\n");
                    strcat(s, " \n");

                    inline onFetchAdmins() {
                        new id, nick[25], admin, admin_level, date[50];
                        for(new i = 0; i < cache_num_rows(); i++) {
                            cache_get_value_name_int(i, "id", id);
                            cache_get_value_name(i, "nick", nick, sizeof(nick));
                            cache_get_value_name_int(i, "admin", admin);
                            cache_get_value_name_int(i, "admin_level", admin_level);
                            cache_get_value_name(i, "date_last", date, sizeof(date));
                        
                            format(s, sizeof(s), "%s"WHITE"%s (userid: %d)\t{%06x}%s (poz. %d)\t"ORANGE"%s\n", s, nick, id, getRankColor(admin) >>> 8, getRankName(admin), admin_level, date);
                            playerData[playerid][player_saveData][i] = id;
                            playerData[playerid][player_adminData][i] = admin;
                        }
                        showDialog(playerid, DIALOG_ADMIN_LIST, DIALOG_STYLE_TABLIST_HEADERS, ""RED"Administracja serwera", s, "Wybierz", "Cofnij");
                    }
                    m_pquery_inline(dbconn, using inline onFetchAdmins, "SELECT * FROM es_accounts WHERE admin > 0 ORDER BY admin DESC");
                }

                case 2: {
                    bankHeistEnabled = !bankHeistEnabled;
                    heist_kurwa = bankHeistVaultOpen = bankHeistGas = false;
                    
                    bankHeistHacker = -1;
                    KillTimer(bankHeistTimer);

                    new str[128]; format(str, sizeof(str), ""ORANGE"WEJŒCIE DO SKARBCA\n"WHITE"Aktualnie %s "WHITE"wejœæ!", (bankHeistEnabled) ? (""GREEN"mo¿na") : (""RED"nie mo¿na"));
                    UpdateDynamic3DTextLabelText(bankHeistLabel, -1, str);

                    msgAll(C_LRED, "%s {c}%s(%d){/c} %s mo¿liwoœæ prowadzenia napadów na {c}/bank{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, (bankHeistEnabled) ? ("w³¹czy³(a)") : ("wy³¹czy³(a)"));
                    callcmd::apanel(playerid);
                }

                case 3: {
                    fishEvent = !fishEvent;
                    msgAll(C_LRED, "%s {c}%s(%d){/c} %s wydarzenie rybackie. "LY"(/fishEvent)", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, (fishEvent) ? ("aktywowa³(a)") : ("zdezaktywowa³(a)"));
                    callcmd::apanel(playerid);
                }

                case 4: {
                    oreEvent = !oreEvent;
                    msgAll(C_LRED, "%s {c}%s(%d){/c} %s wydarzenie górnicze. "LY"(/oreEvent)", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, (oreEvent) ? ("aktywowa³(a)") : ("zdezaktywowa³(a)"));
                    callcmd::apanel(playerid);
                }
            }
        }

        case DIALOG_ADMIN_LIST: {
            if(!response) return callcmd::apanel(playerid);

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_ADMIN_SELECT, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie rangi", ""WHITE"Wpisz poni¿ej ID gracza/konta, któremu chcesz nadaæ rangê:", "Dalej", "Cofnij");
                case 1: OnDialogResponse(playerid, DIALOG_ADMIN_PANEL, 1, 0, "");
                default: {
                    new userid = playerData[playerid][player_saveData][listitem - 2], admin = playerData[playerid][player_adminData][listitem - 2];
                    playerData[playerid][player_saveData][0] = userid;

                    if(userid == playerData[playerid][player_accountID] && playerData[playerid][player_accountID] != 1) return dialogBox(playerid, C_RED, ""RED"Nie mo¿esz edytowaæ swojej rangi!");
                    if(admin >= playerData[playerid][player_admin] && playerData[playerid][player_accountID] != 1) return dialogBox(playerid, C_RED, ""RED"Nie mo¿esz edytowaæ osoby, która posiada rangê wy¿sz¹/tak¹, jak Ty!");

                    s[0] = EOS;
                    for(new i = 0; i < 5; i++) format(s, sizeof(s), "%s{%06x}%s\n", s, getRankColor(i) >>> 8, getRankName(i));
                    showDialog(playerid, DIALOG_ADMIN_PERMISSION, DIALOG_STYLE_LIST, ""RED"Wybierz rangê administracyjn¹", s, "Ustaw", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_ADMIN_SELECT: {
            if(!response) return OnDialogResponse(playerid, DIALOG_ADMIN_PANEL, 1, 0, "");

            new id;
            if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_ADMIN_SELECT, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie rangi", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz nadaæ rangê:\n\n"RED"Podano nieprawid³owe ID.", "Dalej", "Cofnij");
            if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_ADMIN_SELECT, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie rangi", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz nadaæ rangê:\n\n"RED"Ten gracz nie jest na serwerze.", "Dalej", "Cofnij");
            if(playerData[id][player_admin] >= playerData[playerid][player_admin] && playerData[playerid][player_accountID] != 1) return showDialog(playerid, DIALOG_ADMIN_SELECT, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie rangi", ""WHITE"Wpisz poni¿ej ID gracza/konta, któremu chcesz nadaæ rangê:\n\n"RED"Nie mo¿esz edytowaæ osoby, która posiada rangê wy¿sz¹/tak¹, jak Ty!", "Dalej", "Cofnij");
            if(playerData[id][player_accountID] == playerData[playerid][player_accountID] && playerData[playerid][player_accountID] != 1) return showDialog(playerid, DIALOG_ADMIN_SELECT, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie rangi", ""WHITE"Wpisz poni¿ej ID gracza/konta, któremu chcesz nadaæ rangê:\n\n"RED"Nie mo¿esz edytowaæ swojej rangi.", "Dalej", "Cofnij");
            playerData[playerid][player_saveData][0] = playerData[id][player_accountID];

            s[0] = EOS;
            for(new i = 0; i < 5; i++) format(s, sizeof(s), "%s{%06x}%s\n", s, getRankColor(i) >>> 8, getRankName(i));
            return showDialog(playerid, DIALOG_ADMIN_PERMISSION, DIALOG_STYLE_LIST, ""RED"Wybierz rangê administracyjn¹", s, "Ustaw", "Cofnij"), 1;
        }

        case DIALOG_ADMIN_PERMISSION: {
            if(!response) return OnDialogResponse(playerid, DIALOG_ADMIN_PANEL, 1, 0, "");
            SetPVarInt(playerid, "panel_admin_selected", listitem);

            if(listitem == 0) {
                new userid = playerData[playerid][player_saveData][0], nick[25];
                new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", userid));
                if(cache_num_rows() > 0) cache_get_value_name(0, "nick", nick, sizeof(nick));
                cache_delete(result);
            
                inline onEditAdminRank() 
                {
                    new i = CheckPlayerOnline(userid);
                    if(i != -1) {
                        playerData[i][player_admin] = playerData[i][player_adminLoad] = playerData[i][player_adminLevel] = 0;
                        if(Iter_Contains(Permission, i)) Iter_Remove(Permission, i);
                        format(playerData[i][player_adminPass], 50, "@NULL");
                        msg(i, C_MINT, "%s {c}%s(%d){/c} zabra³(a) Ci rangê administracyjn¹.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid);
                    }
                    msg(playerid, C_MINT, "Zabra³eœ(aœ) graczowi {c}%s{/c} rangê administracyjn¹.", nick);
                }
                m_pquery_inline(dbconn, using inline onEditAdminRank, "UPDATE es_accounts SET admin=0, admin_level=0, admin_pass='@NULL' WHERE id=%d LIMIT 1;", userid);
            }
            else showDialog(playerid, DIALOG_ADMIN_LEVEL, DIALOG_STYLE_LIST, ""RED"Wybierz poziom rangi administracyjnej", ""VIOLET2"Poziom rangi: "WHITE"1\n"VIOLET2"Poziom rangi: "WHITE"2", "Ustaw", "Cofnij");
            return 1;
        }

        case DIALOG_ADMIN_LEVEL: {
            if(!response) return OnDialogResponse(playerid, DIALOG_ADMIN_LIST, 1, playerData[playerid][player_saveData][0], "");

            new userid = playerData[playerid][player_saveData][0], admin = GetPVarInt(playerid, "panel_admin_selected"), admin_level = listitem + 1, nick[25];
            new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", userid));
            if(cache_num_rows() > 0) cache_get_value_name(0, "nick", nick, sizeof(nick));
            cache_delete(result);

            inline onEditAdminRank() 
            {
                new i = CheckPlayerOnline(userid);
                if(i != -1) {
                    playerData[i][player_adminLoad] = admin;
                    playerData[i][player_adminLevel] = admin_level;

                    if(strmatch(playerData[i][player_adminPass], "@NULL", true)) return showDialog(i, DIALOG_ADMIN_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Ustawianie has³a na s³u¿bê", ""WHITE"Wpisz poni¿ej has³o, którym bêdziesz siê logowaæ na rangê administracyjn¹.", "Ustaw", "Anuluj");
                    msg(i, getRankColor(admin), "%s {c}%s(%d){/c} ustawi³(a) Ci rangê: {c}%s (poz. %d){/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, getRankName(admin), admin_level);
                }
                msg(playerid, getRankColor(admin), "Ustawi³eœ(aœ) {c}%s{/c} rangê: {c}%s (poz. %d){/c}.", nick, getRankName(admin), admin_level);
            }
            m_pquery_inline(dbconn, using inline onEditAdminRank, "UPDATE es_accounts SET admin=%d, admin_level=%d WHERE id=%d LIMIT 1;", admin, admin_level, userid);
            return 1;
        }
    }
    return 0;
}