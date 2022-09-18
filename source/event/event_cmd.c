stock GetEventType(typeid)
{
    new name[10];
    switch(typeid) {
        case EVENT_TYPE_NONE: name = "---";
        case EVENT_TYPE_DM: name = "DM";
        case EVENT_TYPE_NODM: name = "NoDM";
        case EVENT_TYPE_TDM: name = "TeamDM";
        case EVENT_TYPE_1V1: name = "1 vs 1";
        case EVENT_TYPE_10HP: name = "10HP";
    }
    return name;
}

alias:evtype("evtyp")
CMD:evtype(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new type[10];
    if(sscanf(params, "s[10]", type)) return syntaxError(playerid, "/evtype [dm/nodm/10hp/tdm]");

    if(strmatch(type, "dm", true)) EventType = EVENT_TYPE_DM;
    else if(strmatch(type, "nodm", true)) EventType = EVENT_TYPE_NODM;
    else if(strmatch(type, "10hp", true)) EventType = EVENT_TYPE_10HP;
    else if(strmatch(type, "1v1", true)) {
        if(EventSpawnDuel[0][0] == 0.0 || EventSpawnDuel[1][0] == 0.0) return sendError(playerid, ""RED"Nie ustawi³eœ(aœ) pozycji spawnowania siê graczy.\n"RED"Ustawisz je komend¹: {c}/evduelpos [1/2]{/c}.");
        EventType = EVENT_TYPE_1V1;
    }
    else if(strmatch(type, "tdm", true)) {
        if(EventSpawnTDM[0][0] == 0.0 || EventSpawnTDM[1][0] == 0.0) return sendError(playerid, ""RED"Nie ustawi³eœ(aœ) pozycji spawnowania siê dru¿yn.\n"RED"Ustawisz je komend¹: {c}/evtdmspawn [1/2]{/c}.");

        msg(playerid, C_CORAL, "Aby przygotowaæ duel, u¿yj: {c}/evduel [gracz 1] [gracz 2] [bron 1] [bron 2]");
        EventType = EVENT_TYPE_TDM;
    }
    else return sendError(playerid, ""RED"Podano nieprawid³owy typ prowadzonego eventu.");

    new teamid;
    foreach(new i : Event) {
        ResetPlayerWeapons(i);

        switch(EventType) {
            case EVENT_TYPE_DM, EVENT_TYPE_1V1: SetHealth(i, 100.0, 100.0);
            case EVENT_TYPE_NODM: SetHealth(i, 100.0, 0.0), giveWeapon(i, WEAPON_CAMERA, 1000);
            case EVENT_TYPE_10HP: SetHealth(i, 10.0, 0.0);
            case EVENT_TYPE_TDM: {
                SetHealth(i, 100.0, 100.0);
                
                teamid = !teamid;
                switch(teamid) {
                    case 0: {
                        Teleport(i, false, unpackXYZR(EventSpawnTDM[0]), EventInterior, VW_EVENT);
                        SetPlayerColor(i, C_GREEN);

                        msg(i, C_CORAL, "Zosta³eœ(aœ) przeniesiony do dru¿yny: "GREEN"ZIELONYCH"CORAL".");
                        SetPlayerTeam(i, teamid);
                    }

                    case 1: {
                        Teleport(i, false, unpackXYZR(EventSpawnTDM[1]), EventInterior, VW_EVENT);
                        SetPlayerColor(i, C_YELLOW);

                        msg(i, C_CORAL, "Zosta³eœ(aœ) przeniesiony do dru¿yny: "YELLOW"¯Ó£TYCH"CORAL".");
                        SetPlayerTeam(i, teamid);
                    }
                }
            }
        }
        msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) typ wydarzenia jako: {c}%s{/c}.", getNick(playerid), playerid, GetEventType(EventType));
    }
    return msg(playerid, C_CORAL, "Ustawi³eœ(aœ) typ wydarzenia jako: {c}%s{/c}.", GetEventType(EventType));
}

