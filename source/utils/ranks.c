new pOwnRank[MAX_PLAYERS][50], pOwnRankColor[MAX_PLAYERS], bool:pOwnRankHave[MAX_PLAYERS];

new const deniedRankNames[][] = {
    "kurw", "chuj", "h�j", "huj", "k�rw", "g�wn", "guwn", "idiot", "cip", "rucha", "pizd", "dziwk", "szmat", "suk", "skurw", "jeb", "pierd", "debil", "/q",
	"admin", "moder", "pawn", "pseudol", "XDDDDD", "X D D D D D", "a d m i n", "event", "wlas", "w�as", "w�a�", "wla�", "RCON", "R C O N", "CEO", "C E O",
    "eventmaker", "eventowicz", "ewentowicz", "ev maker", "evmaker", "atmin", "a t m i n", "adm", "vice head", "vicehead", "programista", "skrypter", "moderator"
};

stock getRankName(rankid)
{
    new name[15];
    switch(rankid) {
        case GRACZ:         name = "Gracz";
        case MODERATOR:     name = "Moderator";
        case ADMIN:         name = "Admin";
        case VICE:          name = "Vice Head";
        case HEAD:          name = "Head Admin";
    }
    return name;
}

stock getRankColor(rankid) 
{
    new color;
    switch(rankid) {
        case GRACZ:         color = 0xAFAFAFFF;
        case MODERATOR:     color = 0x00CA00FF;
        case ADMIN:         color = 0x206694FF;
        case VICE:          color = 0x3498dbFF;
        case HEAD:          color = 0xFD0000FF;
    }
    return color;
}

alias:ownrank("ranga", "rank", "rangi", "ranks")
CMD:ownrank(playerid)
{
    if(!IsPremium(playerid)) return PremiumError(playerid);
    if(strmatch(pOwnRank[playerid], "@NULL", true)) return showDialog(playerid, DIALOG_OWNRANK_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Kreator w�asnej rangi (1/2)", ""WHITE"Wpisz poni�ej nazw� dla swojej rangi:", "Dalej", "Anuluj");
    
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"1. "LY"%s rang� na czacie\n", (pOwnRankHave[playerid]) ? ("Ukryj") : ("Poka�"));
    format(s, sizeof(s), "%s"WHITE"2. "LY"Zmie� nazw� rangi: {%06x}%s\n", s, pOwnRankColor[playerid] >>> 8, pOwnRank[playerid]);
    format(s, sizeof(s), "%s"WHITE"3. "LY"Zmie� kolor rangi: {%06x}%s\n", s, pOwnRankColor[playerid] >>> 8, pOwnRank[playerid]);
    return showDialog(playerid, DIALOG_OWNRANK_PANEL, DIALOG_STYLE_LIST, ""YELLOW"Zarz�dzanie w�asn� rang�", s, "Wybierz", "Zamknij"), 1;
}

