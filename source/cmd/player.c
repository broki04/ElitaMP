alias:autor("autorzy", "author", "authors", "credits")
CMD:autor(playerid)
{
    s[0] = EOS;
    strcat(s, ""WHITE"new"RED"D"WHITE"eath"RED"M"WHITE"atch "GOLD""VERSION" "GREY"(update: "DATE")\n\n");

    strcat(s, ""WHITE"G³ówny autor: "RED"Broki\n");
    strcat(s, ""WHITE"Szata graficzna: "BLUE"Broki\n");
    strcat(s, ""WHITE"Niektóre pomys³y: "MINT"Lou, vcxz, Vester_, Tenshinn\n");
    strcat(s, ""WHITE"Obiekty: "LRED"Nesquik, Mr.A, Vester_, wiele serwerów\n\n");

    strcat(s, ""GOLD"Mapa wykorzystuje:\n");
    strcat(s, ""WHITE"MySQL BlueG, Streamer, Sscanf2, YSF, Progress2, Pawn.CMD, Dialog-Pages\n");
    strcat(s, ""WHITE"Pawn.Regex, Md-Sort, Samp_BCrypt, A_Zone, eSelection, PawnPlus, Weapon-Config\n");
    strcat(s, ""WHITE"ColAndreas, y_va, y_timers, y_dialog, y_inline_mysql, y_inline_timers\n\n");

    strcat(s, ""WHITE"Liczba linijek kodu: "RED""#LINES"\n");
    strcat(s, ""WHITE"Liczba modu³ów: "RED""#FILES"\n");
    format(s, sizeof(s), "%s"WHITE"Liczba obiektów: "RED"%s\n\n", s, strdot(CountDynamicObjects()));

    strcat(s, ""LRED"Nalegamy, aby jakiekolwiek b³êdy zg³aszaæ w³aœcicielom.");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Autorzy", s, "OK", #);
}

alias:kolory("colors", "kolor", "color")
CMD:kolory(playerid) 
{
    s[0] = EOS;
    for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
    return showDialog(playerid, DIALOG_KOLORY, DIALOG_STYLE_LIST, ""BLUE"Kolory", #, "Wybierz", "Zamknij", 10), 1;
}

alias:skin("skiny", "skins")
CMD:skin(playerid, params[])
{
    new id;
    if(sscanf(params, "d", id)) {
        new List:skins = list_new();
        for(new i = 0; i != 311; i++) { if(i == 74) continue; AddModelMenuItem(skins, i, form("ID: %d", i)); }

        new response[E_MODEL_SELECTION_INFO];
        await_arr(response) ShowAsyncModelSelectionMenu(playerid, "Skiny", skins);

        if(response[E_MODEL_SELECTION_RESPONSE] == MODEL_RESPONSE_SELECT) return callcmd::skin(playerid, form("%d", response[E_MODEL_SELECTION_MODELID]));
        return 1;
    }
    if(!IsValidSkin(id)) return sendError(playerid, ""RED"Wprowadzono nieprawid³owe ID skina.");

    playerData[playerid][player_skin] = id;
    return SetPlayerSkin(playerid, id), 1;
}

alias:ulecz("100hp", "zycie", "heal")
CMD:ulecz(playerid)
{
    if(getMoney(playerid) < 1000) return valueError(playerid, 1000);

    new Float:health; GetPlayerHealth(playerid, health);
    if(health >= 90.0) return sendError(playerid, ""RED"Twoja postaæ nie wymaga leczenia!");

    blockCMD(playerid, "block_ulecz", (pUlepszenia[playerid][ULEPSZENIE_HEAL]) ? (10) : (30));
    
    giveMoney(playerid, -1000);
    msg(playerid, C_GREEN, "Uleczy³eœ(aœ) siê za: {c}1000${/c}.");
    return SetPlayerHealth(playerid, 100.0);
}

alias:kamizelka("pancerz", "armor")
CMD:kamizelka(playerid)
{
    if(getMoney(playerid) < 1000) return valueError(playerid, 1000);

    new Float:health; GetPlayerArmour(playerid, health);
    if(health >= 90.0) return sendError(playerid, ""RED"Twoja postaæ nie wymaga opancerzenia!");

    blockCMD(playerid, "block_kamizelka", (pUlepszenia[playerid][ULEPSZENIE_HEAL]) ? (10) : (30));

    giveMoney(playerid, -1000);
    msg(playerid, C_GREEN, "Kupi³eœ(aœ) pancerz za: {c}1000${/c}.");
    return SetPlayerArmour(playerid, 100.0);
}

alias:invisible("niewidzialnosc")
CMD:invisible(playerid)
{
    if(!pUlepszenia[playerid][ULEPSZENIE_INVISIBLE]) return ulepszenieError(playerid, ULEPSZENIE_INVISIBLE);
    return SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0x00)), 1;
}

alias:naj("best")
CMD:naj(playerid)
{
    blockCMD(playerid, "block_naj", 100);
    msgAll(C_FAJNY, "Gracz {c}%s(%d){/c} uwa¿a, ¿e {c}"NAME"{/c} jest najlepszym serwerem!", getNick(playerid), playerid);
    return giveMoney(playerid, 500), 1;
}

alias:kill("samobojstwo", "suicide")
CMD:kill(playerid)
{
    new reason[128];
    switch(random(5)) {
        case 0: format(reason, sizeof(reason), "pope³ni³(a) samobójstwo");
        case 1: format(reason, sizeof(reason), "wkurwi³(a) potê¿nego vacexa");
        case 2: format(reason, sizeof(reason), "zosta³(a) rozjebany(a) przez Lou");
        case 3: format(reason, sizeof(reason), "zosta³(a) wpisany(a) w kod serwera");
        case 4: format(reason, sizeof(reason), "spotka³(a) swojego najebanego starego");
    }
    msgAll(C_CORAL, "Gracz {c}%s(%d){/c} %s.", getNick(playerid), playerid, reason);
    return SetPlayerHealth(playerid, 0.0), 1;
}

alias:obiekty("objects")
CMD:obiekty(playerid)
{   
    Streamer_DestroyAllVisibleItems(playerid, 0);
    if(!GetPVarInt(playerid, "player_objects_visible")) {
        Streamer_ToggleIdleUpdate(playerid, 0);
        Streamer_ToggleItemUpdate(playerid, 0, 0);
        SetPVarInt(playerid, "player_objects_visible", 1);
    } else {
        Streamer_ToggleIdleUpdate(playerid, 1);
        Streamer_ToggleItemUpdate(playerid, 0, 1);
        SetPVarInt(playerid, "player_objects_visible", 0);
    }
    return msg(playerid, C_CHAT, "Obiekty zosta³y %s.", (GetPVarInt(playerid, "player_objects_visible") == 0) ? ("w³¹czone") : ("wy³¹czone")), 1;
}

stock CheckAdminList(rankid, count)
{
    new string[256];
    catstr(string, getRankColor(rankid), -1, "%s:\n", getRankName(rankid));

    foreach(new i : Permission) {
        if(playerData[i][player_admin] == rankid && !playerData[i][player_isFake]) {
            count ++;
            format(string, sizeof(string), "%s\t"WHITE"%s(%d)\n", string, getNick(i), i);
        }
    }
    catstr(string, -1, -1, "%s", (count > 0) ? ("\n") : (""GREY"Aktualnie brak!\n\n"));
    return string;
}

alias:admins("admini", "administracja", "administration")
CMD:admins(playerid)
{
    if(Iter_Count(Permission) == 0) return dialogBox(playerid, C_RED, ""RED"Aktualnie nie ma nikogo z administracji.");

    new count[5]; // seth ty kurwo

    s[0] = EOS;
    catstr(s, -1, -1, "%s", CheckAdminList(HEAD, count[0]));
    catstr(s, -1, -1, "%s", CheckAdminList(VICE, count[1]));
    catstr(s, -1, -1, "%s", CheckAdminList(ADMIN, count[2]));
    catstr(s, -1, -1, "%s", CheckAdminList(MODERATOR, count[3]));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Administracja", s, "OK", #), 1;
}

alias:pm("msg", "pw", "w")
CMD:pm(playerid, params[])
{
    new id, message[255];
    if(sscanf(params, "ds[255]", id, message)) return syntaxError(playerid, "/pm [id] [wiadomoœæ]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(id == playerid) return sendError(playerid, ""RED"Nie mo¿esz wysy³aæ prywatnej wiadomoœci do siebie.");
    if(isnull(message)) return sendError(playerid, ""RED"Nie mo¿esz wys³aæ pustej wiadomoœci.");
    if(containsIP(message)) return sendError(playerid, ""RED"Na tym serwerze nie mo¿na niczego reklamowaæ.");

    msg(playerid, C_BROWN, "[PM]: "BLUE2"WYS£ANE DO: "GREEN"%s(%d)", getNick(id), id);
    msg(playerid, C_BROWN, "[PM]: "LORANGE"%s", message);

    msg(id, C_BROWN, "[PM]: "BLUE2"OTRZYMANE DO: "GREEN"%s(%d)", getNick(playerid), playerid);
    msg(id, C_BROWN, "[PM]: "LORANGE"%s", message);

    foreach(new i : Permission) {
        if((playerData[playerid][player_admin] > playerData[i][player_admin]) || (playerData[id][player_admin] > playerData[i][player_admin])) continue;
        if(playerData[i][player_checkPM]) msg(i, C_DRED, "[pm]: "CORAL"%s(%d) -> %s(%d): "LY"%s", getNick(playerid), playerid, getNick(id), id, message);
    }
    Log(LOG_CHAT, "[pm]: %s(%d) -> %s(%d): %s", getNick(playerid), playerid, getNick(id), id, message);

    playerData[id][player_lastPM] = playerid;
    GameTextForPlayer(id, "~y~DOSZEDL PM!", 3000, 6);
    return playSound(id, 1057), 1;
}

CMD:re(playerid, params[])
{
    if(!IsPlayerConnected(playerData[playerid][player_lastPM])) return Polaczenie(playerid);
    return callcmd::pm(playerid, form("%d %s", playerData[playerid][player_lastPM], params)), 1;
}

alias:regulamin("rules")
CMD:regulamin(playerid) return ShowRegulamin(playerid);

alias:taryfikator("penalty", "prules")
CMD:taryfikator(playerid)
{
    s[0] = EOS;
    strcat(s, ""RED"Wszystkie cheaty i wspomagacze:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Ban 30 dni\n");
    strcat(s, ""YELLOW"2 Przewinienie - Ban Permanentny\n\n");

    strcat(s, ""RED"Wyzwiska i prowokacje:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Mute na 3 minuty\n");
    strcat(s, ""YELLOW"2 Przewinienie - Mute na 5 minut\n");
    strcat(s, ""YELLOW"Kolejne przewinienia - Warn/Ban 5 dni\n\n");

    strcat(s, ""RED"Fake-raport i omijanie mute:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Warn\n");
    strcat(s, ""YELLOW"2 Przewinienie - Ban 1 dzieñ\n\n");

    strcat(s, ""RED"Wykorzystywanie bugów serwera na korzyœæ gracza:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Warn\n");
    strcat(s, ""YELLOW"2 Przewinienie - Ban 15 dni\n");
    strcat(s, ""YELLOW"3 Przewinienie - Ban Permanentny\n\n");

    strcat(s, ""RED"Wulgarne i obraŸliwe nicki:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Kick\n");
    strcat(s, ""YELLOW"2 Przewinienie - Warn\n");
    strcat(s, ""YELLOW"Kolejne przewinienia - Ban (admin sam okreœla d³ugoœæ, byle nie perm)\n\n");

    strcat(s, ""RED"Flood i spam:\n");
    strcat(s, ""YELLOW"1 Przewinienie - Mute na 1 minutê\n");
    strcat(s, ""YELLOW"2 Przewinienie - Mute na 3 minuty\n");
    strcat(s, ""YELLOW"3 Przewinienie - Warn\n\n");

    strcat(s, ""RED"Inne sytuacje:\n");
    strcat(s, ""YELLOW"Omijanie bana - Ban Permanentny\n");
    strcat(s, ""YELLOW"MultiKonta (wiêcej ni¿ 2) - Ban Permanentny\n");
    strcat(s, ""YELLOW"Wy³udzanie czegokolwiek - Warn/Ban 3 dni\n\n");

    strcat(s, ""LRED"Je¿eli przewinienia bêd¹ dalej siê powtarzaæ, mo¿na nadawaæ bany.\n");
    strcat(s, ""LRED"D³ugoœæ czasu zale¿y od administratora - mo¿e nawet daæ perma w wyj¹tkowych sytuacjach.");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Taryfikator", s, "OK", #), 1;
}

alias:idzdo("goto", "tpa")
CMD:idzdo(playerid, params[])
{
    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/idzdo [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(!pAccountSettings[id][2]) return sendError(playerid, ""RED"Ten gracz ma wy³¹czone proœby o teleport.");
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Ten gracz wykonuje aktualnie jak¹œ czynnoœæ, przez któr¹ nie mo¿esz siê do niego teleportowaæ.");

    blockCMD(playerid, "block_idzdo", 10);
    msg(playerid, C_CHAT, "Wys³a³eœ(aœ) proœbê o teleportacjê do {c}%s(%d){/c}.", getNick(id), id);

    playerData[id][player_teleportRequest] = playerid;
    playerData[id][player_teleportTimer] = 45;

    if(!pAccountSettings[id][4]) {
        s[0] = EOS;
        catstr(s, C_BLUE, -1, "Gracz {c}%s(%d){/c} chce siê Ciebie teleportowaæ!\n"LY"Kliknij poni¿ej odpowiedni przycisk, by zaakceptowaæ proœbê!", getNick(playerid), playerid);
        
        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
            #pragma unused dd, ll, ii
            if(!rr) {
                playerData[pp][player_teleportRequest] = -1;
                playerData[pp][player_teleportTimer] = 0;

                msg(pp, C_BLUE, "Odrzuci³eœ(aœ) proœbê teleportacji od {c}%s(%d){/c}.", getNick(playerid), playerid);
                return msg(playerid, C_BLUE, "Gracz {c}%s(%d){/c} odrzuci³(a) Twoj¹ proœbê o teleportacjê.", getNick(pp), pp);
            }
            return callcmd::tpaccept(pp), 1; 
        }
        Dialog_ShowCallback(id, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""YELLOW"Proœba o teleport", s, "Akceptuj", "Odrzuæ");
    } 
    else msg(id, C_LGREEN, "Gracz {c}%s(%d){/c} chce siê do Ciebie przyteleportowaæ! "LY"(/tpaccept)", getNick(playerid), playerid);    
    return playSound(id, 1139), 1;
}

alias:tpaccept("tpacc", "akceptuj")
CMD:tpaccept(playerid)
{
    if(playerData[playerid][player_teleportRequest] == -1 || playerData[playerid][player_teleportTimer] < 1) return sendError(playerid, ""RED"Nie posiadasz oczekuj¹cej proœby o teleport.");

    new id = playerData[playerid][player_teleportRequest];
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Ten gracz wykonuje aktualnie jak¹œ czynnoœæ, przez któr¹ nie mo¿esz siê do niego teleportowaæ.");
   
    playerData[playerid][player_teleportRequest] = -1;
    playerData[playerid][player_teleportTimer] = 0;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    return Teleport(id, (GetPlayerState(id) == PLAYER_STATE_DRIVER) ? (true) : (false), x + math_random_float(1.0, 3.0), y + math_random_float(1.0, 3.0), z + 1.0, 0.0, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)), 1;
}

CMD:siema(playerid) {
    blockCMD(playerid, "block_siema", 10);
    return msgAll(C_BLUE, "Gracz {c}%s(%d) {/c}wita siê z ka¿dym!", getNick(playerid), playerid), 1;
}

CMD:nara(playerid) {
    blockCMD(playerid, "block_nara", 10);
    return msgAll(C_BLUE, "Gracz {c}%s(%d) {/c}¿egna siê z ka¿dym!", getNick(playerid), playerid), 1;
}

alias:time("czas")
CMD:time(playerid, params[])
{
    new hour, minute;
    if(sscanf(params, "dd", hour, minute)) return syntaxError(playerid, "/czas [godzina] [minuta]");
    if(hour > 24 || hour < 0) return sendError(playerid, ""RED"Nieprawid³owa godzina.");
    if(minute > 60 || minute < 0) return sendError(playerid, ""RED"Nieprawid³owa minuta.");
    return SetPlayerTime(playerid, hour, minute), 1;
}
CMD:dzien(playerid) return callcmd::time(playerid, "12 0");
CMD:noc(playerid) return callcmd::time(playerid, "0 0");

alias:weather("pogoda")
CMD:weather(playerid, params[])
{
    new weatherid; 
    if(sscanf(params, "d", weatherid)) return syntaxError(playerid, ""RED"/pogoda [id pogody]");
    return SetPlayerWeather(playerid, weatherid), 1;
}

CMD:jebac(playerid, params[])
{
    if(isnull(params)) return syntaxError(playerid, "/jebac [kogo]");
    blockCMD(playerid, "block_jebac", 30);
    return msgAll(C_ZAJEBISTY, "Gracz {c}%s(%d){/c} mówi, ¿e jebaæ {c}%s{/c}.", getNick(playerid), playerid, params), 1;
}

CMD:me(playerid, params[])
{
    new message[128];
    if(sscanf(params, "s[128]", message)) return syntaxError(playerid, "/me [treœæ]");
    if(strlen(message) < 3 || strlen(message) > 35) return sendError(playerid, ""RED"Dozwolona d³ugoœæ tekstu to: "WHITE"3-35 "RED"znaki.");

    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    foreach(new i : Player) if(IsPlayerInRangeOfPoint(i, 15.0, x, y, z)) msg(i, C_GREY, "[me]: "WHITE"%s: "GREY"** %s **", getNick(playerid), message);
    return 1;
}

CMD:do(playerid, params[])
{
    new message[128];
    if(sscanf(params, "s[128]", message)) return syntaxError(playerid, "/do [treœæ]");
    if(strlen(message) < 3 || strlen(message) > 35) return sendError(playerid, ""RED"Dozwolona d³ugoœæ tekstu to: "WHITE"3-35 "RED"znaki.");

    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    foreach(new i : Player) if(IsPlayerInRangeOfPoint(i, 15.0, x, y, z)) msg(i, C_LORANGE, "[do]: "WHITE"%s "LORANGE"%s", getNick(playerid), message);
    return 1;
}

alias:flo("rsp")
CMD:flo(playerid)
{
    if(IsPlayerInAnyVehicle(playerid)) return sendError(playerid, ""RED"Nie mo¿esz przebywaæ w pojeŸdzie, je¿eli chcesz przeprowadziæ {c}resynchronizacjê{/c} swojej postaci!");

    playerData[playerid][player_respawn] = true;
    SavePlayerPosition(playerid);
    ResetPlayerWeapons(playerid);

    SpawnPlayer(playerid);
    for(new x = 0; x < 7; x++) giveWeapon(playerid, pWeapons[playerid][x], 1000);
    return msg(playerid, C_LIME, "Resynchronizacja postaci udana!"), 1;
}

alias:bronie("guns", "weapons")
CMD:bronie(playerid)
{
    s[0] = EOS;
    s = ""CORAL"Bronie\t"WHITE"Koszt\n";
    for(new i = 0; i < sizeof(weaponShopData); i++) { if(!weaponShopData[i][weapon_bonus]) format(s, sizeof(s), "%s"CORAL"%s\t%s%s$\n", s, weaponShopData[i][weapon_name], (getMoney(playerid) > weaponShopData[i][weapon_money]) ? (GREEN) : (RED), strdot(weaponShopData[i][weapon_money])); }

    inline onWeaponBuy(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii
        if(!rr) return 1;
        if(getMoney(playerid) < weaponShopData[ll][weapon_money]) return valueError(playerid, weaponShopData[ll][weapon_money]);
        
        giveMoney(playerid, -weaponShopData[ll][weapon_money]);
        giveWeapon(playerid, weaponShopData[ll][weapon_id], 1000);
        return msg(playerid, C_CORAL, "Kupi³eœ(aœ) broñ {c}%s{/c} za: {c}%s${/c}.", WeaponNames[weaponShopData[ll][weapon_id]], strdot(weaponShopData[ll][weapon_money])), 1;
    }
    return Dialog_ShowCallback(playerid, using inline onWeaponBuy, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Bronie", s, "Zakup", "Cofnij"), 1;
}

alias:style("stylewalki", "fightstyle")
CMD:style(playerid)
{
    if(!pUlepszenia[playerid][ULEPSZENIE_STYLE]) return ulepszenieError(playerid, ULEPSZENIE_STYLE);

    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii
        if(!rr) return 1;

        switch(ll) {
            case 0: SetPlayerFightingStyle(playerid, FIGHT_STYLE_NORMAL);
            case 1: SetPlayerFightingStyle(playerid, FIGHT_STYLE_BOXING);
            case 2: SetPlayerFightingStyle(playerid, FIGHT_STYLE_KUNGFU);
            case 3: SetPlayerFightingStyle(playerid, FIGHT_STYLE_KNEEHEAD);
        }

        playerData[playerid][player_fightstyle] = ll;
        m_pquery_format("UPDATE es_accounts SET fightstyle=%d WHERE id=%d LIMIT 1;", ll, playerData[playerid][player_accountID]);
        return msg(playerid, C_CORAL, "Styl walki zmieniony."), 1;
    }
    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""YELLOW"Style walki", ""WHITE"Normalny\n"WHITE"Boks\n"WHITE"Karate\n"WHITE"Uliczny", "Ustaw", "Zamknij"), 1;
}