CMD:evtdmspawn(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new type;
    if(sscanf(params, "d", type)) return syntaxError(playerid, "/evtdmspawn [1/2]");
    if(type < 1 || type > 2) return sendError(playerid, ""RED"Mo¿esz ustawiæ spawn dru¿yny {c}1 albo 2{/c}.");

    new Float:pos[4];
    getPosition(playerid, false, unpackXYZR(pos));
    for(new i = 0; i < 4; i++) EventSpawnTDM[type - 1][i] = pos[i];
    return msg(playerid, C_CORAL, "Ustawi³eœ(aœ) pozycjê spawnuj¹c¹ siê dla {c}dru¿yny %d{/c}.", type), 1;
}

CMD:evduelpos(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new type;
    if(sscanf(params, "d", type)) return syntaxError(playerid, "/evduelpos [1/2]");
    if(type < 1 || type > 2) return sendError(playerid, ""RED"Mo¿esz ustawiæ spawn dru¿yny {c}1 albo 2{/c}.");

    new Float:pos[4];
    getPosition(playerid, false, unpackXYZR(pos));
    for(new i = 0; i < 4; i++) EventSpawnDuel[type - 1][i] = pos[i];
    return msg(playerid, C_CORAL, "Ustawi³eœ(aœ) pozycjê spawnuj¹c¹ siê dla {c}dru¿yny %d{/c}.", type), 1;
}

CMD:evduel(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);
    if(EventType != EVENT_TYPE_1V1) return sendError(playerid, ""RED"Nie prowadzisz eventu {c}1 vs 1{/c}. Aby zmieniæ typ eventu, wpisz: {c}/evtype 1v1{/c}.");
    if(EventDuelStarted) return sendError(playerid, ""RED"W³aœnie trwa pojedynek duel na evencie!");

    new targetid, enemyid, weap1, weap2;
    if(sscanf(params, "dddd", targetid, enemyid, weap1, weap2)) return syntaxError(playerid, "/evduel [gracz 1] [gracz 2] [bron 1] [bron 2]");
    if(!IsPlayerConnected(targetid)) return sendError(playerid, ""RED"Gracz 1 nie jest po³¹czony z serwerem.");
    if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Gracz 1 nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");
    if(!IsPlayerConnected(enemyid)) return sendError(playerid, ""RED"Gracz 2 nie jest po³¹czony z serwerem.");
    if(!Iter_Contains(Event, enemyid)) return sendError(playerid, ""RED"Gracz 2 nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");
    if(targetid == enemyid || targetid == playerid || enemyid == playerid) return sendError(playerid, ""RED"Nie mo¿esz daæ tego samego gracza jako przeciwnika, tak¿e siebie!"); 
    if(!IsValidWeapon(weap1)) return sendError(playerid, ""RED"Nieprawid³owe ID broni #1.");
    if(!IsValidWeapon(weap2)) return sendError(playerid, ""RED"Nieprawid³owe ID broni #1.");

    EventDuelCount ++;
    EventDuelEnemy[0] = targetid;
    EventDuelEnemy[1] = enemyid;
    EventDuelStarted = true;

    playerData[targetid][player_god] = false;                       playerData[enemyid][player_god] = false;
    ResetPlayerWeapons(targetid);                                   ResetPlayerWeapons(enemyid);
    SetHealth(targetid, 100.0, 100.0);                              SetHealth(enemyid, 100.0, 100.0);
    setPosition(targetid, false, unpackXYZR(EventSpawnDuel[0]));    setPosition(enemyid, false, unpackXYZR(EventSpawnDuel[1]));
    giveWeapon(targetid, weap1, 1000);                              giveWeapon(enemyid, weap1, 1000);  
    giveWeapon(targetid, weap2, 1000);                              giveWeapon(enemyid, weap2, 1000);  
    FreezePlayer(targetid, 3);                                      FreezePlayer(targetid, 3);

    foreach(new i : Event) {
        if(i != targetid || i != enemyid) TogglePlayerControllable(i, false);
        setOdliczanie(i, 3);

        msg(i, -1, " ");
        msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} przygotowa³(a) #%d pojedynek!", getNick(playerid), playerid, EventDuelCount);
        msg(i, C_CORAL, "Za chwilê odbêdzie siê walka: {c}%s vs %s{/c}! "LY"(%s + %s)", getNick(targetid), getNick(enemyid), WeaponNames[weap1], WeaponNames[weap2]);
    }
    return msg(playerid, C_CORAL, "Przygotowa³eœ(aœ) #%d pojedynek {c}%s vs %s{/c} na {c}%s + %s{/c}.", EventDuelCount, getNick(targetid), getNick(enemyid), WeaponNames[weap1], WeaponNames[weap2]), 1;
}

