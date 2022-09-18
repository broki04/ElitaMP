#include YSI_Coding\y_hooks

enum e_eventPanelData {
    event_id,
    event_cmd[25],
    event_description[128],
    bool:event_params
};

enum {
    EVENT_PANEL_TYPE,
    EVENT_PANEL_ADD,
    EVENT_PANEL_DELETE,
    EVENT_PANEL_GIFT,
    EVENT_PANEL_WEAPON,
    EVENT_PANEL_FREEZE,
    EVENT_PANEL_UNFREEZE,
    EVENT_PANEL_DISARM,
    EVENT_PANEL_GOD,
    EVENT_PANEL_DELGOD,
    EVENT_PANEL_COLOR,
    EVENT_PANEL_INVISIBLE,
    EVENT_PANEL_TIME,
    EVENT_PANEL_WEATHER,
    EVENT_PANEL_HEALTH,
    EVENT_PANEL_ARMOUR,
    EVENT_PANEL_VEHICLE,
    EVENT_PANEL_KICK50,
    EVENT_PANEL_STOP
};

new const eventPanelData[][e_eventPanelData] = {
    {EVENT_PANEL_TYPE,      "EvType",           "Ustawiasz typ wydarzenia",             true},
    {EVENT_PANEL_ADD,       "EvAdd",            "Dodajesz kogoœ do eventu",             true},
    {EVENT_PANEL_DELETE,    "EvDelete",         "Usuwasz kogoœ z eventu",               true},
    {EVENT_PANEL_GIFT,      "EvNagroda",        "Dajesz komuœ/wszystkim nagrodê",       true},
    {EVENT_PANEL_WEAPON,    "EvWeapon",         "Dajesz komuœ/wszystkim broñ",          true},
    {EVENT_PANEL_FREEZE,    "EvFreeze",         "Zamra¿asz kogoœ/wszystkich",           true},
    {EVENT_PANEL_UNFREEZE,  "EvUnFreeze",       "Odmra¿asz kogoœ/wszystkich",           true},
    {EVENT_PANEL_DISARM,    "EvDisArm",         "Rozbrajasz kogoœ/wszystkich",          true},
    {EVENT_PANEL_GOD,       "EvGod",            "Dajesz nieœmiertelnoœæ",               true},
    {EVENT_PANEL_DELGOD,    "EvDelGod",         "Zabierasz nieœmiertelnoœæ",            true},
    {EVENT_PANEL_COLOR,     "EvColor",          "Zmieniasz komuœ kolor nicku",          true},
    {EVENT_PANEL_INVISIBLE, "EvInvisible",      "Dajesz niewidzialnoœæ na radar",       true},
    {EVENT_PANEL_TIME,      "EvCzas",           "Zmieniasz czas na evencie",            true},
    {EVENT_PANEL_WEATHER,   "EvPogoda",         "Zmieniasz pogodê na evencie",          true},
    {EVENT_PANEL_HEALTH,    "EvHealth",         "Ustawiasz komuœ/wszystkim ¿ycie",      true},
    {EVENT_PANEL_ARMOUR,    "EvArmour",         "Ustawiasz komuœ/wszystkim pancerz",    true},
    {EVENT_PANEL_VEHICLE,   "EvCar",            "Tworzysz pojazd",                      true},
    {EVENT_PANEL_KICK50,    "EvKick50",         "Wyrzucasz oddalonych uczestników",     false},
    {EVENT_PANEL_STOP,      ""RED"EvEnd",       "Koñczysz event",                       true}
};

