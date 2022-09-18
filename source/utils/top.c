#include YSI_Coding\y_hooks

alias:top("top10")
CMD:top(playerid)
{
    s[0] = EOS;
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"gotówki\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"poziomu postaci\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"liczby zabójstw\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"liczby œmierci\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"przegranego czasu\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"wizyt na serwerze\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"rozwi¹zanych testów reakcji\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"znalezionych dyskietek\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"znalezionych figurek\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"wygranych dueli\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"zaliczonych zadañ dnia\n");
    strcat(s, ""WHITE"»  "BLUE2"Ranking wed³ug: "WHITE"najszybszego czasu na "LY"/tgl\n");
    return showDialog(playerid, DIALOG_RANKING, DIALOG_STYLE_LIST, ""ORANGE"Ranking "WHITE"TOP 10", s, "Wybierz", "Zamknij");
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_RANKING: {
            if(!response) return 1;
            inline onFetchTop()
            {
                s[0] = EOS;
                s = ""BLACK"ID\t"WHITE"Nick\t"CORAL"Wynik\n";

                new id, nick[25], result, Float:result_float;
                for(new i = 0; i < cache_num_rows(); i++) {
                    cache_get_value_int(i, 0, id);
                    cache_get_value(i, 1, nick, 25);
                    cache_get_value_int(i, 2, result);
                    cache_get_value_name_float(i, "glitch", result_float);

                    switch(listitem) {
                        case 0: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s$\n", s, i + 1, nick, strdot(result));
                        case 1: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s poziom\n", s, i + 1, nick, strdot(result));
                        case 2: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s zabójstw\n", s, i + 1, nick, strdot(result));
                        case 3: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s œmierci\n", s, i + 1, nick, strdot(result));
                        case 4: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%02dh %02dmin\n", s, i + 1, nick, floatround((result / 3600)), floatround((result / 60) % 60));
                        case 5: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s wizyt\n", s, i + 1, nick, strdot(result));
                        case 11: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%.3fs\n", s, i + 1, nick, result_float);
                        default: format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s\n", s, i + 1, nick, strdot(result));
                    }
                }

                inline onTopDialog(pp, dd, rr, ll, string:ii[]) {
                    #pragma unused pp, dd, rr, ll, ii
                    return callcmd::top(playerid); }
                new title[128]; format(title, sizeof(title), ""BLUE"Ranking wed³ug:"WHITE"%s", inputtext[18]);
                return Dialog_ShowCallback(playerid, using inline onTopDialog, DIALOG_STYLE_TABLIST_HEADERS, title, s, "OK", #), 1;
            }

            dbstr[0] = EOS;
            switch(listitem) {
                case 0: format(dbstr, sizeof(dbstr), "SELECT id, nick, money FROM es_accounts ORDER BY money DESC LIMIT 10;");
                case 1: format(dbstr, sizeof(dbstr), "SELECT id, nick, level FROM es_accounts ORDER BY level DESC LIMIT 10;");
                case 2: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_KILLS + 1);
                case 3: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_DEATHS + 1);
                case 4: format(dbstr, sizeof(dbstr), "SELECT id, nick, session FROM es_accounts ORDER BY session DESC LIMIT 10;");
                case 5: format(dbstr, sizeof(dbstr), "SELECT id, nick, visits FROM es_accounts ORDER BY visits DESC LIMIT 10;");
                case 6: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_QUIZ + 1);
                case 7: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_DISKS + 1);
                case 8: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_FIGURKA + 1);
                case 9: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_DUEL + 1);
                case 10: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_QUEST + 1);
                case 11: format(dbstr, sizeof(dbstr), "SELECT id, nick, glitch FROM es_accounts ORDER BY glitch ASC LIMIT 10;");
            }
            m_pquery_inline(dbconn, using inline onFetchTop, dbstr);
            return 1;
        }
    }
    return 0;
}
