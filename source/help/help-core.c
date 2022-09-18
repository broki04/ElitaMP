alias:help("pomoc", "cmd")
CMD:help(playerid)
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Witaj, {%06x}%s"WHITE" - poni¿ej znajdziesz odpowiedni¹ pomoc!\n", GetPlayerColor(playerid) >>> 8, getNick(playerid));
    format(s, sizeof(s), "%s"WHITE"Obecnie jest godzina: "ORANGE"%s\n", s, getLocalDate());
    format(s, sizeof(s), "%s"WHITE"Aktualna liczba graczy: "GREY"%d osób "WHITE"("YELLOW"%d premium"WHITE", "RED"%d adminów"WHITE")\n", s, Iter_Count(Player), Iter_Count(Premium), Iter_Count(Permission));
    strcat(s, " \n");
    strcat(s, ""BLUE"»  "WHITE"Dostêpne komendy "GREEN"(/cmds)\n");
    strcat(s, ""BLUE"»  "WHITE"Panel konta Premium "YELLOW"(/premium)\n");
    strcat(s, ""BLUE"»  "WHITE"Œciœle tajne dla adminów "RED"(/acmd)\n");
    strcat(s, ""BLUE"»  "WHITE"Dostêpne teleporty "BLUE2"(/teles)\n");
    strcat(s, ""BLUE"»  "WHITE"Animacje "VIOLET2"(/anims)\n");
    strcat(s, ""BLUE"»  "WHITE"Regulamin serwera "LY"(/regulamin)\n");
    strcat(s, ""BLUE"»  "WHITE"Taryfikator serwera "DRED"(/taryfikator)\n");
    strcat(s, ""BLUE"»  "WHITE"Sklep serwera "PINK"(/sklep)\n");
    strcat(s, ""BLUE"»  "WHITE"Rekordy serwera "ORANGE"(/rekordy)\n");
    strcat(s, ""BLUE"»  "WHITE"Autor GameMode "NICK"(/autor)");
    return showDialog(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, ""SERVER_TAG" "WHITE"- pomoc", s, "Wybierz", "Zamknij"), 1;
}

stock OnDialogHelp(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_HELP_CMDLIST: {
            if(listitem != INVALID_LISTITEM) return OnDialogResponse(playerid, DIALOG_HELP, 1, 4, ""); 
            return 1;
        }
        case DIALOG_HELP_ADMIN: return callcmd::help(playerid);

        case DIALOG_HELP_CMDS: {
            if(!response) return callcmd::help(playerid);
            return showPlayerCMD(playerid, listitem), 1;
        }

        case DIALOG_HELP: {
            if(!response) return 1;

            switch(listitem) {
                case 0..3: callcmd::help(playerid);
                
                case 4: {
                    s[0] = EOS;
                    strcat(s, ""WHITE"1. \t"GREY"Komendy SA-MP\n");
                    strcat(s, ""WHITE"2. \t"GREY"Komendy gracza\n");
                    strcat(s, ""WHITE"3. \t"GREY"Komendy dot. pojazdów\n");
                    showDialog(playerid, DIALOG_HELP_CMDS, DIALOG_STYLE_LIST, ""BLUE"Dostêpne komendy", s, "Wybierz", "Cofnij");
                }

                case 5: {} // vip
                case 6: callcmd::acmd(playerid);
                case 7: callcmd::teles(playerid);
                case 8: callcmd::anims(playerid);
                case 9: callcmd::regulamin(playerid);
                case 10: callcmd::taryfikator(playerid);
                case 11: {} // sklep
                case 12: {} // rekordy
                case 13: callcmd::autor(playerid);
            }
        }
    }
    return 0;
}