CMD:evpanel(playerid)
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    s[0] = EOS;
    s = ""BLACK"X\t"WHITE"Komenda\t"YELLOW"Wyjaœnienie\n";

    new x = 0;
    for(new i = 0; i < sizeof(eventPanelData); i++) {
        format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"/%s\t"YELLOW"%s\n", s, i + 1, eventPanelData[i][event_cmd], eventPanelData[i][event_description]);
        playerData[playerid][player_eventData][x++] = eventPanelData[i][event_id];
    }
    return showDialog(playerid, DIALOG_EVENT_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Panel zarz¹dzania eventem", s, "Wybierz", "Zamknij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_EVENT_PANEL: {
            if(!response) return 1;

            new optionid = playerData[playerid][player_eventData][listitem];
            playerData[playerid][player_eventData][0] = optionid;

            switch(optionid) {
                case EVENT_PANEL_TIME:      showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Czas na evencie", ""WHITE"Wpisz poni¿ej godzinê, jaka ma byæ na evencie:", "Ustaw", "Cofnij");
                case EVENT_PANEL_WEATHER:   showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Pogoda na evencie", ""WHITE"Wpisz poni¿ej pogodê, która ma byæ na evencie:", "Ustaw", "Cofnij");
                case EVENT_PANEL_INVISIBLE: showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Niewidzialnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ niewidzialnoœæ:", "Ustaw", "Cofnij");
                case EVENT_PANEL_COLOR:     showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zmiana koloru nicku", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz zmieniæ kolor:", "Dalej", "Cofnij");
                case EVENT_PANEL_GOD:       showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nieœmiertelnoœæ:", "Nadaj", "Cofnij");
                case EVENT_PANEL_DELGOD:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zabraæ nieœmiertelnoœæ:", "Zabierz", "Cofnij");
                case EVENT_PANEL_DISARM:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Rozbrajanie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by rozbroiæ:", "Rozbrój", "Cofnij");
                case EVENT_PANEL_UNFREEZE:  showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Odmra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by odmroziæ:", "OdmróŸ", "Cofnij");
                case EVENT_PANEL_FREEZE:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zamra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zamroziæ:", "ZamróŸ", "Cofnij");
                case EVENT_PANEL_ADD:       showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:", "Dodaj", "Cofnij");
                case EVENT_PANEL_DELETE:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Usuwanie z eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz usun¹æ z eventu:", "Usuñ", "Cofnij");
                case EVENT_PANEL_TYPE:      showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie typu wydarzenia", ""WHITE"Wpisz poni¿ej odpowiedni typ wydarzenia "RED"(dm, nodm, tdm, 10hp)"WHITE":", "Ustaw", "Cofnij");
                case EVENT_PANEL_GIFT:      showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie nagrody", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nagrodê:", "Nadaj", "Cofnij");
                case EVENT_PANEL_WEAPON:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):", "Ustaw", "Cofnij");
                case EVENT_PANEL_HEALTH:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie ¿ycia", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ ¿ycie "LY"(id/all wartoœæ):", "Ustaw", "Cofnij");
                case EVENT_PANEL_ARMOUR:    showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie pancerza", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ pancerz "LY"(id/all wartoœæ):", "Ustaw", "Cofnij");
                case EVENT_PANEL_VEHICLE:   showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Tworzenie pojazdu", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który chcesz stworzyæ:", "Ustaw", "Cofnij");
                case EVENT_PANEL_KICK50:    showDialog(playerid, DIALOG_EVENT_CONFIRM, DIALOG_STYLE_MSGBOX, ""BLUE"Wyrzucanie oddalonych uczestników", ""WHITE"Czy napewno chcesz wyrzuciæ oddalonych uczestników?\n"WHITE"Kliknij poni¿ej "YELLOW"odpowiedni "WHITE"przycisk!", "Wyrzuæ", "Cofnij");
                case EVENT_PANEL_STOP:      showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_MSGBOX, ""RED"Zakoñczenie eventu", ""WHITE"Wpisz poni¿ej "RED"ID gracza"WHITE", który wygra³ event!\n"LY"Je¿eli nikt nie wygra³, pole zostaw puste!", "Zakoñcz", "Cofnij");
            }
            return 1;
        }

        case DIALOG_EVENT_PARAMS: {
            if(!response) return callcmd::evpanel(playerid);

            new optionid = playerData[playerid][player_eventData][0];
            switch(optionid) {
                case EVENT_PANEL_VEHICLE: {
                    new modelid[32];
                    if(sscanf(inputtext, "s[32]", modelid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Tworzenie pojazdu", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który chcesz stworzyæ:\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(!GetVehicleNameByID(modelid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Tworzenie pojazdu", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który chcesz stworzyæ:\n\n"RED"Nieprawid³owa nazwa pojazdu.", "Ustaw", "Cofnij");

                    callcmd::evcar(playerid, form("%s", VehicleNames[GetVehicleNameByID(modelid) - 400]));
                }

                case EVENT_PANEL_HEALTH: {
                    new temp[20], Float:health;
                    if(sscanf(inputtext, "s[20]f", temp, health)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie ¿ycia", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ ¿ycie "LY"(id/all wartoœæ):\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(health < 1.0 || health > 100.0) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie ¿ycia", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ ¿ycie "LY"(id/all wartoœæ):\n\n"RED"Wartoœæ "WHITE"¿ycia "RED"musi mieœciæ siê w przedziale: "WHITE"1-100 "RED"HP.", "Ustaw", "Cofnij");

                    if(strmatch(temp, "all", true)) callcmd::evhealth(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie ¿ycia", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ ¿ycie "LY"(id/all wartoœæ):\n\n"RED"Ten gracz nie jest dostêpny.", "Ustaw", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie ¿ycia", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ ¿ycie "LY"(id/all wartoœæ):\n\n"RED"Ten gracz nie znajduje siê na evencie.", "Ustaw", "Cofnij");
                        callcmd::evhealth(playerid, form("%d %.1f", id, health));
                    }
                }

                case EVENT_PANEL_ARMOUR: {
                    new temp[20], Float:health;
                    if(sscanf(inputtext, "s[20]f", temp, health)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie pancerza", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ pancerz "LY"(id/all wartoœæ):\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(health < 1.0 || health > 100.0) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie pancerza", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ pancerz "LY"(id/all wartoœæ):\n\n"RED"Wartoœæ "WHITE"¿ycia "RED"musi mieœciæ siê w przedziale: "WHITE"1-100 "RED"HP.", "Ustaw", "Cofnij");

                    if(strmatch(temp, "all", true)) callcmd::evarmor(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie pancerza", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ pancerz "LY"(id/all wartoœæ):\n\n"RED"Ten gracz nie jest dostêpny.", "Ustaw", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie pancerza", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ pancerz "LY"(id/all wartoœæ):\n\n"RED"Ten gracz nie znajduje siê na evencie.", "Ustaw", "Cofnij");
                        callcmd::evarmor(playerid, form("%d %.1f", id, health));
                    }
                }

                case EVENT_PANEL_WEAPON: {
                    new temp[90], weaponid, _ammo;
                    if(sscanf(inputtext, "s[90]dd", temp, weaponid, _ammo)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(!IsValidWeapon(weaponid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):\n\n"RED"Podano nieprawid³owe ID broni.", "Ustaw", "Cofnij");
                    if(_ammo < 0 || _ammo > 9999) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):\n\n"RED"Dozwolony przedzia³ amunicji to: "WHITE"1-9999 "RED"ammo.", "Ustaw", "Cofnij");

                    if(strmatch(temp, "all", true)) callcmd::evweapon(playerid, form("all %d %d", weaponid, _ammo));
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):\n\n"RED"Ten gracz nie jest dostêpny.", "Ustaw", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dawanie broni", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ broñ "LY"(id/all weaponid ammo):\n\n"RED"Ten gracz nie znajduje siê na evencie.", "Ustaw", "Cofnij");
                        callcmd::evweapon(playerid, form("%d %d %d", id, weaponid, _ammo));
                    }
                }

                case EVENT_PANEL_INVISIBLE: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Niewidzialnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ niewidzialnoœæ:\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(strmatch(temp, "all", true)) callcmd::evinvisible(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Niewidzialnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ niewidzialnoœæ:\n\n"RED"Ten gracz nie jest dostêpny.", "Ustaw", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Niewidzialnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by ustawiæ niewidzialnoœæ:\n\n"RED"Ten gracz nie jest na evencie.", "Ustaw", "Cofnij");
                        callcmd::evinvisible(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_WEATHER: {
                    new weatherid;
                    if(sscanf(inputtext, "d", weatherid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Pogoda na evencie", ""WHITE"Wpisz poni¿ej pogodê, która ma byæ na evencie:\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(weatherid < 0 || weatherid > 20) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Pogoda na evencie", ""WHITE"Wpisz poni¿ej pogodê, która ma byæ na evencie:\n\n"RED"Podano nieprawid³owe ID pogody. "WHITE"(0-20)", "Ustaw", "Cofnij");
                    callcmd::evpogoda(playerid, form("%d", weatherid));
                }

                case EVENT_PANEL_TIME: {
                    new hourid;
                    if(sscanf(inputtext, "d", hourid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Czas na evencie", ""WHITE"Wpisz poni¿ej godzinê, jaka ma byæ na evencie:\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
                    if(hourid < 0 || hourid > 24) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Czas na evencie", ""WHITE"Wpisz poni¿ej godzinê, jaka ma byæ na evencie:\n\n"RED"Podano nieprawid³ow¹ godzinê. "WHITE"(0-24)", "Ustaw", "Cofnij");
                    callcmd::evczas(playerid, form("%d", hourid));
                }

                case EVENT_PANEL_COLOR: {
                    new id;
                    if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zmiana koloru nicku", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz zmieniæ kolor:\n\n"RED"Nieprawid³owe parametry.", "Dalej", "Cofnij");
                    if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zmiana koloru nicku", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz zmieniæ kolor:\n\n"RED"Ten gracz nie jest dostêpny.", "Dalej", "Cofnij");
                    if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zmiana koloru nicku", ""WHITE"Wpisz poni¿ej ID gracza, któremu chcesz zmieniæ kolor:\n\n"RED"Ten gracz nie jest na evencie.", "Dalej", "Cofnij");
                    callcmd::evcolor(playerid, form("%d", id));
                }

                case EVENT_PANEL_GOD: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nieœmiertelnoœæ:\n\n"RED"Nieprawid³owe parametry.", "Nadaj", "Cofnij");
                
                    if(strmatch(temp, "all", true)) callcmd::evgod(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nieœmiertelnoœæ:\n\n"RED"Ten gracz nie jest dostêpny.", "Nadaj", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nieœmiertelnoœæ:\n\n"RED"Ten gracz nie jest na evencie.", "Nadaj", "Cofnij");
                        callcmd::evgod(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_DELGOD: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zabraæ nieœmiertelnoœæ:\n\n"RED"Nieprawid³owe parametry.", "Zabierz", "Cofnij");
                
                    if(strmatch(temp, "all", true)) callcmd::evdelgod(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zabraæ nieœmiertelnoœæ:\n\n"RED"Ten gracz nie jest dostêpny.", "Zabierz", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nieœmiertelnoœæ na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zabraæ nieœmiertelnoœæ:\n\n"RED"Ten gracz nie jest na evencie.", "Zabierz", "Cofnij");
                        callcmd::evdelgod(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_DISARM: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Rozbrajanie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by rozbroiæ:\n\n"RED"Nieprawid³owe parametry.", "Rozbrój", "Cofnij");
                
                    if(strmatch(temp, "all", true)) callcmd::evdisarm(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Rozbrajanie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by rozbroiæ:\n\n"RED"Ten gracz nie jest dostêpny.", "Rozbrój", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Rozbrajanie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by rozbroiæ:\n\n"RED"Ten gracz nie jest na evencie.", "Rozbrój", "Cofnij");
                        callcmd::evdisarm(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_UNFREEZE: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Odmra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by odmroziæ:\n\n"RED"Nieprawid³owe parametry.", "OdmróŸ", "Cofnij");

                    if(strmatch(temp, "all", true)) callcmd::evunfreeze(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Odmra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by odmroziæ:\n\n"RED"Ten gracz nie jest dostêpny.", "OdmróŸ", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Odmra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by odmroziæ:\n\n"RED"Ten gracz nie jest na evencie.", "OdmróŸ", "Cofnij");
                        callcmd::evunfreeze(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_FREEZE: {
                    new temp[20];
                    if(sscanf(inputtext, "s[20]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zamra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zamroziæ:\n\n"RED"Nieprawid³owe parametry.", "ZamróŸ", "Cofnij");
                
                    if(strmatch(temp, "all", true)) callcmd::evfreeze(playerid, "all");
                    else {
                        new id = strval(temp);
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zamra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zamroziæ:\n\n"RED"Ten gracz nie jest dostêpny.", "ZamróŸ", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Zamra¿anie na evencie", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by zamroziæ:\n\n"RED"Ten gracz nie jest na evencie.", "ZamróŸ", "Cofnij");
                        callcmd::evfreeze(playerid, form("%d", id));
                    }
                }

                case EVENT_PANEL_DELETE: {
                    new id;
                    if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Usuwanie z eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz usun¹æ z eventu:\n\n"RED"Nieprawid³owe parametry.", "Usuñ", "Cofnij");
                    if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Usuwanie z eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz usun¹æ z eventu:\n\n"RED"Ten gracz nie jest dostêpny.", "Usuñ", "Cofnij");
                    if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Usuwanie z eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz usun¹æ z eventu:\n\n"RED"Ten gracz nie jest na evencie.", "Usuñ", "Cofnij");
                    callcmd::evdelete(playerid, form("%d", id));
                }

                case EVENT_PANEL_ADD: {
                    new id;
                    if(sscanf(inputtext, "d", id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:\n\n"RED"Nieprawid³owe parametry.", "Dodaj", "Cofnij");
                    if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:\n\n"RED"Ten gracz nie jest dostêpny.", "Dodaj", "Cofnij");
                    if(Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:\n\n"RED"Ten gracz jest ju¿ na evencie.", "Dodaj", "Cofnij");
                    if(EventLeader == id) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:\n\n"RED"Nie mo¿esz zaprosiæ samego siebie na event.", "Dodaj", "Cofnij");
                    if(NieMozeTeraz(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Dodawanie do eventu", ""WHITE"Wpisz poni¿ej ID gracza, którego chcesz dodaæ do eventu:\n\n"RED"Nie mo¿esz teraz zaprosiæ tego gracza na event.", "Dodaj", "Cofnij");
                    callcmd::evadd(playerid, form("%d", id));
                }

                case EVENT_PANEL_TYPE: {
                    new temp[90];
                    if(sscanf(inputtext, "s[90]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie typu wydarzenia", ""WHITE"Wpisz poni¿ej odpowiedni typ wydarzenia "RED"(dm, nodm, tdm, 10hp)"WHITE":", "Ustaw", "Cofnij");
                    if(strmatch(temp, "dm", true) || strmatch(temp, "nodm", true) || strmatch(temp, "tdm", true) || strmatch(temp, "10hp", true)) callcmd::evtype(playerid, form("%s", temp));
                    else return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Ustawianie typu wydarzenia", ""WHITE"Wpisz poni¿ej odpowiedni typ wydarzenia "RED"(dm, nodm, tdm, 10hp)"WHITE":\n\n"RED"Wprowadzono nieprawid³owy typ eventu.", "Ustaw", "Cofnij");
                }

                case EVENT_PANEL_GIFT: {
                    new temp[90];
                    if(sscanf(inputtext, "s[90]", temp)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie nagrody", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nagrodê:\n\n"RED"Nieprawid³owe parametry.", "Nadaj", "Cofnij");
                    if(strmatch(temp, "all", true)) callcmd::evnagroda(playerid, "");
                    else {
                        new id = strval(temp); 
                        if(!IsPlayerConnected(id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie nagrody", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nagrodê:\n\n"RED"Ten gracz nie jest dostêpny.", "Nadaj", "Cofnij");
                        if(!Iter_Contains(Event, id)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_INPUT, ""BLUE"Nadawanie nagrody", ""WHITE"Wpisz poni¿ej ID gracza, b¹dŸ "RED"all"WHITE", by nadaæ nagrodê:\n\n"RED"Ten gracz nie znajduje siê na evencie.", "Nadaj", "Cofnij");
                        callcmd::evnagroda(playerid, form("%d", strval(temp)));
                    }
                }

                case EVENT_PANEL_STOP: {
                    if(!isnull(inputtext)) {
                        new targetid = strval(inputtext);
                        if(!IsPlayerConnected(targetid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_MSGBOX, ""RED"Zakoñczenie eventu", ""WHITE"Wpisz poni¿ej "RED"ID gracza"WHITE", który wygra³ event!\n"LY"Je¿eli nikt nie wygra³, pole zostaw puste!\n\n"RED"Ten gracz nie jest dostêpny.", "Zakoñcz", "Cofnij");
                        if(!Iter_Contains(Event, targetid)) return showDialog(playerid, DIALOG_EVENT_PARAMS, DIALOG_STYLE_MSGBOX, ""RED"Zakoñczenie eventu", ""WHITE"Wpisz poni¿ej "RED"ID gracza"WHITE", który wygra³ event!\n"LY"Je¿eli nikt nie wygra³, pole zostaw puste!\n\n"RED"Ten gracz nie znajduje siê na evencie.", "Zakoñcz", "Cofnij");

                        callcmd::evend(playerid, form("%d", targetid));
                    }
                    else callcmd::evend(playerid, "");
                }
            }
            return callcmd::evpanel(playerid);
        }

        case DIALOG_EVENT_CONFIRM: {
            if(!response) return callcmd::evpanel(playerid);

            new optionid = playerData[playerid][player_eventData][0];
            switch(optionid) {
                case EVENT_PANEL_KICK50: callcmd::evkick50(playerid);
            }
            return 1;
        }

        case DIALOG_EVENT_COLOR: {
            if(!response) return 1;

            new targetid = GetPVarInt(playerid, "player_event_targetid");
            if(listitem != INVALID_LISTITEM) {
                SetPlayerColor(targetid, KoloryGraczy[listitem]);

                msg(playerid, C_CORAL, "Zmieni³eœ(aœ) kolor uczestnika {c}%s(%d){/c} na: {%06x}TAKI!", getNick(targetid), targetid, KoloryGraczy[listitem] >>> 8);
                msg(targetid, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zmieni³(a) Ci kolor nicku na: {%06x}TAKI!", getNick(playerid), playerid, KoloryGraczy[listitem] >>> 8);
            }
            return 1;
        }
    }

    return 0;
}