alias:evnagroda("evgift")
CMD:evnagroda(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) {
        new money = math_random(1000, 3000);
        foreach(new i : Event) {
            giveMoney(i, money);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) ka¿demu nagrodê: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
        }

        msg(playerid, C_CORAL, "Da³eœ(aœ) ka¿demu uczestnikowi {c}eventu{/c} nagrodê: {c}%s${/c}.", strdot(money));
        return msg(playerid, C_CORAL, "Mo¿esz równie¿ u¿yæ: {c}/evnagroda [id gracza]{/c}, by móc nadaæ nagrodê konkretnemu graczowi."), 1;   
    }
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

    new money = math_random(1000, 3000);
    giveMoney(id, money);
    msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) Tobie nagrodê: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
    return msg(playerid, C_CORAL, "Da³eœ(aœ) uczestnikowi {c}%s(%d){/c} nagrodê: {c}%s${/c}.", getNick(id), id,  strdot(money)), 1;
}

alias:evweapon("evgun")
CMD:evweapon(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[90], weaponid, _ammo;
    if(sscanf(params, "s[90]dd", temp, weaponid, _ammo)) return syntaxError(playerid, "/evweapon [id (all = wszyscy)] [id broni] [amunicja]");
    if(!IsValidWeapon(weaponid)) return sendError(playerid, ""RED"Wprowadzono nieprawid³owe ID broni.");
    if(_ammo < 0 || _ammo > 9999) return sendError(playerid, ""RED"Dozwolony przedzia³ amunicji to: {c}1-9999{/c}.");

    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            giveWeapon(i, weaponid, _ammo);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) ka¿demu {c}%s{/c} z {c}%d{/c} amunicji!", getNick(playerid), playerid, WeaponNames[weaponid], _ammo);
        }
        msg(playerid, C_CORAL, "Da³eœ(aœ) ka¿demu uczestnikowi {c}%s{/c} z {c}%d{/c} amunicji!", WeaponNames[weaponid], _ammo);
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        giveWeapon(id, weaponid, _ammo);
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) Tobie {c}%s{/c} z {c}%d{/c} amunicji!", getNick(playerid), playerid, WeaponNames[weaponid], _ammo);
        msg(playerid, C_CORAL, "Da³eœ(aœ) uczestnikowi {c}%s(%d){/c} broñ {c}%s{/c} z {c}%d{/c} amunicji.", getNick(id), id, WeaponNames[weaponid], _ammo);
    }
    return 1;
}

CMD:evfreeze(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evfreeze [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            TogglePlayerControllable(i, false);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zamrozi³(a) ka¿dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Zamrozi³eœ(aœ) ka¿dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        TogglePlayerControllable(id, false);
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zamrozi³(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Zamrozi³eœ(aœ) uczestnika {c}%s(%d){/c}.", getNick(id), id);
    }
    return 1;
}

CMD:evdisarm(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evdisarm [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            ResetPlayerWeapons(i);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zamrozi³(a) ka¿dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Rozbroi³eœ(aœ) ka¿dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        ResetPlayerWeapons(id);
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} rozbroi³(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Rozbroi³eœ(aœ) uczestnika {c}%s(%d){/c}.", getNick(id), id);
    }
    return 1;
}

CMD:evunfreeze(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evunfreeze [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            TogglePlayerControllable(i, true);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} odmrozi³(a) ka¿dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Odmrozi³eœ(aœ) ka¿dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        TogglePlayerControllable(id, true);
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} odmrozi³(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Odmrozi³eœ(aœ) uczestnika {c}%s(%d){/c}.", getNick(id), id);
    }
    return 1;
}

