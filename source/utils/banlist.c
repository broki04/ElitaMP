CMD:banlist(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
   
    inline onCheckBanList()
    {
        if(!cache_num_rows()) return dialogBox(playerid, C_RED, ""RED"Nikt nie jest zbanowany!");
        AddDialogListitem(playerid, ""RED"Osoba banuj¹ca\t"LY"Osoba zbanowana\t"LBLUE"Typ\t"YELLOW"Powód\n");

        new id, userid, adminid, reason[90], date[50], ip[18], serial[65], expire, x = 0;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", id);
            cache_get_value_name_int(i, "userid", userid);
            cache_get_value_name_int(i, "adminid", adminid);
            cache_get_value_name(i, "reason", reason, 90);
            cache_get_value_name(i, "date", date, 50);
            cache_get_value_name(i, "ip", ip, 18);
            cache_get_value_name(i, "gpci", serial, 65);
            cache_get_value_name_int(i, "expire", expire);

            new dd, hh, mm, ss;
            if(expire != -1) timediff(expire, dd, hh, mm, ss);

            new anick[25], Cache:a_result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", adminid));
            if(cache_num_rows() > 0) cache_get_value(0, 0, anick, sizeof(anick));
            cache_delete(a_result);

            new bnick[25], Cache:b_result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", userid));
            if(cache_num_rows() > 0) cache_get_value(0, 0, bnick, sizeof(bnick));
            cache_delete(b_result);

            AddDialogListitem(playerid, ""RED"%s\t"LY"%s\t"LBLUE"%s\t"YELLOW"%s\n", anick, bnick, (expire == -1) ? ("Permanentny") : ("Czasowy"), reason);
            playerData[playerid][player_banData][x++] = id;
        }
        showDialog(playerid, DIALOG_BANLIST, DIALOG_STYLE_TABLIST_HEADERS, form(""RED"Lista zbanowanych: "WHITE"%d", cache_num_rows()), #, "Wybierz", "Zamknij", 10);
    }
    m_tquery_inline(dbconn, using inline onCheckBanList, "SELECT * FROM es_bans");
    return 1;
}
flags:banlist(VICE)

stock Task:ShowBanManage(playerid, bannedid)
{
    new Task:t = task_new();
    inline onShowBanData()
    {
        if(cache_num_rows() > 0) {
            new userid, adminid, reason[50], ip[18], serial[65], expire, date[50];
            cache_get_value_name_int(0, "userid", userid);
            cache_get_value_name_int(0, "adminid", adminid);
            cache_get_value_name(0, "reason", reason, sizeof(reason));
            cache_get_value_name(0, "ip", ip, sizeof(ip));
            cache_get_value_name(0, "gpci", serial, sizeof(serial));
            cache_get_value_name_int(0, "expire", expire);
            cache_get_value_name(0, "date", date, sizeof(date));

            playerData[playerid][player_saveData][0] = bannedid;

            new anick[25], Cache:a_result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", adminid));
            if(cache_num_rows() > 0) cache_get_value(0, 0, anick, sizeof(anick));
            cache_delete(a_result);

            new bnick[25], Cache:b_result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", userid));
            if(cache_num_rows() > 0) cache_get_value(0, 0, bnick, sizeof(bnick));
            cache_delete(b_result);

            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Nick osoby zbanowanej: "BLUE"%s (userid: %d)\n", bnick, userid);
            format(s, sizeof(s), "%s"WHITE"Nick admina banuj¹cego: "RED"%s (userid: %d)\n", s, anick, adminid);
            format(s, sizeof(s), "%s"WHITE"D³ugoœæ banicji: "ORANGE"%s\n", s, (expire != -1) ? (form("Czasowy (%s)", ConvertTime(expire, TIME_TYPE_UNIX))) : ("Permanentny"));
            format(s, sizeof(s), "%s"WHITE"Powód banicji: "VIOLET2"%s\n", s, reason);
            strcat(s, " \n");
            strcat(s, ""LRED"»  "WHITE"Zmieñ powód bana\n");
            strcat(s, ""LRED"»  "WHITE"Zmieñ d³ugoœæ bana\n");
            strcat(s, ""LRED"»  "WHITE"Usuñ bana\n");

            new title[128]; 
            format(title, sizeof(title), ""DRED"Banicja: "WHITE"%s (userid: %d)", bnick, userid);
            showDialog(playerid, DIALOG_BAN_MANAGE, DIALOG_STYLE_LIST, title, s, "Wybierz", "Cofnij");
        }
    }
    m_tquery_inline(dbconn, using inline onShowBanData, "SELECT * FROM es_bans WHERE id=%d LIMIT 1;", bannedid);
    return t;
}

