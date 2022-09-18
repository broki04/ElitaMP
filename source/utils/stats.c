stock Task:RunPlayerStats(playerid, selectedid)
{
    new Task:t = task_new();

    new kills = GetAchievement(selectedid, ACH_KILLS);
    new deaths = GetAchievement(selectedid, ACH_DEATHS);

    s[0] = EOS;
    s = ""WHITE"Kategoria\t"BLUE"Wynik\n";
    format(s, sizeof(s), "%s"WHITE"Iloœæ gotówki:\t"BLUE"%s$\n", s, strdot(getMoney(selectedid)));
    format(s, sizeof(s), "%s"WHITE"Poziom postaci:\t"GREEN"%s poziom\n", s, strdot(playerData[selectedid][player_level]));
    format(s, sizeof(s), "%s"WHITE"Statystyki bitewne:\t"LRED"K: %s, D: %s "LY"(%.1f)\n", s, strdot(kills), strdot(deaths), getRatio(kills, deaths));
    strcat(s, " \n");
    format(s, sizeof(s), "%s"WHITE"Posiadana ranga:\t%s\n", s, (playerData[selectedid][player_admin] > GRACZ) ? (form("{%06x}%s poz. %d", getRankColor(playerData[selectedid][player_admin]) >>> 8, getRankName(playerData[selectedid][player_admin]), playerData[selectedid][player_adminLevel])) : (""GREY"Gracz"));
    format(s, sizeof(s), "%s"WHITE"Obecny gang:\t%s\n", s, (pGang[selectedid] == -1) ? (""GREY"BRAK") : (form("{%06x}%s [%s]", getGangColor(pGang[selectedid]) >>> 8, getGangName(pGang[selectedid]), getGangTag(pGang[selectedid]))));
    format(s, sizeof(s), "%s"WHITE"Liczba wizyt:\t"LGREEN"%s\n", s, strdot(playerData[selectedid][player_visits]));
    if(pDuelRank[playerid] == DUEL_RANK_CHAMPION) format(s, sizeof(s), "%s"WHITE"Ranga na duelach:\t"BROWN"%s #%d\n", s, getDuelName(pDuelRank[selectedid]), pDuelPoints[selectedid]);
    else format(s, sizeof(s), "%s"WHITE"Ranga na duelach:\t"BROWN"%s (%d/%d)\n", s, getDuelName(pDuelRank[selectedid]), pDuelPoints[selectedid], getDuelPoints(pDuelRank[selectedid]));
    format(s, sizeof(s), "%s"WHITE"Wartoœæ rynkowa:\t"LY"€%s", s, strdot(GetPlayerBudget(selectedid)));
    strcat(s, " \n");
    format(s, sizeof(s), "%s"WHITE"Aktualny czas gry:\t"YELLOW"%02dh %02d min\n", s, floatround(((playerData[selectedid][player_online]) / 3600)), floatround(((playerData[selectedid][player_online]) / 60) % 60));
    format(s, sizeof(s), "%s"WHITE"£¹czny czas gry:\t"YELLOW"%02dh %02d min\n", s, floatround(((playerData[selectedid][player_online] + playerData[selectedid][player_session]) / 3600)), floatround(((playerData[selectedid][player_online] + playerData[selectedid][player_session]) / 60) % 60));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, form(""BLUE"Statystyki: "LY"%s (userid: %d)", getNick(selectedid), playerData[selectedid][player_accountID]), s, "OK", #), t;
}

alias:stats("staty", "statystyki")
CMD:stats(playerid, params[])
{
    new id;
    if(sscanf(params, "d", id)) return task_await(RunPlayerStats(playerid, playerid));
    if(id == playerid) return task_await(RunPlayerStats(playerid, playerid));
    return task_await(RunPlayerStats(playerid, id)), 1;
}