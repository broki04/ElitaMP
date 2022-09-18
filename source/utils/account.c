stock FindAccount(playerid)
{   
    new Cache:result = mysql_query(dbconn, form("SELECT * FROM es_accounts WHERE nick='%s'", getNick(playerid)));

    for(new i = 0; i < 4; i++) TextDrawShowForPlayer(playerid, tdWelcome[i]);
    PlayerTextDrawShow(playerid, tdLoginTime);

    if(cache_num_rows() > 0) {
        playerData[playerid][player_loginTime] = 60;

        cache_get_value_name_int(0, "id", playerData[playerid][player_accountID]);
        cache_get_value_name(0, "date_last", playerData[playerid][player_datelast], 65);
        cache_get_value_name(0, "ip_last", playerData[playerid][player_iplast], 18);

        cache_get_value_name(0, "secret_question", pSecretQuestion[playerid]);
        cache_get_value_name(0, "secret_answer", pSecretAnswer[playerid]);
        cache_get_value_name(0, "secret_pincode", pSecretPinCode[playerid]);

        CheckPlayerBanned(playerid, true);
        LoadPlayerStats(playerid);
        ShowLoginPanel(playerid, "");
    } else {
        CheckPlayerBanned(playerid, false);
        ShowRegisterPanel(playerid, ""); 
    }

    for(new i = 0; i < 50; i++) msg(playerid, -1, " ");
    msg(playerid, -1, " "NAME" "GREY""VERSION" "WHITE"- update: "LIME""DATE" "WHITE"by: "LIME""AUTHOR"");
    msg(playerid, -1, " ");
    msg(playerid, C_NICK, "{c}(i)*{/c}  Dodaj nasz serwer do listy {c}ulubionych{/c}, poleæ tak¿e swoim znajomym.");
    msg(playerid, C_GREEN, "{c}(i)*{/c}  Posiadaj¹c tag {c}[ES]{/c} otrzymujesz wiêksze wynagrodzenie.");
    msg(playerid, C_BROWN, "{c}(i)*{/c}  Zapraszamy na serwer Discord: {c}"#DISCORD"{/c}.");
    msg(playerid, -1, " ");
    return cache_delete(result), 1;
}

stock ShowLoginPanel(playerid, const string[])
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Witaj, "LY"%s(%d) "WHITE"na serwerze!\n"WHITE"Twoje konto zosta³o odnalezione w naszej bazie danych!\n"WHITE"Wpisz poni¿ej "GREEN"odpowiednie has³o"WHITE", aby kontynuowaæ.", getNick(playerid), playerid);
    if(strlen(string) > 1) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, ""BLUE"Logowanie", s, "Zaloguj", "Anuluj");
}

stock ShowQuestionPanel(playerid, const string[])
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Twoje konto jest zabezpieczone "BLUE"sekretnym "WHITE"pytaniem.\n"WHITE"Twoje sekretne pytanie brzmi: "LY"%s\n\n"WHITE"Odpowiedz na nie poni¿ej:", pSecretQuestion[playerid]);
    if(strlen(string) > 1) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_QUESTION_LOGIN, DIALOG_STYLE_PASSWORD, ""BLUE"Odpowiedz na pytanie", s, "Zaloguj", "Anuluj");
}

stock ShowPinCodePanel(playerid, const string[])
{
    s[0] = EOS;
    strcat(s, ""WHITE"Twoje konto jest zabezpieczone "MINT"kodem PIN"WHITE".\n"WHITE"Wpisz wybrany przez ciebie "VIOLET2"kod "WHITE"poni¿ej:");
    if(strlen(string) > 1) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_PINCODE_LOGIN, DIALOG_STYLE_PASSWORD, ""BLUE"Odpowiedz na pytanie", s, "Zaloguj", "Anuluj");
}

stock ShowRegisterPanel(playerid, const string[])
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Witaj, "BLUE"%s(%d)"WHITE" na serwerze.\n"WHITE"Twoje konto nie zosta³o odnalezione na naszym serwerze.\n"WHITE"Wpisz poni¿ej odpowiednie has³o, by kontynuowaæ.\n\n"LRED"Dozwolona d³ugoœæ has³a to: "WHITE"5-30 "WHITE"znaków.", getNick(playerid), playerid);
    if(strlen(string) > 1) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, ""BLUE"Rejestracja", s, "Zarejestruj", "Anuluj");
}

