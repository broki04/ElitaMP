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
        if(EventSpawnDuel[0][0] == 0.0 || EventSpawnDuel[1][0] == 0.0) return sendError(playerid, ""RED"Nie ustawi�e�(a�) pozycji spawnowania si� graczy.\n"RED"Ustawisz je komend�: {c}/evduelpos [1/2]{/c}.");
        EventType = EVENT_TYPE_1V1;
    }
    else if(strmatch(type, "tdm", true)) {
        if(EventSpawnTDM[0][0] == 0.0 || EventSpawnTDM[1][0] == 0.0) return sendError(playerid, ""RED"Nie ustawi�e�(a�) pozycji spawnowania si� dru�yn.\n"RED"Ustawisz je komend�: {c}/evtdmspawn [1/2]{/c}.");

        msg(playerid, C_CORAL, "Aby przygotowa� duel, u�yj: {c}/evduel [gracz 1] [gracz 2] [bron 1] [bron 2]");
        EventType = EVENT_TYPE_TDM;
    }
    else return sendError(playerid, ""RED"Podano nieprawid�owy typ prowadzonego eventu.");

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

                        msg(i, C_CORAL, "Zosta�e�(a�) przeniesiony do dru�yny: "GREEN"ZIELONYCH"CORAL".");
                        SetPlayerTeam(i, teamid);
                    }

                    case 1: {
                        Teleport(i, false, unpackXYZR(EventSpawnTDM[1]), EventInterior, VW_EVENT);
                        SetPlayerColor(i, C_YELLOW);

                        msg(i, C_CORAL, "Zosta�e�(a�) przeniesiony do dru�yny: "YELLOW"�ӣTYCH"CORAL".");
                        SetPlayerTeam(i, teamid);
                    }
                }
            }
        }
        msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) typ wydarzenia jako: {c}%s{/c}.", getNick(playerid), playerid, GetEventType(EventType));
    }
    return msg(playerid, C_CORAL, "Ustawi�e�(a�) typ wydarzenia jako: {c}%s{/c}.", GetEventType(EventType));
}

CMD:evtdmspawn(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new type;
    if(sscanf(params, "d", type)) return syntaxError(playerid, "/evtdmspawn [1/2]");
    if(type < 1 || type > 2) return sendError(playerid, ""RED"Mo�esz ustawi� spawn dru�yny {c}1 albo 2{/c}.");

    new Float:pos[4];
    getPosition(playerid, false, unpackXYZR(pos));
    for(new i = 0; i < 4; i++) EventSpawnTDM[type - 1][i] = pos[i];
    return msg(playerid, C_CORAL, "Ustawi�e�(a�) pozycj� spawnuj�c� si� dla {c}dru�yny %d{/c}.", type), 1;
}

CMD:evduelpos(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new type;
    if(sscanf(params, "d", type)) return syntaxError(playerid, "/evduelpos [1/2]");
    if(type < 1 || type > 2) return sendError(playerid, ""RED"Mo�esz ustawi� spawn dru�yny {c}1 albo 2{/c}.");

    new Float:pos[4];
    getPosition(playerid, false, unpackXYZR(pos));
    for(new i = 0; i < 4; i++) EventSpawnDuel[type - 1][i] = pos[i];
    return msg(playerid, C_CORAL, "Ustawi�e�(a�) pozycj� spawnuj�c� si� dla {c}dru�yny %d{/c}.", type), 1;
}

