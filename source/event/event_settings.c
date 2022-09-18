#include YSI_Coding\y_hooks

CMD:evsettings(playerid)
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    s[0] = EOS;
    s = ""LY"Polecenie\t"GREEN"Sta"RED"tus\n";

    catstr(s, C_LY, -1, "Nazwa eventu\t"WHITE"%s\n", EventName);
    catstr(s, C_LY, -1, "Typ eventu\t"BLUE2"%s\n", GetEventType(EventType));
    strcat(s, " \n");

    for(new i = 0; i < sizeof(eventSettingData); i++) {
        if(i == EVENT_SETTING_VEHICLE) format(s, sizeof(s), "%s"LY"%s\t"BLUE2"%s (ID: %d)\n", s, eventSettingData[i], (EventVehicle == -1) ? ("---") : (form("%s", VehicleNames[EventVehicle - 400])));
        else format(s, sizeof(s), "%s"LY"%s\t%s\n", s, eventSettingData[i], (EventSettings[i]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
    }
    return showDialog(playerid, DIALOG_EVENT_SETTINGS, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Ustawienia eventu", s, "Wybierz", "Zamknij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_EVENT_NAME: {
            if(!response) return callcmd::evsettings(playerid);

            new name[50];
            if(sscanf(inputtext, "s[50]", name) || strlen(name) < 3 || strlen(name) > 20) return showDialog(playerid, DIALOG_EVENT_NAME, DIALOG_STYLE_INPUT, ""GREEN"Zmiana nazwy eventu", ""WHITE"Wpisz poni¿ej now¹ nazwê eventu:\n\n"RED"Nieprawid³owa nazwa.", "Ustaw", "Cofnij");
            
            format(EventName, sizeof(EventName), "%s", name);
            foreach(new i : Event) msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zmieni³(a) nazwê eventu na: {c}%s{/c}.", getNick(playerid), playerid, name);
            return callcmd::evsettings(playerid), 1;
        }

        case DIALOG_EVENT_VEHICLE: {
            if(!response) return callcmd::evsettings(playerid);

            new modelid[32];
            if(sscanf(inputtext, "s[32]", modelid)) return showDialog(playerid, DIALOG_EVENT_VEHICLE, DIALOG_STYLE_INPUT, ""RED"Ustaw pojazd eventowy", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który bêdzie móg³ byæ spawnowany przez uczestników:\n\n"RED"Nieprawid³owe parametry.", "Ustaw", "Cofnij");
            if(!GetVehicleNameByID(modelid)) return showDialog(playerid, DIALOG_EVENT_VEHICLE, DIALOG_STYLE_INPUT, ""RED"Ustaw pojazd eventowy", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który bêdzie móg³ byæ spawnowany przez uczestników:\n\n"RED"Nieprawid³owa nazwa pojazdu.", "Ustaw", "Cofnij");

            EventVehicle = GetVehicleNameByID(modelid);
            foreach(new i : Event) msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³ {c}%s (ID: %d){/c} jako pojazd eventowy! "LY"(/evspawn)", getNick(playerid), playerid, VehicleNames[EventVehicle - 400], EventVehicle);
            msg(playerid, C_CORAL, "Ustawi³eœ(aœ) {c}%s (ID: %d){/c} jako pojazd eventowy.", VehicleNames[EventVehicle - 400], EventVehicle);
            return callcmd::evsettings(playerid), 1;
        }

        case DIALOG_EVENT_SETTINGS: {
            if(!response) return 1;
            
            switch(listitem) {
                case 2: callcmd::evsettings(playerid);

                case 0: showDialog(playerid, DIALOG_EVENT_NAME, DIALOG_STYLE_INPUT, ""GREEN"Zmiana nazwy eventu", ""WHITE"Wpisz poni¿ej now¹ nazwê eventu:", "Ustaw", "Cofnij");
                case 1: {
                    s[0] = EOS;
                    for(new i = 1; i < EVENT_TYPE_LAST + 1; i++) format(s, sizeof(s), "%s"BLUE2"  "ARROW" "WHITE"%s\n", s, GetEventType(i));

                    inline onEventType(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ii
                        if(!rr) return callcmd::evsettings(playerid);

                        new type[10];
                        switch(ll + 1) {
                            case EVENT_TYPE_DM: type = "dm";
                            case EVENT_TYPE_NODM: type = "nodm";
                            case EVENT_TYPE_TDM: type = "tdm";
                            case EVENT_TYPE_1V1: type = "1v1";
                            case EVENT_TYPE_10HP: type = "10hp";
                        }

                        callcmd::evtype(playerid, type);
                        return callcmd::evsettings(playerid), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onEventType, DIALOG_STYLE_LIST, ""VIOLET"Zmiana typu eventu", s, "Ustaw", "Cofnij");
                }

                default: {
                    new settingid = listitem - 3;
                    switch(settingid) {
                        case EVENT_SETTING_VEHICLE: showDialog(playerid, DIALOG_EVENT_VEHICLE, DIALOG_STYLE_INPUT, ""RED"Ustaw pojazd eventowy", ""WHITE"Wpisz poni¿ej nazwê pojazdu, który bêdzie móg³ byæ spawnowany przez uczestników:", "Ustaw", "Cofnij");

                        default: {
                            EventSettings[settingid] = !EventSettings[settingid];

                            foreach(new i : Event) msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} %s ustawienie: {c}%s{/c}.", getNick(playerid), playerid, (EventSettings[settingid]) ? ("w³¹czy³(a)") : ("wy³¹czy³(a)"), eventSettingData[settingid]);
                            msg(playerid, C_CORAL, "%s ustawienie {c}%s{/c} na evencie.", (EventSettings[settingid]) ? ("W³¹czy³eœ(aœ)") : ("Wy³¹czy³eœ(aœ)"), eventSettingData[settingid]);
                        }
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}