stock OnDialogAccount(playerid, dialogid, response, inputtext[])
{
    switch(dialogid) {
        case DIALOG_REGISTER: {
            if(!response) return KickPlayer(playerid, -1, "Anulowanie rejestracji");

            new dest[90];
            mysql_escape_string(inputtext, dest, sizeof(dest));
            SetPVarString(playerid, "player_entered_password", dest);

            if(!isValidPassword(dest)) return ShowRegisterPanel(playerid, ""RED"Wprowadzono nieprawid³owe znaki, b¹dŸ przekroczono dozwolony limit "WHITE"5-25 "RED"znaków.");
            return bcrypt_hash(playerid, "OnPasswordHash", dest, BCRYPT_COST), 1;
        }

        case DIALOG_LOGIN: {
            if(!response) return KickPlayer(playerid, -1, "Anulowanie logowania");
            if(!isValidPassword(inputtext)) return ShowLoginPanel(playerid, "");
            return bcrypt_verify(playerid, "OnPasswordChecked", inputtext, playerData[playerid][player_password]), 1;
        }

        case DIALOG_QUESTION_LOGIN: {
            if(!response) return KickPlayer(playerid, -1, "Anulowanie logowanie"), 1;

            if(!strmatch(pSecretAnswer[playerid], inputtext, true)) {
                playerData[playerid][player_attempts] ++;
                if(playerData[playerid][player_attempts] >= 3) return KickPlayer(playerid, -1, "B³êdne has³o"), 1;
                else ShowQuestionPanel(playerid, form(""RED"Wprowadzono nieprawid³ow¹ odpowiedŸ na pytanie. "WHITE"(%d/3 prób)", playerData[playerid][player_attempts]));
            } else {
                playerData[playerid][player_attempts] = 0;
                if(!strmatch(pSecretPinCode[playerid], "@NULL", true)) ShowPinCodePanel(playerid, "");
                else OnPlayerLogged(playerid);
            }
            return 1;
        }

        case DIALOG_PINCODE_LOGIN: {
            if(!response) return KickPlayer(playerid, -1, "Anulowano logowanie"), 1;

            if(!strmatch(pSecretPinCode[playerid], inputtext, true)) {
                playerData[playerid][player_attempts] ++;
                if(playerData[playerid][player_attempts] >= 3) return KickPlayer(playerid, -1, "B³êdne has³o"), 1;
                else ShowPinCodePanel(playerid, form(""RED"Wprowadzono nieprawid³owy kod PIN. "WHITE"(%d/3 prób)", playerData[playerid][player_attempts]));
            }
            else OnPlayerLogged(playerid);
            return 1;
        }
    }
    return 0;
}

function OnPasswordHash(playerid)
{
    new hashed[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hashed);

    new rand = random(sizeof(KoloryGraczy));
    for(new i = 0; i < sizeof(hudOptions); i++) pHud[playerid][i] = KoloryGraczy[rand];

    dbstr[0] = EOS;
    format(dbstr, sizeof(dbstr), "INSERT INTO es_accounts (nick, password, date_reg, date_last, ip_reg, ip_last, serial_reg, serial_last) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');", getNick(playerid), hashed, getLocalDate(), getLocalDate(), getIP(playerid), getIP(playerid), getSerial(playerid), getSerial(playerid));
    m_pquery(dbstr, "OnPlayerRegistered", "d", playerid);
    return 1;
}