alias:evadd("evdodaj")
CMD:evadd(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/evadd [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz jest na evencie.");
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Nie mo¿esz teraz zaprosiæ tego gracza na event.");

    inline onEventInvite(pp, dd, rr, ll, string:ii[]) {
        if(!EventStarted) return 1;

        #pragma unused pp, dd, ll, ii
        if(!rr) {
            msg(id, C_CORAL, "Odrzuci³eœ(aœ) zaproszenie na event od {c}%s(%d){/c}.", getNick(playerid), playerid);
            return msg(playerid, C_CORAL, "Gracz {c}%s(%d){/c} odrzuci³(a) Twoje zaproszenie na event!", getNick(id), id);
        }

        SavePlayerPosition(id);
        ResetPlayerWeapons(id);
        SetHealth(id, 100.0, 0.0);
        Teleport(id, false, unpackXYZR(EventPosition), EventInterior, VW_EVENT, false);
        Iter_Add(Event, id);
        msg(id, C_LBLUE, "Zosta³eœ(aœ) dodany(a) do wydarzenia! Aby siê z niego wypisaæ, wpisz: {c}/exit{/c}.");
    }
    msg(playerid, C_CORAL, "Wys³a³eœ(aœ) zaproszenie do eventu graczowi: {c}%s(%d){/c}.", getNick(id), id);

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Otrzyma³eœ(aœ) zaproszenie na event "LY"%s"WHITE".\n"WHITE"Prowadz¹cym eventu jest: "RED"%s(%d)"WHITE".\n"WHITE"Aktualna liczba uczestników wynosi: "BLUE2"%d %s"WHITE".\n\n"LRED"Kliknij poni¿ej odpowiedni przycisk, aby zaakceptowaæ zaproszenie.", EventName, getNick(playerid), playerid, Iter_Count(Event), ConvertTimeFormat(Iter_Count(Event), "osób", "osoba", "osoby", "osób"));
    return Dialog_ShowCallback(id, using inline onEventInvite, DIALOG_STYLE_MSGBOX, ""YELLOW"Zaproszenie na event", s, "Akceptuj", "Odrzuæ"), 1;
}

alias:evdelete("evusun")
CMD:evdelete(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/evadd [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

    LoadPlayerPosition(id);
    Iter_Remove(Event, id);

    msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} wyrzuci³(a) Ciebie z eventu!", getNick(playerid), playerid);
    return msg(playerid, C_CORAL, "Wyrzuci³eœ(aœ) uczestnika {c}%s(%d){/c} z eventu!", getNick(id), id), 1;
}

CMD:evgod(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evgod [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            playerData[i][player_god] = true;
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) ka¿demu nieœmiertelnosæ!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da³eœ(aœ) ka¿demu {c}uczestnikowi{/c} wydarzenia nieœmiertelnoœæ.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        playerData[id][player_god] = true;
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} da³(a) Ci nieœmiertelnoœæ!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Da³eœ(aœ) nieœmiertelnoœæ uczestnikowi {c}%s(%d){/c}.", getNick(id), id);
    }
    return 1;
}

alias:evdelgod("evungod")
CMD:evdelgod(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evdelgod [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            playerData[i][player_god] = false;
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zabra³(a) ka¿demu nieœmiertelnosæ!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da³eœ(aœ) ka¿demu {c}uczestnikowi{/c} wydarzenia nieœmiertelnoœæ.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        playerData[id][player_god] = false;
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zabra³(a) Ci nieœmiertelnoœæ!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Zabra³eœ(aœ) nieœmiertelnoœæ uczestnikowi {c}%s(%d){/c}.", getNick(id), id);
    }
    return 1;
}

CMD:evinvisible(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evinvisible [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0x00));
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) ka¿demu niewidzialnoœæ na radarze!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da³eœ(aœ) ka¿demu {c}uczestnikowi{/c} niewidzialnoœæ na radarze.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        SetPlayerColor(id, setAlpha(GetPlayerColor(id), 0x00));
        msg(id, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) Tobie niewidzialnoœæ na radarze!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Ustawi³eœ(aœ) niewidzialnoœæ uczestnikowi {c}%s(%d){/c} na radarze.", getNick(id), id);
    }
    return 1;
}

alias:evczas("evtime")
CMD:evczas(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new hourid = strval(params);
    if(isnull(params) || !(0 <= hourid <= 24)) return syntaxError(playerid, "/evczas [0-24]");

    foreach(new i : Event) {
        SetPlayerTime(i, hourid, 0);
        msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zmieni³(a) godzinê na: {c}%02d:00{/c}.", getNick(playerid), playerid, hourid);
    }
    return msg(playerid, C_CORAL, "Ustawi³eœ(aœ) godzinê na evencie na: {c}%02d:00{/c}.", hourid);
}