stock OnDialogBanList(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BANLIST: {
            if(!response) return 1;
            if(listitem != INVALID_LISTITEM) return task_await(ShowBanManage(playerid, playerData[playerid][player_banData][listitem]));
        }

        case DIALOG_BAN_REASON: {
            if(!response) return task_await(ShowBanManage(playerid, playerData[playerid][player_saveData][0]));

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 3 || strlen(dest) > 20) return showDialog(playerid, DIALOG_BAN_REASON, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ powód bana", ""WHITE"Wpisz poni¿ej nowy powód bana:\n\n"RED"Dozwolona liczba znaków w powodzie to: "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");
            
            inline onUpdateBan() return task_await(ShowBanManage(playerid, playerData[playerid][player_saveData][0]));
            m_tquery_inline(dbconn, using inline onUpdateBan, "UPDATE es_bans SET reason='%s' WHERE id=%d LIMIT 1;", dest, playerData[playerid][player_saveData][0]);
            return 1;
        }

        case DIALOG_BAN_EXPIRE: {
            if(!response) return task_await(ShowBanManage(playerid, playerData[playerid][player_saveData][0]));

            new time;
            if(sscanf(inputtext, "d", time)) return showDialog(playerid, DIALOG_BAN_EXPIRE, DIALOG_STYLE_INPUT, ""LBLUE"Zmieñ d³ugoœæ bana", ""WHITE"Wpisz poni¿ej, jak d³ugo ma trwaæ ban (-1=perm):\n\n"RED"Czas podaje siê liczbami.", "Zmieñ", "Cofnij");
            if(time != -1 && (time < 1 || time > 30)) return showDialog(playerid, DIALOG_BAN_EXPIRE, DIALOG_STYLE_INPUT, ""LBLUE"Zmieñ d³ugoœæ bana", ""WHITE"Wpisz poni¿ej, jak d³ugo ma trwaæ ban (-1=perm):\n\n"RED"Dozwolony przedzia³ czasowy bana to: "WHITE"1-30 "RED"dni.", "Zmieñ", "Cofnij");
            
            inline onUpdateBan() return task_await(ShowBanManage(playerid, playerData[playerid][player_saveData][0]));
            m_tquery_inline(dbconn, using inline onUpdateBan, "UPDATE es_bans SET expire=%d WHERE id=%d LIMIT 1;", (time == -1) ? (-1) : (gettime() + UnixTime('d', time)), playerData[playerid][player_saveData][0]);
            return 1;
        }

        case DIALOG_BAN_MANAGE: {
            if(!response) return callcmd::banlist(playerid);

            new bid = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 0..4: task_await(ShowBanManage(playerid, bid));

                case 5: showDialog(playerid, DIALOG_BAN_REASON, DIALOG_STYLE_INPUT, ""YELLOW"Zmieñ powód bana", ""WHITE"Wpisz poni¿ej nowy powód bana:", "Ustaw", "Cofnij");
                case 6: showDialog(playerid, DIALOG_BAN_EXPIRE, DIALOG_STYLE_INPUT, ""LBLUE"Zmieñ d³ugoœæ bana", ""WHITE"Wpisz poni¿ej, jak d³ugo ma trwaæ ban (-1=perm):", "Zmieñ", "Cofnij");
                case 7: {
                    inline onDeleteBan() return callcmd::banlist(playerid);
                    m_pquery_inline(dbconn, using inline onDeleteBan, "DELETE FROM es_bans WHERE id=%d LIMIT 1;", bid);
                }
            }
            return 1;
        }
    }
    return 0;
}