CMD:evduel(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);
    if(EventType != EVENT_TYPE_1V1) return sendError(playerid, ""RED"Nie prowadzisz eventu {c}1 vs 1{/c}. Aby zmieni� typ eventu, wpisz: {c}/evtype 1v1{/c}.");
    if(EventDuelStarted) return sendError(playerid, ""RED"W�a�nie trwa pojedynek duel na evencie!");

    new targetid, enemyid, weap1, weap2;
    if(sscanf(params, "dddd", targetid, enemyid, weap1, weap2)) return syntaxError(playerid, "/evduel [gracz 1] [gracz 2] [bron 1] [bron 2]");
    if(!IsPlayerConnected(targetid)) return sendError(playerid, ""RED"Gracz 1 nie jest po��czony z serwerem.");
    if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Gracz 1 nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");
    if(!IsPlayerConnected(enemyid)) return sendError(playerid, ""RED"Gracz 2 nie jest po��czony z serwerem.");
    if(!Iter_Contains(Event, enemyid)) return sendError(playerid, ""RED"Gracz 2 nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");
    if(targetid == enemyid || targetid == playerid || enemyid == playerid) return sendError(playerid, ""RED"Nie mo�esz da� tego samego gracza jako przeciwnika, tak�e siebie!"); 
    if(!IsValidWeapon(weap1)) return sendError(playerid, ""RED"Nieprawid�owe ID broni #1.");
    if(!IsValidWeapon(weap2)) return sendError(playerid, ""RED"Nieprawid�owe ID broni #1.");

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
        msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} przygotowa�(a) #%d pojedynek!", getNick(playerid), playerid, EventDuelCount);
        msg(i, C_CORAL, "Za chwil� odb�dzie si� walka: {c}%s vs %s{/c}! "LY"(%s + %s)", getNick(targetid), getNick(enemyid), WeaponNames[weap1], WeaponNames[weap2]);
    }
    return msg(playerid, C_CORAL, "Przygotowa�e�(a�) #%d pojedynek {c}%s vs %s{/c} na {c}%s + %s{/c}.", EventDuelCount, getNick(targetid), getNick(enemyid), WeaponNames[weap1], WeaponNames[weap2]), 1;
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) ka�demu nagrod�: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
        }

        msg(playerid, C_CORAL, "Da�e�(a�) ka�demu uczestnikowi {c}eventu{/c} nagrod�: {c}%s${/c}.", strdot(money));
        return msg(playerid, C_CORAL, "Mo�esz r�wnie� u�y�: {c}/evnagroda [id gracza]{/c}, by m�c nada� nagrod� konkretnemu graczowi."), 1;   
    }
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

    new money = math_random(1000, 3000);
    giveMoney(id, money);
    msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) Tobie nagrod�: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
    return msg(playerid, C_CORAL, "Da�e�(a�) uczestnikowi {c}%s(%d){/c} nagrod�: {c}%s${/c}.", getNick(id), id,  strdot(money)), 1;
}

