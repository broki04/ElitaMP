alias:team("teams", "zespoly", "zespol")
CMD:team(playerid) 
{
    s[0] = EOS;
    
    new gangid = pGang[playerid];
    if(gangid == -1) {
        format(s, sizeof(s), "%s"GREY"�  "WHITE"Lista zespo��w na serwerze\n", s);
        format(s, sizeof(s), "%s"GREY"�  "WHITE"Podium najlepszych zespo��w\n", s);
        format(s, sizeof(s), "%s"GREY"�  "WHITE"Strefy do przejmowania\n", s);
        format(s, sizeof(s), "%s"GREY"�  "WHITE"Za�� w�asny zesp� "YELLOW"(%s$)\n", s, strdot(GANG_PRICE_MONEY));
        format(s, sizeof(s), "%s"GREY"�  "WHITE"Zaproszenia do zespo��w "GREEN"(/tzaproszenia)\n", s);
        return showDialog(playerid, DIALOG_GANG_NEWBIE, DIALOG_STYLE_LIST, ""YELLOW"Panel zespo��w", s, "Wybierz", "Zamknij");
    } else {
        format(s, sizeof(s), "{%06x}%s [%s] - "WHITE"%s/%s exp {%06x}(poz. %s)\n", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid), strdot(gangData[gangid][gang_respect]), strdot(gangData[gangid][gang_respect_needed]), getGangColor(gangid) >>> 8, strdot(gangData[gangid][gang_level]));
        format(s, sizeof(s), "%s{%06x}Pozycja zespo�u w rankingu: "WHITE"#%s\n", s, getGangColor(gangid) >>> 8, strdot(gangData[gangid][gang_place]));
        if(pGangSparRank[gangid] == SPARING_RANK_CHAMPION) format(s, sizeof(s), "%s{%06x}Ranga na sparingach: "WHITE"%s #%d\n", s, getGangColor(gangid) >>> 8, getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid]);
        else format(s, sizeof(s), "%s{%06x}Ranga na sparingach: "WHITE"%s (%d/%d)\n", s, getGangColor(gangid) >>> 8, getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], getRankingPoints(pGangSparRank[gangid]));
        strcat(s, " \n");
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Lista zespo��w na serwerze\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Podium najlepszych zespo��w\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Strefy do przejmowania\n", s, getGangColor(gangid) >>> 8);
        strcat(s, " \n");
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Bud�et zespo�u "LY"(�%s)\n", s, getGangColor(gangid) >>> 8, strdot(gangData[gangid][gang_budget]));
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Sparingi zespo�owe\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Osi�gni�cia zespo�u\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Rynek transferowy\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Ulepszenia zespo�u\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "WHITE"Bazy zespo�owe\n", s, getGangColor(gangid) >>> 8);
        format(s, sizeof(s), "%s{%06x}�  "GREEN"Zapro� gracza do zespo�u\n", s, getGangColor(gangid) >>> 8);
        if(checkRankPermission(playerid, GANG_PERM_MEMBERS)) format(s, sizeof(s), "%s{%06x}�  "GOLD"Zarz�dzanie cz�onkami zespo�u\n", s, getGangColor(gangid) >>> 8);
        if(gangData[gangid][gang_ownerid] == playerData[playerid][player_accountID]) format(s, sizeof(s), "%s{%06x}�  "RED"Zarz�dzanie zespo�em\n", s, getGangColor(gangid) >>> 8);
        strcat(s, " \n");
        format(s, sizeof(s), "%s{%06x}CMD: \t/topusc /tcolor /tspawn", s, getGangColor(gangid) >>> 8);
        return showDialog(playerid, DIALOG_GANG, DIALOG_STYLE_LIST, form("{%06x}%s [%s]", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid)), s, "Wybierz", "Zamknij");
    }
}

alias:tkolor("tcolor")
CMD:tkolor(playerid) 
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jeste� w �adnym zespole.");
    return SetPlayerColor(playerid, gangData[pGang[playerid]][gang_color]), 1;
}

alias:topusc("tleave", "texit")
CMD:topusc(playerid)
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jeste� w �adnym zespole.");

    new gangid = pGang[playerid];
    if(gangData[gangid][gang_ownerid] == playerData[playerid][player_accountID]) return sendError(playerid, ""RED"Nie mo�esz opu�ci� zespo�u, kt�rego jeste� za�o�ycielem.");
    return LeaveGang(playerid, gangid);
}

stock LeaveGang(playerid, gangid, bool:kicked = false)
{
    if(pGang[playerid] == -1) return 1;

    for(new i = 0; i < 3; i++) TextDrawHideForPlayer(playerid, gangData[gangid][gang_draw][i]);
    HidePlayerProgressBar(playerid, GangBar[playerid]);

    pGang[playerid] = -1;
    Iter_Remove(GangMember[gangid], playerid);
    UpdateGangBox(gangid);

    m_pquery_format("UPDATE es_accounts SET gang=-1 WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID]);
    if(!kicked) msg(playerid, C_BLUE, "Opu�ci�e�(a�) zesp� {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid));
    return 1;
}

alias:tzaproszenia("tinvites")
CMD:tzaproszenia(playerid) 
{
    if(pGang[playerid] != -1) return sendError(playerid, ""RED"Jeste� ju� w jakim� zespole.");

    s[0] = EOS;
    for(new i = 0; i < 5; i++) {
        if(pGangInvites[playerid][i] != -1) {
            new gangid = pGangInvites[playerid][i];
            format(s, sizeof(s), "%s"WHITE"�  {%06x}%s [%s] "WHITE"- ranking: {%06x}#%s\n", s, getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid), getGangColor(gangid) >>> 8, strdot(gangData[gangid][gang_place]));
        }
        else strcat(s, ""WHITE"�  "GREY"WOLNY SLOT\n");
    }
    return showDialog(playerid, DIALOG_GANG_INVITE_LIST, DIALOG_STYLE_LIST, ""YELLOW"Zaproszenia do zespo��w", s, "Do��cz", "Zamknij"), 1;
}