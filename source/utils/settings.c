stock ResetAccountData(playerid)
{
    for(new i = 0; i < sizeof(AccountDataSettings); i++) pAccountSettings[playerid][i] = true;
}

alias:konto("settings", "ustawienia", "account")
CMD:konto(playerid)
{
    s[0] = EOS;
    format(s, sizeof(s), ""LBLUE"U�ytkownik: "WHITE"%s [userid: %d]\n", getNick(playerid), playerData[playerid][player_accountID]);
    format(s, sizeof(s), "%s"LBLUE"Pozycja w rankingu: "WHITE"#%s\n", s, strdot(0));
    strcat(s, " \n");
    strcat(s, ""ORANGE"�  "WHITE"Zmie� sw�j nick "LY"(5.000$)\n");
    strcat(s, ""ORANGE"�  "WHITE"Zmie� swoje has�o\n");
    strcat(s, ""ORANGE"�  "WHITE"Dodatkowe zabezpieczenia konta\n");
    format(s, sizeof(s), "%s"ORANGE"�  "WHITE"Pozycja spawnowania si�: "BLUE"%s\n", s, getSpawnName(playerData[playerid][player_spawnData]));
    format(s, sizeof(s), "%s"ORANGE"�  "WHITE"Tryb czatowania: "RED"%s\n", s, getChatType(playerData[playerid][player_chatID]));
    strcat(s, " \n");
    for(new i = 0; i < sizeof(AccountDataSettings); i++) format(s, sizeof(s), "%s%s "WHITE"%s\n", s, (pAccountSettings[playerid][i]) ? (""GREEN"(ON)") : (""RED"(OFF)"), AccountDataSettings[i]);
    return showDialog(playerid, DIALOG_SETTINGS, DIALOG_STYLE_LIST, form(""BLUE"U�ytkownik: "WHITE"%s [userid: %d]", getNick(playerid), playerData[playerid][player_accountID]), s, "Wybierz", "Zamknij"), 1;
}

function OnPlayerChangePassword(playerid)
{
    new hashed[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hashed);

    inline onPasswordChange()
    {
        new password[130]; GetPVarString(playerid, "player_entered_password", password, sizeof(password));

        s[0] = EOS;
        format(s, sizeof(s), ""WHITE"Pomy�lnie "LY"zmieni�e�(a�) "WHITE"swoje has�o.\n"WHITE"Zapami�taj je, teraz b�dziesz si� nim "BLUE"logowa�!\n"WHITE"Nowe has�o to: "ORANGE"%s"WHITE".", password);
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""VIOLET"Zmiana has�a", s, "OK", #);
    }
    m_tquery_inline(dbconn, using inline onPasswordChange, "UPDATE es_accounts SET password='%s' WHERE id=%d LIMIT 1;", hashed, playerData[playerid][player_accountID]);
    return 1;
}

stock getAccountSettings(playerid)
{
    new astr[256];
    for(new i = 0; i < sizeof(AccountDataSettings); i++) format(astr, sizeof(astr), "%s%s%d", astr, i == 0 ? ("") : (","), pAccountSettings[playerid][i]);
    return astr;
}

stock getSpawnName(typeid)
{
    new name[25];
    switch(typeid) {
        case 0: name = "Losowa";
        case 1: name = "Sta�a";
        case 2: name = "Ostatnia";
    }
    return name;
}