alias:evweapon("evgun")
CMD:evweapon(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[90], weaponid, _ammo;
    if(sscanf(params, "s[90]dd", temp, weaponid, _ammo)) return syntaxError(playerid, "/evweapon [id (all = wszyscy)] [id broni] [amunicja]");
    if(!IsValidWeapon(weaponid)) return sendError(playerid, ""RED"Wprowadzono nieprawid�owe ID broni.");
    if(_ammo < 0 || _ammo > 9999) return sendError(playerid, ""RED"Dozwolony przedzia� amunicji to: {c}1-9999{/c}.");

    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            giveWeapon(i, weaponid, _ammo);
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) ka�demu {c}%s{/c} z {c}%d{/c} amunicji!", getNick(playerid), playerid, WeaponNames[weaponid], _ammo);
        }
        msg(playerid, C_CORAL, "Da�e�(a�) ka�demu uczestnikowi {c}%s{/c} z {c}%d{/c} amunicji!", WeaponNames[weaponid], _ammo);
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        giveWeapon(id, weaponid, _ammo);
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) Tobie {c}%s{/c} z {c}%d{/c} amunicji!", getNick(playerid), playerid, WeaponNames[weaponid], _ammo);
        msg(playerid, C_CORAL, "Da�e�(a�) uczestnikowi {c}%s(%d){/c} bro� {c}%s{/c} z {c}%d{/c} amunicji.", getNick(id), id, WeaponNames[weaponid], _ammo);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zamrozi�(a) ka�dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Zamrozi�e�(a�) ka�dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        TogglePlayerControllable(id, false);
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zamrozi�(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Zamrozi�e�(a�) uczestnika {c}%s(%d){/c}.", getNick(id), id);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zamrozi�(a) ka�dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Rozbroi�e�(a�) ka�dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        ResetPlayerWeapons(id);
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} rozbroi�(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Rozbroi�e�(a�) uczestnika {c}%s(%d){/c}.", getNick(id), id);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} odmrozi�(a) ka�dego uczestnika!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Odmrozi�e�(a�) ka�dego {c}uczestnika{/c} wydarzenia.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        TogglePlayerControllable(id, true);
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} odmrozi�(a) Ciebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Odmrozi�e�(a�) uczestnika {c}%s(%d){/c}.", getNick(id), id);
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
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Nie mo�esz teraz zaprosi� tego gracza na event.");

    inline onEventInvite(pp, dd, rr, ll, string:ii[]) {
        if(!EventStarted) return 1;

        #pragma unused pp, dd, ll, ii
        if(!rr) {
            msg(id, C_CORAL, "Odrzuci�e�(a�) zaproszenie na event od {c}%s(%d){/c}.", getNick(playerid), playerid);
            return msg(playerid, C_CORAL, "Gracz {c}%s(%d){/c} odrzuci�(a) Twoje zaproszenie na event!", getNick(id), id);
        }

        SavePlayerPosition(id);
        ResetPlayerWeapons(id);
        SetHealth(id, 100.0, 0.0);
        Teleport(id, false, unpackXYZR(EventPosition), EventInterior, VW_EVENT, false);
        Iter_Add(Event, id);
        msg(id, C_LBLUE, "Zosta�e�(a�) dodany(a) do wydarzenia! Aby si� z niego wypisa�, wpisz: {c}/exit{/c}.");
    }
    msg(playerid, C_CORAL, "Wys�a�e�(a�) zaproszenie do eventu graczowi: {c}%s(%d){/c}.", getNick(id), id);

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Otrzyma�e�(a�) zaproszenie na event "LY"%s"WHITE".\n"WHITE"Prowadz�cym eventu jest: "RED"%s(%d)"WHITE".\n"WHITE"Aktualna liczba uczestnik�w wynosi: "BLUE2"%d %s"WHITE".\n\n"LRED"Kliknij poni�ej odpowiedni przycisk, aby zaakceptowa� zaproszenie.", EventName, getNick(playerid), playerid, Iter_Count(Event), ConvertTimeFormat(Iter_Count(Event), "os�b", "osoba", "osoby", "os�b"));
    return Dialog_ShowCallback(id, using inline onEventInvite, DIALOG_STYLE_MSGBOX, ""YELLOW"Zaproszenie na event", s, "Akceptuj", "Odrzu�"), 1;
}

alias:evdelete("evusun")
CMD:evdelete(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/evadd [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

    LoadPlayerPosition(id);
    Iter_Remove(Event, id);

    msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} wyrzuci�(a) Ciebie z eventu!", getNick(playerid), playerid);
    return msg(playerid, C_CORAL, "Wyrzuci�e�(a�) uczestnika {c}%s(%d){/c} z eventu!", getNick(id), id), 1;
}