alias:evpogoda("evweather")
CMD:evpogoda(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new weatherid = strval(params);
    if(isnull(params) || weatherid < 0 || weatherid > 20) return syntaxError(playerid, "/evpogoda [0-20]");
    
    foreach(new i : Event) {
        SetPlayerWeather(i, weatherid);
        msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zmieni³(a) pogodê na: {c}ID: %d{/c}.", getNick(playerid), playerid, weatherid);
    }
    return msg(playerid, C_CORAL, "Ustawi³eœ(aœ) pogodê na evencie na: {c}ID: %d{/c}.", weatherid);
}

CMD:evcolor(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/evcolor [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

    SetPVarInt(playerid, "player_event_targetid", id);

    s[0] = EOS;
    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
    return showDialog(playerid, DIALOG_EVENT_COLOR, DIALOG_STYLE_LIST, ""BLUE"Wybierz kolor gracza", #, "Ustaw", "Zamknij", 10), 1;
}

alias:evhealth("evsethp", "evheal")
CMD:evhealth(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new check[32], Float:health;
    if(sscanf(params, "s[32]f", check, health)) return syntaxError(playerid, "/evhealth [id/all] [zycie]");
    if(health < 1.0 || health > 100.0) return sendError(playerid, ""RED"Wartoœæ {c}¿ycia{/c} musi mieœciæ siê w przedziale: {c}1-100{/c} HP.");

    if(strmatch(check, "all", true)) {
        foreach(new i : Event) {
            SetPlayerHealth(i, health);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) ka¿demu {c}%.1f{/c} ¿ycia.", getNick(playerid), playerid, health);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");
        
        SetPlayerHealth(targetid, health);

        msg(targetid, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) Tobie {c}%.1f{/c} ¿ycia!", getNick(playerid), playerid, health);
        msg(playerid, C_CORAL, "Ustawi³eœ(aœ) {c}%.1f{/c} ¿ycia graczowi {c}%s(%d){/c}.", health, getNick(targetid), targetid);
    }
    return 1;
}

alias:evarmor("evarmour", "evsetar")
CMD:evarmor(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new check[32], Float:health;
    if(sscanf(params, "s[32]f", check, health)) return syntaxError(playerid, "/evarmor [id/all] [zycie]");
    if(health < 1.0 || health > 100.0) return sendError(playerid, ""RED"Wartoœæ {c}pancerza{/c} musi mieœciæ siê w przedziale: {c}1-100{/c} AR.");

    if(strmatch(check, "all", true)) {
        foreach(new i : Event) {
            SetPlayerArmour(i, health);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) ka¿demu {c}%.1f{/c} pancerza.", getNick(playerid), playerid, health);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");
        
        SetPlayerArmour(targetid, health);

        msg(targetid, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} ustawi³(a) Tobie {c}%.1f{/c} pancerza!", getNick(playerid), playerid, health);
        msg(playerid, C_CORAL, "Ustawi³eœ(aœ) {c}%.1f{/c} pancerza graczowi {c}%s(%d){/c}.", health, getNick(targetid), targetid);
    }
    return 1;
}

alias:evtph("evtphere")
CMD:evtph(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new check[32];
    if(sscanf(params, "s[32]", check)) return syntaxError(playerid, "/evtph [id/all]"); 

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if(strmatch(check, "all", true)) {
        foreach(new i : Event) {
            SetPlayerPos(i, x, y, z);
            msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} teleportowa³(a) ka¿dego do siebie.", getNick(playerid), playerid);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        SetPlayerPos(targetid, x, y + 0.20, z + 0.35);

        msg(targetid, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} teleportowa³(a) Ciebie do siebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Teleportowa³eœ(aœ) gracza {c}%s(%d){/c} do siebie.", getNick(targetid), targetid);
    }
    return 1;
}

alias:evtpt("evtpto")
CMD:evtpt(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new targetid;
    if(sscanf(params, "d", targetid)) return syntaxError(playerid, "/evtpt [id]"); 
    if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(targetid, x, y, z);

    msg(targetid, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} teleportowa³(a) siê do Ciebie!", getNick(playerid), playerid);
    msg(playerid, C_CORAL, "Teleportowa³eœ(aœ) siê do gracza: {c}%s(%d){/c}.", getNick(targetid), targetid);
    return SetPlayerPos(playerid, x, y + 0.20, z + 0.35), 1;
}

