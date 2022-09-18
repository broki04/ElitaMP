stock showPlayerCMD(playerid, optionid)
{
    s[0] = EOS;

    #define CMD_COLOR   "{0b84e0}"
    AddDialogListitem(playerid, ""CMD_COLOR"Komenda\t"WHITE"Wyjaœnienie\n");

    new title[128];
    switch(optionid) {
        case 0: {
            format(title, sizeof(title), ""YELLOW"Komendy SA-MP");

            AddDialogListitem(playerid, ""CMD_COLOR"/pagesize [wartoœæ]\t"WHITE"Ustawiasz d³ugoœæ czatu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/fontsize [wartoœæ]\t"WHITE"Ustawiasz wielkoœæ czcionki czatu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/q /quit\t"WHITE"Wychodzisz automatycznie z gry\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/timestamp\t"WHITE"W³¹czasz/wy³¹czasz wyœwietlanie godziny obok wiadomoœci\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/save [nazwa]\t"WHITE"Zapisujesz koordynaty swojego po³o¿enia\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/interior\t"WHITE"Sprawdzasz, w którym ID interioru siê znajdujesz\n");
        }

        case 1: {
            format(title, sizeof(title), ""MINT"Komendy graczy");

            AddDialogListitem(playerid, ""CMD_COLOR"/ulecz\t"WHITE"Leczysz siê za 1000$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pancerz\t"WHITE"Kupujesz pancerz za 1000$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/hud\t"WHITE"Zarz¹dzasz kolorystyk¹ szaty graficznej\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/gang\t"WHITE"Sprawdzasz panel gangów\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/duel\t"WHITE"Zapraszasz kogoœ na duela\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/aduel\t"WHITE"Akceptujesz ofertê duelu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/konto\t"WHITE"Zarz¹dzasz swoim kontem\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/stats [id]\t"WHITE"Sprawdzasz statystyki (bez id - swoje)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/exit\t"WHITE"Wychodzisz z jakiejœ atrakcji, np. zabawy\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/skin [id]\t"WHITE"Zmieniasz skina (bez id - lista skinów)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/kolory\t"WHITE"Zmieniasz kolor swojego nicku\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/kill\t"WHITE"Pope³niasz samobójstwo\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/obiekty\t"WHITE"Zarz¹dzasz widocznoœci¹ obiektów\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pm\t"WHITE"Wysy³asz komuœ prywatn¹ wiadomoœæ\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/re\t"WHITE"Odsy³asz komuœ prywatn¹ wiadomoœæ\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/regulamin\t"WHITE"Sprawdzasz regulamin serwera\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/taryfikator\t"WHITE"Sprawdasz taryfikator kar\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/idzdo\t"WHITE"Wysy³asz komuœ proœbê o teleportacjê\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/tpaccept\t"WHITE"Akceptujesz czyj¹œ proœbê o teleportacjê\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/czas\t"WHITE"Zmieniasz swój czas gry\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/dzien\t"WHITE"Ustawiasz swój czas gry na dzieñ\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/noc\t"WHITE"Ustawiasz swój czas gry na noc\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pogoda\t"WHITE"Zmieniasz swoj¹ pogodê w grze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/me\t"WHITE"Piszesz, jak¹ czynnoœæ wykonujesz obecnie\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/do\t"WHITE"Opisujesz coœ w czasie rzeczywistym\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/siema\t"WHITE"Witasz siê ze wszystkimi na serwerze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/nara\t"WHITE"¯egnasz siê ze wszystkimi na serwerze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/zw\t"WHITE"Oznajmiasz, ¿e zaraz wracasz\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/jj\t"WHITE"Wracasz do gry po AFK\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/rsp\t"WHITE"Respawnujesz swoj¹ postaæ\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/admins\t"WHITE"Sprawdzasz listê administracji online\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/bronie\t"WHITE"Kupujesz sobie jak¹œ broñ\n");
        }

        case 2: {
            format(title, sizeof(title), ""BLUE"Komendy dot. pojazdów");

            AddDialogListitem(playerid, ""CMD_COLOR"/v [nazwa/model]\t"WHITE"Tworzysz pojazd (bez modelu - lista pojazdów)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/napraw\t"WHITE"Naprawiasz swój pojazd za 500$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/flip\t"WHITE"Obracasz swój pojazd spowrotem na ko³a\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/tune\t"WHITE"Tuningujesz losowo swój pojazd\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/lakier\t"WHITE"Zmieniasz kolor lakieru swojego pojazdu\n");
        }
    }
    return showDialog(playerid, DIALOG_HELP_CMDLIST, DIALOG_STYLE_TABLIST_HEADERS, title, #, "OK", "", 10), 1;
}

alias:serverstats("server", "elita")
CMD:serverstats(playerid)
{
    s[0] = EOS;
    s = ""ZAJEBISTY"Kategoria\t"WHITE"Wynik\n";
    catstr(s, C_ZAJEBISTY, -1, "Po³¹czeñ z serwerem\t"WHITE"%s\n", strdot(connects));
    catstr(s, C_ZAJEBISTY, -1, "Wys³anych wiadomoœci\t"WHITE"%s\n", strdot(messages));
    catstr(s, C_ZAJEBISTY, -1, "Zbanowanych graczy\t"WHITE"%s\n", strdot(bans));
    catstr(s, C_ZAJEBISTY, -1, "Wyrzuconych graczy\t"WHITE"%s\n", strdot(kicks));
    strcat(s, " \n");
    catstr(s, C_VIOLET, -1, "Za³o¿onych gangów:\t"WHITE"%d/%d\n", Iter_Count(Gangs), MAX_GANGS);
    catstr(s, C_VIOLET, -1, "Prywatne pojazdy:\t"WHITE"%s/%s\n", strdot(CountServerPrivCars()), strdot(MAX_PRIVCARS));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Statystyki serwera", s, "OK", #), 1;
}