CMD:evgod(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new temp[20];
    if(sscanf(params, "s[20]", temp)) return syntaxError(playerid, ""RED"/evgod [id (all = wszyscy)]");
    
    if(strmatch(temp, "all", true)) {
        foreach(new i : Event) {
            playerData[i][player_god] = true;
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) ka�demu nie�miertelnos�!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da�e�(a�) ka�demu {c}uczestnikowi{/c} wydarzenia nie�miertelno��.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        playerData[id][player_god] = true;
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} da�(a) Ci nie�miertelno��!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Da�e�(a�) nie�miertelno�� uczestnikowi {c}%s(%d){/c}.", getNick(id), id);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zabra�(a) ka�demu nie�miertelnos�!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da�e�(a�) ka�demu {c}uczestnikowi{/c} wydarzenia nie�miertelno��.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        playerData[id][player_god] = false;
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zabra�(a) Ci nie�miertelno��!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Zabra�e�(a�) nie�miertelno�� uczestnikowi {c}%s(%d){/c}.", getNick(id), id);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) ka�demu niewidzialno�� na radarze!", getNick(playerid), playerid);
        }
        msg(playerid, C_CORAL, "Da�e�(a�) ka�demu {c}uczestnikowi{/c} niewidzialno�� na radarze.");
    } else {
        new id = strval(temp);
        if(!IsPlayerConnected(id)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        SetPlayerColor(id, setAlpha(GetPlayerColor(id), 0x00));
        msg(id, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) Tobie niewidzialno�� na radarze!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Ustawi�e�(a�) niewidzialno�� uczestnikowi {c}%s(%d){/c} na radarze.", getNick(id), id);
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
        msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zmieni�(a) godzin� na: {c}%02d:00{/c}.", getNick(playerid), playerid, hourid);
    }
    return msg(playerid, C_CORAL, "Ustawi�e�(a�) godzin� na evencie na: {c}%02d:00{/c}.", hourid);
}

alias:evpogoda("evweather")
CMD:evpogoda(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new weatherid = strval(params);
    if(isnull(params) || weatherid < 0 || weatherid > 20) return syntaxError(playerid, "/evpogoda [0-20]");
    
    foreach(new i : Event) {
        SetPlayerWeather(i, weatherid);
        msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} zmieni�(a) pogod� na: {c}ID: %d{/c}.", getNick(playerid), playerid, weatherid);
    }
    return msg(playerid, C_CORAL, "Ustawi�e�(a�) pogod� na evencie na: {c}ID: %d{/c}.", weatherid);
}

CMD:evcolor(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/evcolor [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!Iter_Contains(Event, id)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

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
    if(health < 1.0 || health > 100.0) return sendError(playerid, ""RED"Warto�� {c}�ycia{/c} musi mie�ci� si� w przedziale: {c}1-100{/c} HP.");

    if(strmatch(check, "all", true)) {
        foreach(new i : Event) {
            SetPlayerHealth(i, health);
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) ka�demu {c}%.1f{/c} �ycia.", getNick(playerid), playerid, health);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");
        
        SetPlayerHealth(targetid, health);

        msg(targetid, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) Tobie {c}%.1f{/c} �ycia!", getNick(playerid), playerid, health);
        msg(playerid, C_CORAL, "Ustawi�e�(a�) {c}%.1f{/c} �ycia graczowi {c}%s(%d){/c}.", health, getNick(targetid), targetid);
    }
    return 1;
}

alias:evarmor("evarmour", "evsetar")
CMD:evarmor(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new check[32], Float:health;
    if(sscanf(params, "s[32]f", check, health)) return syntaxError(playerid, "/evarmor [id/all] [zycie]");
    if(health < 1.0 || health > 100.0) return sendError(playerid, ""RED"Warto�� {c}pancerza{/c} musi mie�ci� si� w przedziale: {c}1-100{/c} AR.");

    if(strmatch(check, "all", true)) {
        foreach(new i : Event) {
            SetPlayerArmour(i, health);
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) ka�demu {c}%.1f{/c} pancerza.", getNick(playerid), playerid, health);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");
        
        SetPlayerArmour(targetid, health);

        msg(targetid, C_CORAL, "Prowadz�cy {c}%s(%d){/c} ustawi�(a) Tobie {c}%.1f{/c} pancerza!", getNick(playerid), playerid, health);
        msg(playerid, C_CORAL, "Ustawi�e�(a�) {c}%.1f{/c} pancerza graczowi {c}%s(%d){/c}.", health, getNick(targetid), targetid);
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
            msg(i, C_CORAL, "Prowadz�cy {c}%s(%d){/c} teleportowa�(a) ka�dego do siebie.", getNick(playerid), playerid);
        }
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

        SetPlayerPos(targetid, x, y + 0.20, z + 0.35);

        msg(targetid, C_CORAL, "Prowadz�cy {c}%s(%d){/c} teleportowa�(a) Ciebie do siebie!", getNick(playerid), playerid);
        msg(playerid, C_CORAL, "Teleportowa�e�(a�) gracza {c}%s(%d){/c} do siebie.", getNick(targetid), targetid);
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
    if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo�esz go doda� komend�: {c}/evadd{/c}.");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(targetid, x, y, z);

    msg(targetid, C_CORAL, "Prowadz�cy {c}%s(%d){/c} teleportowa�(a) si� do Ciebie!", getNick(playerid), playerid);
    msg(playerid, C_CORAL, "Teleportowa�e�(a�) si� do gracza: {c}%s(%d){/c}.", getNick(targetid), targetid);
    return SetPlayerPos(playerid, x, y + 0.20, z + 0.35), 1;
}