function OnPasswordChecked(playerid, bool:success)
{
    if(success) {
        playerData[playerid][player_attempts] = 0;
        if(!strmatch(pSecretQuestion[playerid], "@NULL", true)) ShowQuestionPanel(playerid, "");
        else if(!strmatch(pSecretPinCode[playerid], "@NULL", true)) ShowPinCodePanel(playerid, "");
        else OnPlayerLogged(playerid);
    }
    else {
        if(playerData[playerid][player_attempts] ++ >= 3) {
            s[0] = EOS;
            strcat(s, ""WHITE"Wykorzystano wszystkie próby na wpisanie has³a.\n"WHITE"Wpisywane przez ciebie has³o nie by³o poprawne, wiêc zosta³eœ(aœ) wyrzucony(a).\n\n");
            strcat(s, ""WHITE"Nie pamiêtasz has³a? "LY"Skontaktuj siê z nami na Discord.\n"WHITE"Discord: "LBLUE""#DISCORD"");
            showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Nieprawid³owe has³o", s, "OK", #);

            Kick(playerid);
        } 
        else ShowLoginPanel(playerid, form(""RED"Wprowadzono nieprawid³owe has³o. Liczba prób: "WHITE"%d/3", playerData[playerid][player_attempts]));
    }
    return 1;
}

stock OnPlayerLogged(playerid)
{
    playerData[playerid][player_logged] = true;
    playerData[playerid][player_attempts] = playerData[playerid][player_loginTime] = 0;

    if(LoadPlayerStats(playerid)) {
        playerData[playerid][player_visits] ++;

        msg(playerid, C_GREEN, "Zalogowano pomyœlnie - {c}%d{/c} raz.", playerData[playerid][player_visits]);
        msg(playerid, C_GREEN, "Ostatnia wizyta: {c}%s{/c} "LGREEN"(IP: %s)", playerData[playerid][player_datelast], playerData[playerid][player_iplast]);

        checkRecord(RECORD_PLAYERS, Iter_Count(Player));
        checkRecord(RECORD_TODAY, Iter_Count(Player));

        if(playerData[playerid][player_mutetime] > 0) msg(playerid, C_MINT, "Jesteœ wyciszony(a)! Czas wyciszenia: {c}%s{/c}.", ConvertTime(playerData[playerid][player_mutetime], TIME_TYPE_MS));
        if(playerData[playerid][player_adminLoad] > 0) msg(playerid, getRankColor(playerData[playerid][player_adminLoad]), "Posiadasz rangê administracyjn¹ {c}%s poz. %d{/c}. Aby siê zalogowaæ, u¿yj: {c}/alogin{/c}.", getRankName(playerData[playerid][player_adminLoad]), playerData[playerid][player_adminLevel]);

        if(playerData[playerid][player_premiumTime] > gettime()) {
            if(!Iter_Contains(Premium, playerid)) Iter_Add(Premium, playerid);

            callcmd::vpozostalo(playerid);
            if(!strmatch(pOwnRank[playerid], "@NULL", true)) { 
                pOwnRankHave[playerid] = true; 
                msg(playerid, pOwnRankColor[playerid], "Twoja w³asna ranga: {c}%s{/c} za³adowana - zarz¹dzanie pod: {c}/ownrank{/c}.", pOwnRank[playerid]); 
            }
        }
        if(pCredit[playerid] > 0) msg(playerid, C_FAJNY, "Posiadasz zaci¹gniêty kredyt na {c}%s${/c} - procent: {c}0.2{/c}.", strdot(pCredit[playerid]));

        if(pDailyQuest[playerid] == -1 && !pDailyDone[playerid]) SetRandomQuest(playerid);       
        if(pDailyQuest[playerid] != -1 && !pDailyDone[playerid]) {
            PlayerTextDrawSetString(playerid, tdQuest, form("ZADANIE DNIA:~n~~w~~h~%s", GetQuestName(playerid)));
            msg(playerid, C_BLUE2, "Posiadasz aktywne zadanie dnia: {c}%s{/c}. Postêp: {c}%d/%d{/c}.", GetQuestName(playerid), pDailyStatus[playerid], questListData[pDailyQuest[playerid]][daily_required]);
        }

        if(pGang[playerid] != -1) LoadPlayerGang(playerid);
        LoadPlayerPrivCars(playerid);

        setMoney(playerid, playerData[playerid][player_money]);
        setLevel(playerid, playerData[playerid][player_level]);
        PlayerTextDrawHide(playerid, tdLoginTime);

        switch(playerData[playerid][player_fightstyle]) {
            case 0: SetPlayerFightingStyle(playerid, FIGHT_STYLE_NORMAL);
            case 1: SetPlayerFightingStyle(playerid, FIGHT_STYLE_BOXING);
            case 2: SetPlayerFightingStyle(playerid, FIGHT_STYLE_KUNGFU);
            case 3: SetPlayerFightingStyle(playerid, FIGHT_STYLE_KNEEHEAD);
        }

        if(playerData[playerid][player_visits] < 10) ShowTutorial(playerid);
        else callcmd::nowosci(playerid);
    }

    msgAll(C_YELLOW, ""GREEN"[JOIN]: "YELLOW"Gracz {c}%s(%d) "YELLOW"do³¹czy³(a) do serwera!", getNick(playerid), playerid);
    msgAdmins(C_LORANGE, ""RED"[JOIN]: "LORANGE"IP: {c}%s{/c} "GREY"(%s - %s, %s)", getIP(playerid), playerData[playerid][player_country], playerData[playerid][player_city], playerData[playerid][player_isp]);
    if(IsPlayerUsingProxy(playerid)) msgAdmins(C_ORANGE, ""RED"[PROXY]: "LORANGE"Gracz {c}%s(%d){/c} prawdopodobnie u¿ywa {c}Proxy{/c}!", getNick(playerid), playerid);
    return Log(LOG_DEBUG, "[login]: %s(%d) -> %s - %s, %s (IP: %s)", getNick(playerid), playerid, playerData[playerid][player_country], playerData[playerid][player_city], playerData[playerid][player_isp], getIP(playerid)), 1;
}

function OnPlayerRegistered(playerid)
{
    playerData[playerid][player_logged] = true;
    playerData[playerid][player_accountID] = cache_insert_id();

    if(LoadPlayerStats(playerid)) {
        msgAll(C_BROWN, "Pojawi³ siê nowy gracz - {c}%s(%d){/c}. To jest nasz {c}%s{/c} gracz!", getNick(playerid), playerid, strdot(playerData[playerid][player_accountID]));
        Log(LOG_DEBUG, "[register]: %s(%d) -> uid: %s", getNick(playerid), playerid, strdot(playerData[playerid][player_accountID]));

        givePremium(playerid, 3);
        callcmd::vpozostalo(playerid);

        new password[130]; GetPVarString(playerid, "player_entered_password", password, sizeof(password));

        s[0] = EOS;
        format(s, sizeof(s), ""WHITE"Twoje konto zosta³o zarejestrowane pomyœlnie.\n"WHITE"Zapamiêtaj swoje has³o do nastêpnego logowania!\n\n");
        format(s, sizeof(s), "%s"WHITE"Unikalny identyfikator konta: "LGREEN"%s\n"WHITE"Has³o do konta: "RED"%s"WHITE".", s, strdot(playerData[playerid][player_accountID]), password);
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Rejestracja", s, "OK", #);
    }
    return 1;
}