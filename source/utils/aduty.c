stock ShowAdminLogin(playerid, const string[])
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Posiadasz rangê administracyjn¹: {%06x}%s"WHITE".\n"WHITE"Wpisz poni¿ej has³o, które ustawi³eœ(aœ), by siê zalogowaæ:", getRankColor(playerData[playerid][player_adminLoad]) >>> 8, getRankName(playerData[playerid][player_adminLoad]));
    if(strlen(string) > 2) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_ADMIN_LOGIN, DIALOG_STYLE_PASSWORD, ""RED"Logowanie na administratora", s, "Zaloguj", "Anuluj"), 1;
}

alias:aduty("alogin", "sluzba")
CMD:aduty(playerid)
{
    if(playerData[playerid][player_adminLoad] < MODERATOR) return dialogBox(playerid, C_RED, ""RED"Nie posiadasz uprawnieñ, by móc zalogowaæ siê na rangê administracyjn¹.");
    if(strmatch(playerData[playerid][player_adminPass], "@NULL", true)) return showDialog(playerid, DIALOG_ADMIN_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Ustawianie has³a na s³u¿bê", ""WHITE"Wpisz poni¿ej has³o, którym bêdziesz siê logowaæ na rangê administracyjn¹.", "Ustaw", "Anuluj");

    if(!Iter_Contains(Permission, playerid)) ShowAdminLogin(playerid, "");
    else {
        new hours = floatround(((playerData[playerid][player_online] + playerData[playerid][player_adminTime]) / 3600));
        new minutes = floatround(((playerData[playerid][player_online] + playerData[playerid][player_adminTime]) / 60) % 60);

        s[0] = EOS; 
        format(s, sizeof(s), ""WHITE"Twoja ranga:   \t\t{%06x}%s "WHITE"(poziom: %d)\n", getRankColor(playerData[playerid][player_admin]) >>> 8, getRankName(playerData[playerid][player_admin]), playerData[playerid][player_adminLevel]);
        format(s, sizeof(s), "%s"WHITE"Przegrany czas: \t{%06x}%02dh %02dmin\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8, hours, minutes);
        strcat(s, " \n");
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Wyloguj siê z rangi\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8);
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Zmieñ has³o na rangê\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8);
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Statystyki administracyjne\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8);
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Ranking administratorów\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8);
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Fa³szywy nick\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8);
        strcat(s, " \n");
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Wiadomoœci osób wyciszonych: %s\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8, (playerData[playerid][player_checkMuted]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Wyœwietlanie prywatnych wiadomoœci: %s\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8, (playerData[playerid][player_checkPM]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
        format(s, sizeof(s), "%s{%06x}»  "WHITE"Podgl¹d wpisywanych komend: %s\n", s, getRankColor(playerData[playerid][player_admin]) >>> 8, (playerData[playerid][player_checkCMD]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
        showDialog(playerid, DIALOG_ADMIN_DUTY, DIALOG_STYLE_LIST, ""RED"Ranga administracyjna", s, "Wybierz", "Zamknij");
    }
    return 1;
}

stock OnDialogAdminDuty(playerid, dialogid, response, listitem, inputtext[]) 
{
    switch(dialogid) {
        case DIALOG_ADMIN_DUTY: {
            if(!response) return 1;

            switch(listitem) {
                case 0..2, 8: callcmd::aduty(playerid);

                case 3: {
                    Iter_Remove(Permission, playerid);
                    playerData[playerid][player_admin] = 0;
                    msg(playerid, C_RED, "Wylogowa³eœ(aœ) siê z {c}rangi{/c}. Aby siê ponownie zalogowaæ, u¿yj: {c}/aduty{/c}.");
                }
                case 4: showDialog(playerid, DIALOG_ADMIN_PASSWORD_CHANGE, DIALOG_STYLE_PASSWORD, ""RED"Zmiana has³a na rangê", ""WHITE"Wpisz poni¿ej nowe has³o na rangê:", "Ustaw", "Cofnij");

                case 5: {
                    new hours = floatround(((playerData[playerid][player_online] + playerData[playerid][player_adminTime]) / 3600));
                    new minutes = floatround(((playerData[playerid][player_online] + playerData[playerid][player_adminTime]) / 60) % 60);

                    s[0] = EOS;
                    s = ""WHITE"Polecenie\t"LY"Rezultat\n";
                    catstr(s, C_WHITE, -1, "Tygodniowy przegrany czas:\t"LY"%02dh %02dmin", hours, minutes);
                    catstr(s, C_WHITE, -1, "Liczba punktów:\t"RED"A: %d"WHITE", "ORANGE"Ev: %d\n", playerData[playerid][player_adminPoints], playerData[playerid][player_eventPoints]);
                    catstr(s, C_WHITE, -1, "Liczba wykonanych eventów:\t"LY"%d\n", playerData[playerid][player_eventCount]);
                    strcat(s, " \n");
                    catstr(s, C_WHITE, -1, "Zbanowanych osób:\t"RED"%d\n", playerData[playerid][player_adminPenalty][ADMIN_PENALTY_BAN]);
                    catstr(s, C_WHITE, -1, "Wyrzuconych osób:\t"RED"%d\n", playerData[playerid][player_adminPenalty][ADMIN_PENALTY_KICK]);
                    catstr(s, C_WHITE, -1, "Uciszonych osób:\t"RED"%d\n", playerData[playerid][player_adminPenalty][ADMIN_PENALTY_MUTE]);
                    catstr(s, C_WHITE, -1, "Ostrze¿onych osób:\t"RED"%d\n", playerData[playerid][player_adminPenalty][ADMIN_PENALTY_WARN]);
                    catstr(s, C_WHITE, -1, "Uwiêzionych osób:\t"RED"%d\n", playerData[playerid][player_adminPenalty][ADMIN_PENALTY_JAIL]);
                    
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return callcmd::aduty(playerid); }
                    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""RED"Statystyki administracyjne", s, "OK", #), 1;
                }

                case 6: {
                    s[0] = EOS;
                    strcat(s, ""WHITE"1. \t"LORANGE"Ranking wed³ug: "BLUE2"przegranego czasu\n");
                    strcat(s, ""WHITE"2. \t"LORANGE"Ranking wed³ug: "RED"pkt. administracyjnych\n");
                    strcat(s, ""WHITE"3. \t"LORANGE"Ranking wed³ug: "GOLD"pkt. eventów\n");
                    strcat(s, ""WHITE"4. \t"LORANGE"Ranking wed³ug: "GOLD"liczby wykonanych eventów\n");
                    strcat(s, ""WHITE"5. \t"LORANGE"Ranking wed³ug: "LRED"nadanych banów\n");
                    strcat(s, ""WHITE"6. \t"LORANGE"Ranking wed³ug: "LRED"nadanych kicków\n");
                    strcat(s, ""WHITE"7. \t"LORANGE"Ranking wed³ug: "LRED"nadanych uciszeñ\n");
                    strcat(s, ""WHITE"8. \t"LORANGE"Ranking wed³ug: "LRED"nadanych warnów\n");
                    strcat(s, ""WHITE"9. \t"LORANGE"Ranking wed³ug: "LRED"nadanych jailów\n");
                    showDialog(playerid, DIALOG_ADMIN_RANKING, DIALOG_STYLE_LIST, ""DRED"Ranking administracji", s, "Wybierz", "Cofnij");
                }

                case 7: {
                    if(!playerData[playerid][player_isFake]) {
                        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                            #pragma unused pp, dd, ll
                            if(!rr) return 1;
                            return callcmd::fake(playerid, form("%s", ii));
                        }
                        Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_INPUT, ""BLUE"Fa³szywy nick", ""WHITE"Wpisz poni¿ej fa³szywy nick:", "Ustaw", "Cofnij");
                    }
                    else callcmd::fake(playerid, "");
                }

                default: {
                    if(listitem == 9) playerData[playerid][player_checkMuted] = !playerData[playerid][player_checkMuted];
                    if(listitem == 10) playerData[playerid][player_checkPM] = !playerData[playerid][player_checkPM];
                    if(listitem == 11) {
                        if(playerData[playerid][player_admin] < ADMIN) return dialogBox(playerid, C_RED, ""RED"Nie masz uprawnieñ, by móc u¿yæ tej funkcji!");
                        playerData[playerid][player_checkCMD] = !playerData[playerid][player_checkCMD];
                    }
                    callcmd::aduty(playerid);
                }
            }
            return 1;
        }

        case DIALOG_ADMIN_RANKING: {
            if(!response) return callcmd::aduty(playerid);

            new temp[128];
            dbstr[0] = EOS;
            switch(listitem) {
                case 0: format(dbstr, sizeof(dbstr), "SELECT nick, admin, admin_online FROM es_accounts WHERE admin > 0 ORDER BY admin_online DESC"), strcat(temp, "przegranego czasu");
                case 1: format(dbstr, sizeof(dbstr), "SELECT nick, admin, admin_points FROM es_accounts WHERE admin > 0 ORDER BY admin_points DESC"), strcat(temp, "pkt. administracyjnych");
                case 2: format(dbstr, sizeof(dbstr), "SELECT nick, admin, event_points FROM es_accounts WHERE admin > 0 ORDER BY event_points DESC"), strcat(temp, "pkt. eventów");
                case 3: format(dbstr, sizeof(dbstr), "SELECT nick, admin, event_count FROM es_accounts WHERE admin > 0 ORDER BY event_count DESC"), strcat(temp, "liczby wykonanych eventów");

                case 4: format(dbstr, sizeof(dbstr), "SELECT nick, admin, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(admin_penalty, ',', 1), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts WHERE admin > 0 ORDER BY `liczba` DESC"), strcat(temp, "nadanych banów");
                case 5: format(dbstr, sizeof(dbstr), "SELECT nick, admin, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(admin_penalty, ',', 2), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts WHERE admin > 0 ORDER BY `liczba` DESC"), strcat(temp, "nadanych kicków");
                case 6: format(dbstr, sizeof(dbstr), "SELECT nick, admin, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(admin_penalty, ',', 3), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts WHERE admin > 0 ORDER BY `liczba` DESC"), strcat(temp, "nadanych uciszeñ");
                case 7: format(dbstr, sizeof(dbstr), "SELECT nick, admin, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(admin_penalty, ',', 4), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts WHERE admin > 0 ORDER BY `liczba` DESC"), strcat(temp, "nadanych jailów");
                case 8: format(dbstr, sizeof(dbstr), "SELECT nick, admin, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(admin_penalty, ',', 5), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts WHERE admin > 0 ORDER BY `liczba` DESC"), strcat(temp, "nadanych warnów");
            }

            m_pquery(dbstr, "ShowAdminRanking", "dds", playerid, listitem, temp);
            return 1;
        }

        case DIALOG_ADMIN_LOGIN: {
            if(!response) return 1;
            if(!strmatch(playerData[playerid][player_adminPass], inputtext, true)) return dialogBox(playerid, C_RED, ""RED"Wprowadzono nieprawid³owe has³o na s³u¿bê.");

            Iter_Add(Permission, playerid);
            playerData[playerid][player_admin] = playerData[playerid][player_adminLoad];
            return msg(playerid, getRankColor(playerData[playerid][player_admin]), "Zalogowa³eœ(aœ) na rangê administracyjn¹ jako: {c}%s poz. %d{/c}.", getRankName(playerData[playerid][player_admin]), playerData[playerid][player_adminLevel]), 1;
        }

        case DIALOG_ADMIN_PASSWORD: {
            if(!response) return dialogBox(playerid, C_RED, ""RED"Anulowano próbê ustawienia has³a na s³u¿bê, wiêc logowanie na rangê nie powiod³o siê.");

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!isValidPassword(dest)) return showDialog(playerid, DIALOG_ADMIN_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Ustawianie has³a na s³u¿bê", ""WHITE"Wpisz poni¿ej has³o, którym bêdziesz siê logowaæ na rangê administracyjn¹.\n\n"RED"Dozwolona iloœæ znaków: "WHITE"5-25.", "Ustaw", "Anuluj");

            format(playerData[playerid][player_adminPass], 65, "%s", dest);
            m_pquery_format("UPDATE es_accounts SET admin_pass='%s' WHERE id=%d LIMIT 1;", dest, playerData[playerid][player_accountID]);
            return msg(playerid, C_YELLOW, "Ustawi³eœ(aœ) has³o na rangê administracyjn¹. Twoje has³o to: {c}%s{/c}.", dest);
        }

        case DIALOG_ADMIN_PASSWORD_CHANGE: {
            if(!response) return callcmd::aduty(playerid);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!isValidPassword(dest)) return showDialog(playerid, DIALOG_ADMIN_PASSWORD_CHANGE, DIALOG_STYLE_PASSWORD, ""RED"Zmiana has³a na rangê", ""WHITE"Wpisz poni¿ej nowe has³o na rangê:\n\n"RED"Dozwolona iloœæ znaków: "WHITE"5-25.", "Ustaw", "Cofnij");
            if(strmatch(dest, "@NULL", true)) return showDialog(playerid, DIALOG_ADMIN_PASSWORD_CHANGE, DIALOG_STYLE_PASSWORD, ""RED"Zmiana has³a na rangê", ""WHITE"Wpisz poni¿ej nowe has³o na rangê:\n\n"RED"Poda³eœ(aœ) has³o, które odpowiada za puste has³o.", "Ustaw", "Cofnij");

            format(playerData[playerid][player_adminPass], 65, "%s", dest);
            m_pquery_format("UPDATE es_accounts SET admin_pass='%s' WHERE id=%d LIMIT 1;", dest, playerData[playerid][player_accountID]);
            return msg(playerid, C_YELLOW, "Zmieni³eœ(aœ) has³o na rangê administracyjn¹. Twoje has³o to: {c}%s{/c}.", dest);
        }
    }
    return 0;
}