alias:evcar("evvehicle")
CMD:evcar(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    new modelid[32];
    if(sscanf(params, "s[32]", modelid)) return syntaxError(playerid, "/evcar [model]");
    if(!GetVehicleNameByID(modelid)) return sendError(playerid, ""RED"Podany model pojazdu jest nieprawid�owy.");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);

    if(IsValidVehicle(pEventVehicle[playerid])) DestroyVehicle(pEventVehicle[playerid]);
    msg(playerid, C_CORAL, "Stworzy�e�(a�) {c}%s (ID: %d){/c}.", VehicleNames[GetVehicleNameByID(modelid) - 400], GetVehicleNameByID(modelid));

    pEventVehicle[playerid] = CreateVehicle(GetVehicleNameByID(modelid), x, y, z, a, -1, -1, 60);
    LinkVehicleToInterior(pEventVehicle[playerid], GetPlayerInterior(playerid));
    SetVehicleVirtualWorld(pEventVehicle[playerid], GetPlayerVirtualWorld(playerid));
    SetVehicleToRespawn(pEventVehicle[playerid]);
    return PutPlayerInVehicle(playerid, pEventVehicle[playerid], 0), 1;
}

CMD:evspawn(playerid)
{
    if(!EventStarted) return sendError(playerid, ""RED"Event nie wystartowa�!");
    if(!Iter_Contains(Event, playerid)) return sendError(playerid, ""RED"Nie jeste� zapisany(a) na event!");
    if(EventVehicle == -1) return sendError(playerid, ""RED"Prowadz�cy nie ustawi� �adnego pojazdu eventowego.");
    if(EventLeader == playerid) return sendError(playerid, form(""RED"Jeste� prowadz�cym, u�yj: {c}/evcar %s{/c}.", VehicleNames[EventVehicle - 400]));

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);

    if(IsValidVehicle(pEventVehicle[playerid])) DestroyVehicle(pEventVehicle[playerid]);
    msg(playerid, C_LORANGE, "Stworzy�e�(a�) pojazd eventowy: {c}%s (ID: %d){/c}.", VehicleNames[EventVehicle - 400], EventVehicle);

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
            msg(i, C_DRED, "Oddali�e�(a�) si� od {c}prowadz�cego{/c} eventu, wi�c zosta�e�(a�) z niego wyrzucony(a)!");

            SetPlayerColor(i, setAlpha(GetPlayerColor(playerid), 0xFF));
            LoadPlayerPosition(i);

            Iter_SafeRemove(Event, i, iterate_max);
            playerid = iterate_max;
        }

        msgAll(C_CORAL, "Prowadz�cy {c}%s(%d){/c} wyrzuci�(a) wszystkich oddalonych uczestnik�w eventu!", getNick(playerid), playerid);
    }
    return msg(playerid, C_CORAL, "Wyrzuci�e�(a�) wszystkich {c}oddalonych{/c} uczestnik�w z eventu!"), 1;
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