alias:evcar("evvehicle")
CMD:evcar(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new modelid[32];
    if(sscanf(params, "s[32]", modelid)) return syntaxError(playerid, "/evcar [model]");
    if(!GetVehicleNameByID(modelid)) return sendError(playerid, ""RED"Podany model pojazdu jest nieprawid³owy.");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);

    if(IsValidVehicle(pEventVehicle[playerid])) DestroyVehicle(pEventVehicle[playerid]);
    msg(playerid, C_CORAL, "Stworzy³eœ(aœ) {c}%s (ID: %d){/c}.", VehicleNames[GetVehicleNameByID(modelid) - 400], GetVehicleNameByID(modelid));

    pEventVehicle[playerid] = CreateVehicle(GetVehicleNameByID(modelid), x, y, z, a, -1, -1, 60);
    LinkVehicleToInterior(pEventVehicle[playerid], GetPlayerInterior(playerid));
    SetVehicleVirtualWorld(pEventVehicle[playerid], GetPlayerVirtualWorld(playerid));
    SetVehicleToRespawn(pEventVehicle[playerid]);
    return PutPlayerInVehicle(playerid, pEventVehicle[playerid], 0), 1;
}

CMD:evspawn(playerid)
{
    if(!EventStarted) return sendError(playerid, ""RED"Event nie wystartowa³!");
    if(!Iter_Contains(Event, playerid)) return sendError(playerid, ""RED"Nie jesteœ zapisany(a) na event!");
    if(EventVehicle == -1) return sendError(playerid, ""RED"Prowadz¹cy nie ustawi³ ¿adnego pojazdu eventowego.");
    if(EventLeader == playerid) return sendError(playerid, form(""RED"Jesteœ prowadz¹cym, u¿yj: {c}/evcar %s{/c}.", VehicleNames[EventVehicle - 400]));

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);

    if(IsValidVehicle(pEventVehicle[playerid])) DestroyVehicle(pEventVehicle[playerid]);
    msg(playerid, C_LORANGE, "Stworzy³eœ(aœ) pojazd eventowy: {c}%s (ID: %d){/c}.", VehicleNames[EventVehicle - 400], EventVehicle);

    pEventVehicle[playerid] = CreateVehicle(EventVehicle, x, y, z, a, -1, -1, 60);
    LinkVehicleToInterior(pEventVehicle[playerid], GetPlayerInterior(playerid));
    SetVehicleVirtualWorld(pEventVehicle[playerid], GetPlayerVirtualWorld(playerid));
    SetVehicleToRespawn(pEventVehicle[playerid]);
    return PutPlayerInVehicle(playerid, pEventVehicle[playerid], 0), 1;
}

CMD:evkick50(playerid)
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new iterate_max, Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    foreach(new i : Event) {
        if(!IsPlayerInRangeOfPoint(i, 50.0, x, y, z)) {
            CheckBet(i, BET_EVENT, false);
            msg(i, C_DRED, "Oddali³eœ(aœ) siê od {c}prowadz¹cego{/c} eventu, wiêc zosta³eœ(aœ) z niego wyrzucony(a)!");

            SetPlayerColor(i, setAlpha(GetPlayerColor(playerid), 0xFF));
            LoadPlayerPosition(i);

            Iter_SafeRemove(Event, i, iterate_max);
            playerid = iterate_max;
        }

        msgAll(C_CORAL, "Prowadz¹cy {c}%s(%d){/c} wyrzuci³(a) wszystkich oddalonych uczestników eventu!", getNick(playerid), playerid);
    }
    return msg(playerid, C_CORAL, "Wyrzuci³eœ(aœ) wszystkich {c}oddalonych{/c} uczestników z eventu!"), 1;
}

CMD:evzone(playerid)
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    showGameDraw(playerid, "~y~TWORZENIE STREFY EVENTU~n~~y~PORUSZAJ SIE ~r~STRZALKAMI~y~!", 10);
    pEventZone[playerid] = true;

    new Float:unusedFloat;
    GetPlayerPos(playerid, pEventZonePos[playerid][0], pEventZonePos[playerid][1], unusedFloat);
    return GetPlayerPos(playerid, pEventZonePos[playerid][2], pEventZonePos[playerid][3], unusedFloat);
}
