stock ShowGangList(playerid)
{   
    s[0] = EOS;

    new x = 0;
    foreach(new gangid : Gangs) {
        AddDialogListitem(playerid, ""WHITE"»  {%06x}%s [%s]\n", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
        playerData[playerid][player_gangData][x++] = gangid; 
    }
    return showDialog(playerid, DIALOG_GANG_LIST, DIALOG_STYLE_LIST, ""GOLD"Lista zespo³ów", #, "Wybierz", "Cofnij", 10), 1;
}

stock Task:ShowGangInformation(playerid, gangid)
{
    new Task:t = task_new();
    inline onGangInfo()
    {
        s[0] = EOS;
        s = ""WHITE"Informacja\t"BLUE"Wynik\n";
        format(s, sizeof(s), "%s"WHITE"Nazwa zespo³u:\t{%06x}%s [%s]\n", s, getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));

        new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", gangData[gangid][gang_ownerid]));
        if(cache_num_rows() > 0) {
            new nick[25]; cache_get_value(0, 0, nick, sizeof(nick));
            format(s, sizeof(s), "%s"WHITE"W³aœciciel zespo³u:\t"BLUE"%s (userid: %d)\n", s, nick, gangData[gangid][gang_ownerid]);
        }
        cache_delete(result);

        strcat(s, " \n");
        format(s, sizeof(s), "%s"WHITE"Poziom zespo³u:\t"BROWN"%s poziom (%s/%s exp)\n", s, strdot(gangData[gangid][gang_level]), strdot(gangData[gangid][gang_respect]), strdot(gangData[gangid][gang_respect_needed]));
        format(s, sizeof(s), "%s"WHITE"Ranga na sparingach:\t"LRED"%s (%d/%d)\n", s, getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], getRankingPoints(pGangSparRank[gangid]));
        format(s, sizeof(s), "%s"WHITE"Bilans sparingów:\t"RED"W: %d, L: %d, R: %d\n", s, gangData[gangid][gang_spar_win], gangData[gangid][gang_spar_loss], gangData[gangid][gang_spar_remis]);
        format(s, sizeof(s), "%s"WHITE"Statystyki bitewne:\t"LBLUE"K: %s, D: %s "LY"(%.1f)\n", s, strdot(gangData[gangid][gang_kills]), strdot(gangData[gangid][gang_deaths]), getRatio(gangData[gangid][gang_kills], gangData[gangid][gang_deaths]));
        strcat(s, " \n");
        strcat(s, ""WHITE"»  "LY"Kliknij, aby zobaczyæ przejête terytoria\n");
        strcat(s, ""WHITE"»  "LY"Kliknij, aby zobaczyæ cz³onków zespo³u\n");

        inline onDialogInfo(pid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused pid, dialogid, inputtext
            if(!response) return callcmd::team(playerid), t;

            switch(listitem) {
                case 0..7: task_await(ShowGangInformation(playerid, gangid));
                case 8: {
                    s[0] = EOS;
                    strcat(s, ""BLUE"Terytoria przejête przez ten gang:\n");

                    new count = 0;
                    foreach(new i : GangZones) { if(pGangZoneOwner[i] == gangid) { count ++; format(s, sizeof(s), "%s"WHITE"»  "YELLOW"%s "LY"(#%d)\n", s, getZoneName(unpackXYZ(pGangZonePoint[i])), i + 1); } }
                    if(count < 1) return dialogBox(playerid, C_BLUE, ""BLUE"Ten zespó³ nie posiada zajêtego terytorium."), t;

                    inline onGangTeren(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return task_await(ShowGangInformation(playerid, gangid)), t; } 
                    Dialog_ShowCallback(playerid, using inline onGangTeren, DIALOG_STYLE_LIST, ""BLUE"Zajête terytoria", s, "OK", #);
                }
                case 9: {
                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "SELECT * FROM es_accounts WHERE gang=%d LIMIT %d;", gangid, gangData[gangid][gang_slots]);
                    m_tquery(dbstr, "ShowPlayerGangMembers", "dd", playerid, gangid);
                }
            }
            return t;
        }
        Dialog_ShowCallback(playerid, using inline onDialogInfo, DIALOG_STYLE_TABLIST_HEADERS, form("{%06x}%s [%s]", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid)), s, "Wybierz", "Cofnij");
    }
    playerData[playerid][player_gangData][0] = gangid;

    m_tquery_inline(dbconn, using inline onGangInfo, "SELECT * FROM es_gangs WHERE id=%d LIMIT 1;", gangid);
    return t;
}

function ShowPlayerGangMembers(playerid, gangid)
{
    if(!Iter_Contains(Gangs, gangid)) return 0;

    s[0] = EOS;
    s = ""BLACK"X\t"WHITE"Nick\t"BLUE"Ranga\n";

    new nick[25], rank_name[50], rank_color;
    for(new i = 0; i < cache_num_rows(); i++) {
        cache_get_value_name(i, "nick", nick, 25);
        cache_get_value_name(i, "gang_rank", rank_name);
        cache_get_value_name_int(i, "gang_rank_color", rank_color);

        format(s, sizeof(s), "%s"BLACK"%d.\t{%06x}%s\t{%06x}%s\n", s, i + 1, getGangColor(gangid) >>> 8, nick, rank_color >>> 8, rank_name);
    }

    inline onDialogCallback(pid, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused pid, dialogid, response, listitem, inputtext
        return task_await(ShowGangInformation(playerid, gangid));
    }
    return Dialog_ShowCallback(playerid, using inline onDialogCallback, DIALOG_STYLE_LIST, ""BLUE"Cz³onkowie zespo³u", s, "Cofnij", #), 1;
}