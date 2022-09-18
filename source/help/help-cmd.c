stock showPlayerCMD(playerid, optionid)
{
    s[0] = EOS;

    #define CMD_COLOR   "{0b84e0}"
    AddDialogListitem(playerid, ""CMD_COLOR"Komenda\t"WHITE"Wyja�nienie\n");

    new title[128];
    switch(optionid) {
        case 0: {
            format(title, sizeof(title), ""YELLOW"Komendy SA-MP");

            AddDialogListitem(playerid, ""CMD_COLOR"/pagesize [warto��]\t"WHITE"Ustawiasz d�ugo�� czatu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/fontsize [warto��]\t"WHITE"Ustawiasz wielko�� czcionki czatu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/q /quit\t"WHITE"Wychodzisz automatycznie z gry\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/timestamp\t"WHITE"W��czasz/wy��czasz wy�wietlanie godziny obok wiadomo�ci\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/save [nazwa]\t"WHITE"Zapisujesz koordynaty swojego po�o�enia\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/interior\t"WHITE"Sprawdzasz, w kt�rym ID interioru si� znajdujesz\n");
        }

        case 1: {
            format(title, sizeof(title), ""MINT"Komendy graczy");

            AddDialogListitem(playerid, ""CMD_COLOR"/ulecz\t"WHITE"Leczysz si� za 1000$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pancerz\t"WHITE"Kupujesz pancerz za 1000$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/hud\t"WHITE"Zarz�dzasz kolorystyk� szaty graficznej\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/gang\t"WHITE"Sprawdzasz panel gang�w\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/duel\t"WHITE"Zapraszasz kogo� na duela\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/aduel\t"WHITE"Akceptujesz ofert� duelu\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/konto\t"WHITE"Zarz�dzasz swoim kontem\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/stats [id]\t"WHITE"Sprawdzasz statystyki (bez id - swoje)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/exit\t"WHITE"Wychodzisz z jakiej� atrakcji, np. zabawy\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/skin [id]\t"WHITE"Zmieniasz skina (bez id - lista skin�w)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/kolory\t"WHITE"Zmieniasz kolor swojego nicku\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/kill\t"WHITE"Pope�niasz samob�jstwo\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/obiekty\t"WHITE"Zarz�dzasz widoczno�ci� obiekt�w\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pm\t"WHITE"Wysy�asz komu� prywatn� wiadomo��\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/re\t"WHITE"Odsy�asz komu� prywatn� wiadomo��\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/regulamin\t"WHITE"Sprawdzasz regulamin serwera\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/taryfikator\t"WHITE"Sprawdasz taryfikator kar\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/idzdo\t"WHITE"Wysy�asz komu� pro�b� o teleportacj�\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/tpaccept\t"WHITE"Akceptujesz czyj�� pro�b� o teleportacj�\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/czas\t"WHITE"Zmieniasz sw�j czas gry\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/dzien\t"WHITE"Ustawiasz sw�j czas gry na dzie�\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/noc\t"WHITE"Ustawiasz sw�j czas gry na noc\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/pogoda\t"WHITE"Zmieniasz swoj� pogod� w grze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/me\t"WHITE"Piszesz, jak� czynno�� wykonujesz obecnie\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/do\t"WHITE"Opisujesz co� w czasie rzeczywistym\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/siema\t"WHITE"Witasz si� ze wszystkimi na serwerze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/nara\t"WHITE"�egnasz si� ze wszystkimi na serwerze\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/zw\t"WHITE"Oznajmiasz, �e zaraz wracasz\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/jj\t"WHITE"Wracasz do gry po AFK\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/rsp\t"WHITE"Respawnujesz swoj� posta�\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/admins\t"WHITE"Sprawdzasz list� administracji online\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/bronie\t"WHITE"Kupujesz sobie jak�� bro�\n");
        }

        case 2: {
            format(title, sizeof(title), ""BLUE"Komendy dot. pojazd�w");

            AddDialogListitem(playerid, ""CMD_COLOR"/v [nazwa/model]\t"WHITE"Tworzysz pojazd (bez modelu - lista pojazd�w)\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/napraw\t"WHITE"Naprawiasz sw�j pojazd za 500$\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/flip\t"WHITE"Obracasz sw�j pojazd spowrotem na ko�a\n");
            AddDialogListitem(playerid, ""CMD_COLOR"/tune\t"WHITE"Tuningujesz losowo sw�j pojazd\n");
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
    catstr(s, C_ZAJEBISTY, -1, "Po��cze� z serwerem\t"WHITE"%s\n", strdot(connects));
    catstr(s, C_ZAJEBISTY, -1, "Wys�anych wiadomo�ci\t"WHITE"%s\n", strdot(messages));
    catstr(s, C_ZAJEBISTY, -1, "Zbanowanych graczy\t"WHITE"%s\n", strdot(bans));
    catstr(s, C_ZAJEBISTY, -1, "Wyrzuconych graczy\t"WHITE"%s\n", strdot(kicks));
    strcat(s, " \n");
    catstr(s, C_VIOLET, -1, "Za�o�onych gang�w:\t"WHITE"%d/%d\n", Iter_Count(Gangs), MAX_GANGS);
    catstr(s, C_VIOLET, -1, "Prywatne pojazdy:\t"WHITE"%s/%s\n", strdot(CountServerPrivCars()), strdot(MAX_PRIVCARS));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Statystyki serwera", s, "OK", #), 1;
}