stock OnDialogOwnRank(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_OWNRANK_PANEL: {
            if(!response) return 1;

            switch(listitem) {
                case 0: {
                    pOwnRankHave[playerid] = !pOwnRankHave[playerid];
                    callcmd::ownrank(playerid);
                }

                case 1: showDialog(playerid, DIALOG_OWNRANK_EDIT_NAME, DIALOG_STYLE_INPUT, ""BLUE"Edycja nazwy rangi", ""WHITE"Wpisz poni�ej now� nazw� dla rangi:", "Ustaw", "Cofnij");
                case 2: {
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
                    showDialog(playerid, DIALOG_OWNRANK_EDIT_COLOR, DIALOG_STYLE_LIST, ""PINK"Edycja koloru rangi", #, "Ustaw", "Cofnij", 10);
                }
            }
            return 1;
        }

        case DIALOG_OWNRANK_EDIT_COLOR: {
            if(!response) return callcmd::ownrank(playerid);

            if(listitem != INVALID_LISTITEM) {
                pOwnRankColor[playerid] = KoloryGraczy[listitem];
                m_pquery_format("UPDATE es_accounts SET own_rank_color=%d WHERE id=%d LIMIT 1;", pOwnRankColor[playerid], playerData[playerid][player_accountID]);
                msg(playerid, pOwnRankColor[playerid], "Zmieni�e�(a�) kolor swojej rangi na taki, jak ta {c}wiadomo��{/c}.");
                callcmd::ownrank(playerid);
            }
            return 1;
        }

        case DIALOG_OWNRANK_COLOR: {
            if(!response) return 1;

            if(listitem != INVALID_LISTITEM) {
                pOwnRankColor[playerid] = KoloryGraczy[listitem];

                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Kreacja rangi zosta�a zako�czona.\n"WHITE"Nazwa twojej rangi to: {%06x}%s\n\n", pOwnRankColor[playerid] >>> 8, pOwnRank[playerid]);
                format(s, sizeof(s), "%s"WHITE"Kliknij odpowiedni przycisk, aby potwierdzi� sw�j wyb�r.\n"WHITE"Pami�taj - w ka�dej chwili mo�esz zarz�dza� swoj� rang� pod: "BLUE2"/ownrank.", s);
                
                inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                    #pragma unused pp, dd, ll, ii

                    if(!rr) {
                        pOwnRankColor[playerid] = -1;
                        strcat(pOwnRank[playerid], "@NULL");
                        pOwnRankHave[playerid] = false;
                    } else {
                        pOwnRankHave[playerid] = true;
                        callcmd::ownrank(playerid);
                        
                        msg(playerid, pOwnRankColor[playerid], "Stworzy�e�(a�) swoj� rang�: {c}%s{/c}. Mo�esz j� zarz�dza� komend�: {c}/ownrank{/c}.", pOwnRank[playerid]);
                        m_pquery_format("UPDATE es_accounts SET own_rank='%s', own_rank_color=%d WHERE id=%d LIMIT 1;", pOwnRank[playerid], pOwnRankColor[playerid], playerData[playerid][player_accountID]);
                    }
                    return 1;
                }
                Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE2"Kreator w�asnej rangi - podsumowanie", s, "Tak", "Nie");
            }
            return 1;
        }

        case DIALOG_OWNRANK_EDIT_NAME: {
            if(!response) return callcmd::ownrank(playerid);
            if(strlen(inputtext) < 4 || strlen(inputtext) > 20) return showDialog(playerid, DIALOG_OWNRANK_EDIT_NAME, DIALOG_STYLE_INPUT, ""BLUE"Edycja nazwy rangi", ""WHITE"Wpisz poni�ej now� nazw� dla rangi:\n\n"RED"Dozwolony przedzia� znak�w to: "WHITE"4-20 "RED"znaki.", "Ustaw", "Cofnij");
            for(new i = 0; i < sizeof(deniedRankNames); i++) { while((strfind(inputtext, deniedRankNames[i], true)) > -1) return showDialog(playerid, DIALOG_OWNRANK_EDIT_NAME, DIALOG_STYLE_INPUT, ""BLUE"Edycja nazwy rangi", ""WHITE"Wpisz poni�ej now� nazw� dla rangi:\n\n"RED"Wprowadzono niedozwolone s�owo w nazwie rangi.", "Ustaw", "Cofnij"); }
            
            format(pOwnRank[playerid], 50, "%s", inputtext);
            m_pquery_format("UPDATE es_accounts SET own_rank='%s' WHERE id=%d LIMIT 1;", inputtext, playerData[playerid][player_accountID]);
            msg(playerid, pOwnRankColor[playerid], "Zmieni�e�(a�) nazw� rangi na: {c}%s{/c}.", inputtext);
            return callcmd::ownrank(playerid), 1;
        }

        case DIALOG_OWNRANK_NAME: {
            if(!response) return 1;
            if(strlen(inputtext) < 4 || strlen(inputtext) > 20) return showDialog(playerid, DIALOG_OWNRANK_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Kreator w�asnej rangi (1/2)", ""WHITE"Wpisz poni�ej nazw� dla swojej rangi:\n\n"RED"Dozwolony przedzia� znak�w to: "WHITE"4-20 "RED"znaki.", "Dalej", "Anuluj");
            for(new i = 0; i < sizeof(deniedRankNames); i++) { while((strfind(inputtext, deniedRankNames[i], true)) > -1) return showDialog(playerid, DIALOG_OWNRANK_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Kreator w�asnej rangi (1/2)", ""WHITE"Wpisz poni�ej nazw� dla swojej rangi:\n\n"RED"Wprowadzono niedozwolone s�owo w nazwie rangi.", "Dalej", "Anuluj"); }

            format(pOwnRank[playerid], 50, "%s", inputtext);

            s[0] = EOS;
            for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
            return showDialog(playerid, DIALOG_OWNRANK_COLOR, DIALOG_STYLE_LIST, ""BLUE"Kreator w�asnej rangi (2/2)", #, "Ustaw", "Cofnij", 10);
        }
    }
    return 0;
}
