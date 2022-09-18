stock Task:GangMemberEdit(playerid, gangid, userid) 
{
    new Task:t = task_new();
    inline onPlayerManage()
    {
        if(userid == gangData[gangid][gang_ownerid] && playerData[playerid][player_accountID] != userid) return dialogBox(playerid, C_RED, ""RED"Nie mo�esz zarz�dza� za�o�ycielem zespo�u."), t;

        new rank_name[50], rank_color, nick[25];
        if(cache_num_rows() > 0) {
            cache_get_value_name(0, "nick", nick);
            cache_get_value_name(0, "gang_rank", rank_name);
            cache_get_value_name_int(0, "gang_rank_color", rank_color);

            s[0] = EOS;
            format(s, sizeof(s), "{%06x}�  "WHITE"Zmie� nazw� rangi: {%06x}%s\n", getGangColor(gangid) >>> 8, rank_color >>> 8, rank_name);
            format(s, sizeof(s), "%s{%06x}�  "WHITE"Zmie� kolor rangi: {%06x}%s\n", s, getGangColor(gangid) >>> 8, rank_color >>> 8, rank_name);
            format(s, sizeof(s), "%s{%06x}�  "WHITE"Zarz�dzaj uprawnieniami\n", s, getGangColor(gangid) >>> 8);
            format(s, sizeof(s), "%s{%06x}�  "WHITE"Wyrzu� z zespo�u\n", s, getGangColor(gangid) >>> 8);
            showDialog(playerid, DIALOG_GANG_MANAGE_MEMBER, DIALOG_STYLE_LIST, form(""GREEN"Zarz�dzanie graczem "WHITE"%s", nick), s, "Wybierz", "Cofnij");

            playerData[playerid][player_saveData][0] = userid;
        }
    }

    m_pquery_inline(dbconn, using inline onPlayerManage, "SELECT * FROM es_accounts WHERE id=%d LIMIT 1;", userid);
    return t;
}