stock OnDialogSettings(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_SETTINGS: {
            if(!response) return 1;

            switch(listitem) {
                case 0..2, 8: callcmd::konto(playerid);
                
                case 3: {
                    if(playerData[playerid][player_nickTimer] > gettime()) return sendError(playerid, ""RED"Ostatnio dokonywa�e�(a�) zmiany nicku!");
                    if(getMoney(playerid) < 5000) return valueError(playerid, 5000);
                    showDialog(playerid, DIALOG_NEW_NICK, DIALOG_STYLE_INPUT, ""BLUE"Zmiana nicku", ""WHITE"Wpisz poni�ej sw�j nowy nick:", "Ustaw", "Cofnij");
                }
                case 4: showDialog(playerid, DIALOG_NEW_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Zmiana has�a", ""WHITE"Wpisz poni�ej swoje nowe has�o:", "Ustaw", "Cofnij");
            
                case 5: {
                    s[0] = EOS;
                    s = ""WHITE"Polecenie\t"GREEN"Sta"RED"tus\n";
                    
                    if(!strmatch(pSecretQuestion[playerid], "@NULL", true)) format(s, sizeof(s), "%s"WHITE"Sekretne pytanie\t"GREEN"+\n", s);
                    else format(s, sizeof(s), "%s"WHITE"Sekretne pytanie\t"RED"-\n", s);

                    if(!strmatch(pSecretPinCode[playerid], "@NULL", true)) format(s, sizeof(s), "%s"WHITE"Kod PIN\t"GREEN"+\n", s);
                    else format(s, sizeof(s), "%s"WHITE"Kod PIN\t"RED"-\n", s);
                
                    showDialog(playerid, DIALOG_ACCOUNT_SECURITY, DIALOG_STYLE_TABLIST_HEADERS, ""VIOLET2"Dodatkowe zabezpieczenia konta", s, "Wybierz", "Cofnij");
                }

                case 6: {
                    s[0] = EOS;
                    s = ""WHITE"Pozycja spawnowania\t"GREEN"Sta"RED"tus\n";
                    format(s, sizeof(s), "%s"WHITE"Losowa pozycja\t%s\n", s, (playerData[playerid][player_spawnData] == 0) ? (""GREEN"TAK") : (""RED"NIE"));
                    format(s, sizeof(s), "%s"WHITE"Sta�a pozycja\t%s\n", s, (playerData[playerid][player_spawnData] == 1) ? (""GREEN"TAK") : (""RED"NIE"));
                    format(s, sizeof(s), "%s"WHITE"Ostatnia pozycja\t%s\n", s, (playerData[playerid][player_spawnData] == 2) ? (""GREEN"TAK") : (""RED"NIE"));
                    showDialog(playerid, DIALOG_ACCOUNT_POSITION, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Wybierz pozycje spawnowania si�", s, "Wybierz", "Cofnij");
                }

                case 7: {
                    s[0] = EOS;
                    s = ""WHITE"Tryb czatowania\t"GREEN"Sta"RED"tus\n";
                    format(s, sizeof(s), "%s"WHITE"Globalny\t%s\n", s, (playerData[playerid][player_chatID] == 0) ? (""GREEN"TAK") : (""RED"NIE"));
                    format(s, sizeof(s), "%s"WHITE"Lokalny\t%s\n", s, (playerData[playerid][player_chatID] == 1) ? (""GREEN"TAK") : (""RED"NIE"));
                    format(s, sizeof(s), "%s"WHITE"Zespo�owy\t%s\n", s, (playerData[playerid][player_chatID] == 2) ? (""GREEN"TAK") : (""RED"NIE"));
                    showDialog(playerid, DIALOG_ACCOUNT_CHAT, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Wybierz pozycje spawnowania si�", s, "Wybierz", "Cofnij");
                }

                default: {
                    pAccountSettings[playerid][listitem - 9] = !pAccountSettings[playerid][listitem - 9];
                    m_pquery_format("UPDATE es_accounts SET settings='%s' WHERE id=%d LIMIT 1;", getAccountSettings(playerid), playerData[playerid][player_accountID]);
                    callcmd::konto(playerid);
                }
            }
            return 1;
        }

        case DIALOG_ACCOUNT_CHAT: {
            if(!response) return callcmd::konto(playerid);
            if(listitem == 2 && pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jeste� w �adnym zespole.");

            playerData[playerid][player_chatID] = listitem;
            m_pquery_format("UPDATE es_accounts SET chat=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_chatID], playerData[playerid][player_accountID]);
            return callcmd::konto(playerid), 1;
        }

        case DIALOG_ACCOUNT_POSITION: {
            if(!response) return callcmd::konto(playerid);

            if(pStalyPos[playerid][0] == 0.0 && listitem == 1) return sendError(playerid, ""RED"Twoja sta�a pozycja nie zosta�a ustalona. Ustalisz j� komend�: "WHITE"/stalypos.");
            if(listitem == 2) {
                new Float:x, Float:y, Float:z, Float:a;
                getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);

                playerData[playerid][player_lastPos][0] = x;
                playerData[playerid][player_lastPos][1] = y;
                playerData[playerid][player_lastPos][2] = z;
                playerData[playerid][player_lastPos][3] = a;
                playerData[playerid][player_lastInterior] = GetPlayerInterior(playerid);
                m_pquery_format("UPDATE es_accounts SET last_pos='%f,%f,%f,%f', last_int=%d WHERE id=%d LIMIT 1;", unpackXYZR(playerData[playerid][player_lastPos]), playerData[playerid][player_lastInterior], playerData[playerid][player_accountID]);
            }
            
            playerData[playerid][player_spawnData] = listitem;
            m_pquery_format("UPDATE es_accounts SET spawn_data=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_spawnData], playerData[playerid][player_accountID]);
            return callcmd::konto(playerid), 1;
        }

        case DIALOG_ACCOUNT_SECURITY: {
            if(!response) return callcmd::konto(playerid);

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_ACCOUNT_QUESTION, DIALOG_STYLE_INPUT, ""BLUE"Zabezpieczenie konta - sekretne pytanie "WHITE"(1/2)", ""WHITE"Wpisz poni�ej sekretne pytanie:", "Dalej", "Cofnij");
                case 1: showDialog(playerid, DIALOG_ACCOUNT_PINCODE, DIALOG_STYLE_INPUT, ""GOLD"Zabezpieczenie konta - kod pin", ""WHITE"Wpisz poni�ej cyfrowy kod PIN:", "Ustaw", "Cofnij");
            }
            return 1;
        }

        case DIALOG_ACCOUNT_PINCODE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_SETTINGS, 1, 5, "");

            if(!isNumeric(inputtext)) return showDialog(playerid, DIALOG_ACCOUNT_PINCODE, DIALOG_STYLE_INPUT, ""GOLD"Zabezpieczenie konta - kod pin", ""WHITE"Wpisz poni�ej cyfrowy kod PIN:\n\n"RED"Kod PIN musi si� sk�ada� tylko z liczb!", "Ustaw", "Cofnij");
            if(strlen(inputtext) < 3 || strlen(inputtext) > 10) return showDialog(playerid, DIALOG_ACCOUNT_PINCODE, DIALOG_STYLE_INPUT, ""GOLD"Zabezpieczenie konta - kod pin", ""WHITE"Wpisz poni�ej cyfrowy kod PIN:\n\n"RED"Dozwolona d�ugo�� kodu PIN to: "WHITE"3-10 "RED"liczb.", "Ustaw", "Cofnij");

            format(pSecretPinCode[playerid], 20, "%s", inputtext);
            m_pquery_format("UPDATE es_accounts SET secret_pincode='%s' WHERE id=%d LIMIT 1;", pSecretPinCode[playerid], playerData[playerid][player_accountID]);
            
            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Aktywowa�e�(a�) sw�j "PINK"kod PIN"WHITE".\n"WHITE"B�dzie si� on pojawia� po wpisaniu odpowiedniego has�a.\n\n"WHITE"Wybrany kod PIN: "BLUE"%s"WHITE".", pSecretPinCode[playerid]);
            return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Kod PIN", s, "OK", #), 1;
        }

        case DIALOG_ACCOUNT_QUESTION: {
            if(!response) return OnDialogResponse(playerid, DIALOG_SETTINGS, 1, 5, "");
            if(strlen(inputtext) < 3 || strlen(inputtext) > 20) return showDialog(playerid, DIALOG_ACCOUNT_QUESTION, DIALOG_STYLE_INPUT, ""BLUE"Zabezpieczenie konta - sekretne pytanie "WHITE"(1/2)", ""WHITE"Wpisz poni�ej sekretne pytanie:\n\n"RED"Dozwolona d�ugo�� pytania to: "WHITE"3-20 "RED"znak�w.", "Dalej", "Cofnij");
            
            SetPVarString(playerid, "player_secret_question", inputtext);
            return showDialog(playerid, DIALOG_ACCOUNT_ANSWER, DIALOG_STYLE_INPUT, ""BLUE"Zabezpieczenie konta - sekretne pytanie "WHITE"(2/2)", ""WHITE"Wpisz poni�ej sekretn� odpowied�:", "Ustaw", "Cofnij"), 1;
        }

        case DIALOG_ACCOUNT_ANSWER: {
            if(!response) return OnDialogResponse(playerid, DIALOG_ACCOUNT_SECURITY, 1, 0, "");
            if(strlen(inputtext) < 2 || strlen(inputtext) > 20) return showDialog(playerid, DIALOG_ACCOUNT_ANSWER, DIALOG_STYLE_INPUT, ""BLUE"Zabezpieczenie konta - sekretne pytanie "WHITE"(2/2)", ""WHITE"Wpisz poni�ej sekretn� odpowied�:\n\n"RED"Dozwolona d�ugo�� odpowiedzi to: "WHITE"2-20 "RED"znak�w.", "Ustaw", "Cofnij");
            
            new question[128]; GetPVarString(playerid, "player_secret_question", question, sizeof(question));
            format(pSecretQuestion[playerid], 90, "%s", question);  format(pSecretAnswer[playerid], 90, "%s", inputtext);
            m_pquery_format("UPDATE es_accounts SET secret_question='%s', secret_answer='%s' WHERE id=%d LIMIT 1;", pSecretQuestion[playerid], pSecretAnswer[playerid], playerData[playerid][player_accountID]);
            
            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Aktywowa�e�(a�) swoje sekretne pytanie!\n"WHITE"B�dzie si� ono pojawia�o po wpisaniu has�a.\n\n");
            format(s, sizeof(s), "%s"WHITE"Sekretne pytanie: "BLUE"%s\n"WHITE"Sekretna odpowied�: "BLUE"%s", s, pSecretQuestion[playerid], pSecretAnswer[playerid]);
            return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Sekretne pytanie", s, "OK", #), 1;
        }

        case DIALOG_NEW_PASSWORD: {
            if(!response) return callcmd::konto(playerid);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            SetPVarString(playerid, "player_entered_password", dest);

            if(!isValidPassword(dest)) return showDialog(playerid, DIALOG_NEW_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Zmiana has�a", ""WHITE"Wpisz poni�ej swoje nowe has�o:\n\n"RED"Wprowadzono nieprawid�owe znaki, b�d� przekroczono dozwolony limit "WHITE"5-25 "RED"znak�w.", "Ustaw", "Cofnij");
            return bcrypt_hash(playerid, "OnPlayerChangePassword", dest, BCRYPT_COST), 1;
        }

        case DIALOG_NEW_NICK: {
            if(!response) return callcmd::konto(playerid);
            if(isnull(inputtext) || strlen(inputtext) < 3 || strlen(inputtext) > 20 || !IsValidNickName(inputtext)) return showDialog(playerid, DIALOG_NEW_NICK, DIALOG_STYLE_INPUT, ""BLUE"Zmiana nicku", ""WHITE"Wpisz poni�ej sw�j nowy nick:\n\n"RED"Tw�j nick jest nieprawid�owy. Limit znak�w: "WHITE"3-20"RED".", "Ustaw", "Cofnij");
            
            inline onChangeNick()
            {
                if(cache_num_rows() > 0) return showDialog(playerid, DIALOG_NEW_NICK, DIALOG_STYLE_INPUT, ""BLUE"Zmiana nicku", ""WHITE"Wpisz poni�ej sw�j nowy nick:\n\n"RED"Ten nick jest ju� zaj�ty.", "Ustaw", "Cofnij");

                giveMoney(playerid, -5000);
                format(playerData[playerid][player_nickname], 25, "%s", inputtext);
                SetPlayerName(playerid, playerData[playerid][player_nickname]);

                if(pVehCount[playerid] > 0) {
                    for(new i = 0; i < pVehCount[playerid]; i++) {
                        new privcarid = pVehHave[playerid][i];
                        if(privcarid != -1 && pVehSpawned[privcarid]) format(pVehOwnerName[privcarid], 25, "%s", inputtext);
                    }
                }
                foreach(new i : Business) { if(businessData[i][business_ownerid] == playerData[playerid][player_accountID]) format(businessData[i][business_ownername], 25, "%s", inputtext); }

                playerData[playerid][player_nickTimer] = (gettime() + UnixTime('d', 3));
                m_pquery_format("UPDATE es_accounts SET nick_timer=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_nickTimer], playerData[playerid][player_accountID]);

                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Pomy�lnie zmieni�e�(a�) sw�j nick!\n"WHITE"Sw�j nick b�dziesz m�g� zmieni� po up�ywie "RED"3 dni"WHITE".\n\n");
                format(s, sizeof(s), "%s"WHITE"Tw�j nowy nick to: "BLUE"%s\n"WHITE"Koszt zmiany nicku: "YELLOW"5.000$"WHITE".", s, playerData[playerid][player_nickname]);
                showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Zmiana nicku", s, "OK", #);
            }

            m_tquery_inline(dbconn, using inline onChangeNick, "UPDATE es_accounts SET nick='%s' WHERE id=%d LIMIT 1;", inputtext, playerData[playerid][player_accountID]);
            return 1;
        }
    }
    return 0;
}

CMD:stalypos(playerid)
{
    if(NieMozeTeraz(playerid)) return sendError(playerid, ""RED"Nie mo�esz teraz u�y� tej komendy.");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), x, y, z, a);

    pStalyPos[playerid][0] = x;
    pStalyPos[playerid][1] = y;
    pStalyPos[playerid][2] = z;
    pStalyPos[playerid][3] = a;
    pStalyInterior[playerid] = GetPlayerInterior(playerid);
    playerData[playerid][player_spawnData] = 1;

    m_pquery_format("UPDATE es_accounts SET staly_pos='%f, %f, %f, %f', staly_int=%d, spawn_data=%d WHERE id=%d LIMIT 1;", x, y, z, a, pStalyInterior[playerid], playerData[playerid][player_spawnData], playerData[playerid][player_accountID]);
    return msg(playerid, C_YELLOW, "Ustawi�e�(a�) swoj� {c}sta��{/c} lokalizacj� spawnu."), 1;
}