function ShowAdminRanking(playerid, index, const title[])
{
    if(!cache_num_rows()) return 1;

    s[0] = EOS;
    s = ""BLACK"ID\t"WHITE"Nick\t"LY"Rezultat\n";
    
    new nick[25], admin, result;
    for(new i = 0; i < cache_num_rows(); i++) {
        cache_get_value(i, 0, nick, sizeof(nick));
        cache_get_value_int(i, 1, admin);
        cache_get_value_int(i, 2, result);

        switch(index) {
            case 0: format(s, sizeof(s), "%s"BLACK"%d.\t{%06x}%s\t"LY"%02dh %02dmin\n", s, i + 1, getRankColor(admin) >>> 8, nick, floatround(((result) / 3600)), floatround(((result) / 60) % 60));
            case 1, 2: format(s, sizeof(s), "%s"BLACK"%d.\t{%06x}%s\t"LY"%d pkt.\n", s, i + 1, getRankColor(admin) >>> 8, nick, result);
            default: format(s, sizeof(s), "%s"BLACK"%d.\t{%06x}%s\t"LY"%d\n", s, i + 1, getRankColor(admin) >>> 8, nick, result);
        }
    }

    new topic[128]; format(topic, sizeof(topic), ""GREEN"Ranking wed³ug: "LY"%s", title);
    inline onRankingData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, rr, ll, ii
        return OnDialogResponse(playerid, DIALOG_ADMIN_DUTY, 1, 6, ""); }
    return Dialog_ShowCallback(playerid, using inline onRankingData, DIALOG_STYLE_TABLIST_HEADERS, topic, s, "OK", #), 1;
}