stock OnDialogGangMember(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_GANG_RANK_NAME: {
            if(!response) return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 3, 15)) return showDialog(playerid, DIALOG_GANG_RANK_NAME, DIALOG_STYLE_INPUT, ""GOLD"Zmiana nazwy rangi", ""WHITE"Wpisz poni�ej now� nazw� rangi:\n\n"RED"Wprowadzono niedozwolone znaki, b�d� przekroczono limit znak�w: "WHITE"3-15.", "Ustaw", "Cofnij");

            new i = playerData[playerid][player_saveData][0];
            inline onEditPlayer()
            {
                new nick[25], id = CheckPlayerOnline(i);
                if(id != -1) {
                    format(nick, sizeof(nick), "%s(%d)", getNick(id), id);
                    format(pGangRankName[id], 50, "%s", dest);
                    msg(id, C_BLUE, "%s {c}%s(%d){/c} zmieni�(a) nazw� Twojej rangi na: {c}%s{/c}.", pGangRankName[playerid], getNick(playerid), playerid, dest);
                } else {
                    new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", i));
                    if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
                    cache_delete(result);
                }

                msg(playerid, C_BLUE, "Zmieni�e�(a�) nazw� rangi gracza {c}%s{/c} na: {c}%s{/c}.", nick, dest);
                return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_saveData][0])), 1;
            }

            m_pquery_inline(dbconn, using inline onEditPlayer, "UPDATE es_accounts SET gang_rank='%s' WHERE id=%d LIMIT 1;", dest, i);
        }

        case DIALOG_GANG_MANAGE_MEMBER: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_MANAGE, "");

            new userid = playerData[playerid][player_saveData][0], gangid = pGang[playerid];
            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_GANG_RANK_NAME, DIALOG_STYLE_INPUT, ""GOLD"Zmiana nazwy rangi", ""WHITE"Wpisz poni�ej now� nazw� rangi:", "Ustaw", "Cofnij");
                case 1: {
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
                    showDialog(playerid, DIALOG_GANG_RANK_COLOR, DIALOG_STYLE_LIST, ""RED"Zmiana koloru rangi", #, "Ustaw", "Cofnij", 10);
                }

                case 2: {
                    if(userid == gangData[gangid][gang_ownerid]) return dialogBox(playerid, C_RED, ""RED"Nie mo�esz zarz�dza� swoimi uprawnieniami.");

                    inline onMemberPermission()
                    {
                        new x = 0;
                        if(cache_num_rows() > 0) {
                            new buffer[90], bool:rank_perms[sizeof(gangRankPermissions)];
                            cache_get_value_name(0, "gang_rank_permission", buffer);
                            sscanf(buffer, form("p<,>a<d>[%d]", sizeof(gangRankPermissions)), rank_perms);

                            s[0] = EOS;
                            s = ""WHITE"Nazwa\t"GREEN"Sta"RED"tus\n";
                        
                            for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                                format(s, sizeof(s), "%s"WHITE"%s\t%s\n", s, gangRankPermissions[i], (rank_perms[i]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
                                playerData[playerid][player_gangManage][x++] = rank_perms[i];
                            }
                            showDialog(playerid, DIALOG_GANG_RANK_PERMS, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Uprawnienia", s, "Prze��cz", "Cofnij");
                        }
                    }
                    m_pquery_inline(dbconn, using inline onMemberPermission, "SELECT gang_rank_permission FROM es_accounts WHERE id=%d LIMIT 1;", userid);
                }

                case 3: {
                    if(userid == gangData[gangid][gang_ownerid]) return dialogBox(playerid, C_RED, ""RED"Nie mo�esz wyrzuci� za�o�yciela zespo�u.");

                    inline onEditPlayer()
                    {
                        new id = CheckPlayerOnline(userid), nick[25];
                        if(id != -1) {
                            format(nick, sizeof(nick), "%s(%d)", getNick(id), id);
                            LeaveGang(id, gangid, true);
                            msg(id, getGangColor(gangid), "Zosta�e�(a�) wyrzucony(a) z zespo�u {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid));
                        } else {
                            new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", userid));
                            if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
                            cache_delete(result);
                        }
                        msg(playerid, getGangColor(gangid), "Wyrzuci�e�(a�) gracza {c}%s{/c} z zespo�u.", nick);
                    }
                    m_pquery_inline(dbconn, using inline onEditPlayer, "UPDATE es_accounts SET gang=-1 WHERE id=%d LIMIT 1;", userid);
                }
            }
            return 1;
        }

        case DIALOG_GANG_RANK_COLOR: {
            if(!response) return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new i = playerData[playerid][player_saveData][0];
            if(listitem != INVALID_LISTITEM) {
                inline onEditPlayer()
                {
                    new id = CheckPlayerOnline(i), nick[25];
                    if(id != -1) {
                        pGangRankColor[id] = KoloryGraczy[listitem];
                        msg(id, KoloryGraczy[listitem], "%s {c}%s(%d){/c} zmieni�(a) kolor Twojej zespo�owej rangi na taki, jak ta wiadomo��.", pGangRankName[playerid], getNick(playerid), playerid);
                    } else {
                        new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", i));
                        if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
                        cache_delete(result);
                    }

                    msg(playerid, KoloryGraczy[listitem], "Zmieni�e�(a�) kolor zespo�owej rangi graczowi {c}%s{/c} na taki, jak ta wiadomo��.", nick);
                    return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_saveData][0])), 1;
                }
                m_pquery_inline(dbconn, using inline onEditPlayer, "UPDATE es_accounts SET gang_rank_color=%d WHERE id=%d LIMIT 1;", KoloryGraczy[listitem], i);
            }
            return 1;
        }

        case DIALOG_GANG_RANK_PERMS: {
            if(!response) return task_await(GangMemberEdit(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new userid = playerData[playerid][player_saveData][0];            
            playerData[playerid][player_gangManage][listitem] = !playerData[playerid][player_gangManage][listitem];

            new permstr[256];
            for(new i = 0; i < sizeof(gangRankPermissions); i++) format(permstr, sizeof(permstr), "%s%s%d", permstr, i == 0 ? ("") : (","), playerData[playerid][player_gangManage][i]);

            inline onEditPlayer()
            {
                new id = CheckPlayerOnline(userid);
                if(id != -1) {
                    pGangRankPerm[id][listitem] = !pGangRankPerm[id][listitem];
                    msg(id, C_YELLOW, "%s {c}%s(%d){/c} %s uprawnienie do: {c}\"%s\".", pGangRankName[playerid], getNick(playerid), playerid, (pGangRankPerm[id][listitem]) ? ("da�(a)") : ("zabra�(a)"), gangRankPermissions[listitem]);
                }
                return OnDialogResponse(playerid, DIALOG_GANG_MANAGE_MEMBER, 1, 2, ""), 1;
            }

            m_pquery_inline(dbconn, using inline onEditPlayer, "UPDATE es_accounts SET gang_rank_permission='%s' WHERE id=%d LIMIT 1;", permstr, userid);
        }
    }
    return 0;
}
