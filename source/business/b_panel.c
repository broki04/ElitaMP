#include YSI_Coding\y_hooks

stock ShowBusinessPanel(playerid, businessid)
{
    if(businessid == -1) return 1;
    playerData[playerid][player_saveData][0] = businessid;

    s[0] = EOS;
    s = ""VIOLET"Polecenie\t"WHITE"Rezultat\n";

    catstr(s, C_VIOLET, -1, "Nazwa lokalu:\t"WHITE"%s \"%s\"\n", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]);
    catstr(s, C_VIOLET, -1, "Lokalizacja:\t"WHITE"%s, %s\n", getZoneName(unpackXYZ(businessData[businessid][business_position])), getCityName(unpackXYZ(businessData[businessid][business_position])));

    new nick[25], Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", businessData[businessid][business_ownerid]));
    if(cache_num_rows() > 0) {
        cache_get_value(0, 0, nick, sizeof(nick));
        catstr(s, C_VIOLET, -1, "W³aœciciel:\t"WHITE"%s\n", nick);
    }
    else catstr(s, C_VIOLET, -1, "W³aœciciel:\t"WHITE"Wolny lokal\n");
    cache_delete(result);
    strcat(s, " \n");

    catstr(s, C_GITOWY, -1, "  "ARROW" "WHITE"WejdŸ do œrodka lokalu\n");
    catstr(s, C_GITOWY, -1, "  "ARROW" "WHITE"SprawdŸ konkurencyjne biznesy\n");

    if(businessData[businessid][business_ownerid] == -1) catstr(s, C_GITOWY, -1, "  "ARROW" "WHITE"Zakup ten biznes ("LY"%s$)\n", strdot(businessData[businessid][business_cost]));
    else if(businessData[businessid][business_ownerid] == playerData[playerid][player_accountID]) catstr(s, C_GITOWY, -1, "  "ARROW" "WHITE"Zarz¹dzaj swoim biznesem\n");

    new title[128]; format(title, sizeof(title), ""BLUE2"Biznes "WHITE"%s \"%s\"", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]);
    return showDialog(playerid, DIALOG_BUSINESS_PANEL, DIALOG_STYLE_TABLIST_HEADERS, title, s, "Wybierz", "Zamknij"), 1;
}

stock ShowBusinessManage(playerid, businessid)
{
    if(businessid == -1) return 1;
    playerData[playerid][player_saveData][0] = businessid;

    s[0] = EOS;
    s = ""FAJNY"Polecenie\t"WHITE"Rezultat\n";

    catstr(s, C_FAJNY, -1, "Zmieñ nazwê biznesu:\t"WHITE"%s\n", businessData[businessid][business_name]);
    catstr(s, C_FAJNY, -1, "Dochód biznesu:\t"WHITE"%s$\n", strdot(businessData[businessid][business_reward]));

    switch(businessData[businessid][business_type]) {
        case BUSINESS_CASINO: catstr(s, C_FAJNY, -1, "Koszt ¿etonu:\t"WHITE"%d pkt. respektu\n", businessData[businessid][business_price]);
        case BUSINESS_GUNSHOP: catstr(s, C_FAJNY, -1, "Licencja na broñ miotan¹:\t%s\n", (businessData[businessid][business_license]) ? (""GREEN"TAK") : (""RED"NIE"));
    }
    return showDialog(playerid, DIALOG_BUSINESS_MANAGE, DIALOG_STYLE_TABLIST_HEADERS, ""BROWN"Zarz¹dzanie biznesem", s, "Wybierz", "Cofnij"), 1;
}

stock OnBusinessResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new businessid = playerData[playerid][player_saveData][0];
    switch(dialogid) {
        case DIALOG_BUSINESS_PRICE: {
            if(!response) return ShowBusinessManage(playerid, businessid);

            new amount;
            switch(businessData[businessid][business_type]) {
                case BUSINESS_CASINO: if(sscanf(inputtext, "d", amount) || amount < 10 || amount > 70) return showDialog(playerid, DIALOG_BUSINESS_PRICE, DIALOG_STYLE_INPUT, ""GOLD"Koszt ¿etonu", ""WHITE"Wpisz poni¿ej koszt ¿etonu w przedziale "LY"10-70"WHITE" pkt. respektu:\n\n"RED"Nieprawid³owa wartoœæ respektu.", "Ustaw", "Cofnij");
            }
            
            businessData[businessid][business_price] = amount;
            m_pquery_format("UPDATE es_business SET price=%d WHERE id=%d LIMIT 1;", amount, businessid);
            return ShowBusinessManage(playerid, businessid), 1;
        }

        case DIALOG_BUSINESS_REWARD: {
            if(!response) return ShowBusinessManage(playerid, businessid);

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1 || amount > businessData[businessid][business_reward]) {
                s[0] = EOS;
                catstr(s, C_FAJNY, -1, "Dochód Twojego biznesu wynosi: {c}%s${/c}.", strdot(businessData[businessid][business_reward]));
                catstr(s, C_FAJNY, -1, "Wpisz poni¿ej, ile {c}gotówki{/c} chcesz wyp³aciæ:\n\n");
                catstr(s, C_DRED, -1, "Nieprawid³owa iloœæ wyp³acanego dochodu.");
                return showDialog(playerid, DIALOG_BUSINESS_REWARD, DIALOG_STYLE_INPUT, ""ORANGE"Wyp³ata dochodu biznesu", s, "Wyp³aæ", "Cofnij");
            }

            businessData[businessid][business_reward] -= amount;
            m_pquery_format("UPDATE es_business SET reward=%d WHERE id=%d LIMIT 1;", businessData[businessid][business_reward], businessid);

            giveMoney(playerid, amount);
            msg(playerid, C_ZAJEBISTY, "Wyp³aci³eœ(aœ) {c}%s${/c} ze swojego konta biznesowego.", strdot(amount));
            return ShowBusinessManage(playerid, businessid), 1;
        }

        case DIALOG_BUSINESS_MANAGE: {
            if(!response) return ShowBusinessPanel(playerid, businessid);

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_BUSINESS_RENAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana nazwy biznesu", ""WHITE"Wpisz poni¿ej now¹ nazwê dla Twojego biznesu:", "Ustaw", "Cofnij");
                case 1: {
                    s[0] = EOS;
                    catstr(s, C_FAJNY, -1, "Dochód Twojego biznesu wynosi: {c}%s${/c}.", strdot(businessData[businessid][business_reward]));
                    catstr(s, C_FAJNY, -1, "Wpisz poni¿ej, ile {c}gotówki{/c} chcesz wyp³aciæ:");
                    showDialog(playerid, DIALOG_BUSINESS_REWARD, DIALOG_STYLE_INPUT, ""ORANGE"Wyp³ata dochodu biznesu", s, "Wyp³aæ", "Cofnij");
                }

                case 2: {
                    switch(businessData[businessid][business_type]) {
                        case BUSINESS_CASINO: showDialog(playerid, DIALOG_BUSINESS_PRICE, DIALOG_STYLE_INPUT, ""GOLD"Koszt ¿etonu", ""WHITE"Wpisz poni¿ej koszt ¿etonu w przedziale "LY"10-70"WHITE"$:", "Ustaw", "Cofnij");
                        case BUSINESS_GUNSHOP: {
                            if(businessData[businessid][business_license]) return dialogBox(playerid, C_BLUE2, "Twój {c}sklep z broni¹{/c} posiada ju¿ licencjê na broñ miotan¹!");
                            if(getMoney(playerid) < 300000) return valueError(playerid, 300000);

                            s[0] = EOS;
                            catstr(s, C_FAJNY, -1, "Czy napewno chcesz zakupiæ {c}licencjê{/c} na sprzeda¿ {c}broni miotanej{/c}?\n");
                            catstr(s, C_FAJNY, -1, "Koszt tej transakcji wyniesie: {c}300.000${/c}, a zwrot nie bêdzie mo¿liwy!\n");
                            catstr(s, C_LY, -1, "Kliknij poni¿ej odpowiedni przycisk, aby zakupiæ licencjê.");

                            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                                #pragma unused pp, dd, ll, ii
                                if(!rr) return ShowBusinessManage(playerid, businessid);
                                if(getMoney(playerid) < 300000) return valueError(playerid, 300000);

                                giveMoney(playerid, -300000);
                                businessData[businessid][business_license] = true;
                                m_pquery_format("UPDATE es_business SET gun_bonus=1 WHERE id=%d LIMIT 1;", businessid);
                                return msg(playerid, C_GITOWY, "Zakupi³eœ(aœ) {c}licencjê{/c} na sprzeda¿ {c}broni miotanej{/c} - koszt: {c}300.000${/c}."), 1;
                            }
                            Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""MINT"Licencja na broñ miotan¹", s, "Zakup", "Cofnij");
                        }
                    }
                }
            }
            return 1;
        }

        case DIALOG_BUSINESS_RENAME: {
            if(!response) return ShowBusinessManage(playerid, businessid);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 5 || strlen(dest) > 20) return showDialog(playerid, DIALOG_BUSINESS_RENAME, DIALOG_STYLE_INPUT, ""YELLOW"Zmiana nazwy biznesu", ""WHITE"Wpisz poni¿ej now¹ nazwê dla Twojego biznesu:\n\n"RED"Dozwolony limit znaków to: "WHITE"5-20 "RED"znaków.", "Ustaw", "Cofnij");
            
            format(businessData[businessid][business_name], 50, "%s", dest);
            m_pquery_format("UPDATE es_business SET name='%s' WHERE id=%d LIMIT 1;", dest, businessid);
            UpdateDynamic3DTextLabelText(businessData[businessid][business_label], -1, UpdateBusinessLabel(businessid));
            return ShowBusinessManage(playerid, businessid), 1;
        }

        case DIALOG_BUSINESS_PANEL: {
            if(!response) return 1;

            switch(listitem) {
                case 6: {
                    if(businessData[businessid][business_ownerid] != -1) {
                        if(businessData[businessid][business_ownerid] == playerData[playerid][player_accountID]) ShowBusinessManage(playerid, businessid);
                        else {
                            msg(playerid, -1, "tutaj bedzie opcja odkupu");   
                        }
                    } else {
                        if(getMoney(playerid) < businessData[businessid][business_cost]) return valueError(playerid, businessData[businessid][business_cost]);
                        
                        s[0] = EOS;
                        catstr(s, C_GITOWY, -1, "Nazwa biznesu: {c}%s \"%s\"{/c}\n", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]);
                        catstr(s, C_GITOWY, -1, "Koszt zakupu: {c}%s${/c}\n\n", strdot(businessData[businessid][business_cost]));
                        
                        catstr(s, C_LBLUE, -1, "Kliknij poni¿ej {c}odpowiedni przycisk{/c}, aby móc zakupiæ ten biznes!\n");
                        catstr(s, C_LBLUE, -1, "Mo¿liwoœæ sprzeda¿y biznesu bêdzie mo¿liwe po up³ywie {c}3{/c} dni.");

                        inline onBusinessBuy(pp, dd, rr, ll, string:ii[]) {
                            #pragma unused pp, dd, ll, ii
                            if(!rr || businessData[businessid][business_ownerid] != -1) return ShowBusinessPanel(playerid, businessid);

                            if(getMoney(playerid) < businessData[businessid][business_cost]) return valueError(playerid, businessData[businessid][business_cost]);
                            giveMoney(playerid, -businessData[businessid][business_cost]);
                            addLevelScore(playerid, math_random(300, 500));

                            s[0] = EOS;
                            catstr(s, C_ZAJEBISTY, -1, "Zakupi³eœ(aœ) swój %d biznes: {c}%s \"%s\"{/c}\n", 1, GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]);
                            catstr(s, C_ZAJEBISTY, -1, "Koszt zakupu wyniós³: {c}%s${/c}\n\n", strdot(businessData[businessid][business_cost]));
                            catstr(s, C_FAJNY, -1, "Od teraz mo¿esz siê teleportowaæ do swojego biznesu bezpoœrednio z u¿yciem: {c}/biznesy{/c}.\n");
                            catstr(s, C_FAJNY, -1, "Zarz¹dzanie biznesem znajdziesz pod skrótem klawiszowym {c}H{/c}, bêd¹c przy biznesie!");
                            showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Zakupiono biznes", s, "OK", #);

                            format(businessData[businessid][business_ownername], 25, "%s", getNick(playerid));
                            businessData[businessid][business_ownerid] = playerData[playerid][player_accountID];
                            
                            m_pquery_format("UPDATE es_business SET ownerid=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID], businessid);
                            return UpdateDynamic3DTextLabelText(businessData[businessid][business_label], -1, UpdateBusinessLabel(businessid)), 1;
                        }
                        Dialog_ShowCallback(playerid, using inline onBusinessBuy, DIALOG_STYLE_MSGBOX, ""BLUE"Zakup biznesu", s, "Zakup", "Cofnij");
                    }
                }

                case 4: EnterBusiness(playerid, businessid);

                case 5: {
                    s[0] = EOS;
                    s = ""WHITE"Nazwa biznesu\t"GREEN"Lokalizacja\t"LY"W³aœciciel\n";

                    new x = 0;
                    foreach(new i : Business) {
                        if(businessData[i][business_type] == businessData[businessid][business_type] && businessData[i][business_ownerid] != -1) {
                            new nick[25], Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", businessData[i][business_ownerid]));
                            if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
                            cache_delete(result);

                            format(s, sizeof(s), "%s"WHITE"%s\t"GREEN"%s, %s\t"LY"%s\n", s, businessData[i][business_name], getZoneName(unpackXYZ(businessData[i][business_position])), getCityName(unpackXYZ(businessData[i][business_position])), nick);
                            playerData[playerid][player_businessData][x++] = i;
                        }
                    }
                    if(!x) return dialogBox(playerid, C_RED, "Brak konkurencyjnych biznesów typu: {c}%s{/c}.", GetBusinessType(businessData[businessid][business_type]));
                    
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        if(!rr) return ShowBusinessPanel(playerid, businessid);
                        return OnDialogResponse(playerid, DIALOG_BUSINESS_PANEL, 1, 6, ""), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""MINT"Konkurencyjne biznesy", s, "OK", #);
                }

                default: ShowBusinessPanel(playerid, businessid);
            }
            return 1;
        }
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new businessid = GetNearestBusiness(playerid);
    if(businessid != -1 && PRESSED(KEY_CTRL_BACK) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) return ShowBusinessPanel(playerid, businessid);
    return 1;
}

hook OnPlayerPickUpPickup(playerid, pickupid)
{
    new businessid = playerData[playerid][player_business];
    if(businessid != -1 && pickupid == businessData[businessid][business_pickupExit]) {
        if(pBusinessTimer[playerid]) KillTimer(pBusinessTimer[playerid]);
        for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);

        playerData[playerid][player_business] = -1;
        LoadWeaponData(playerid);
        return Teleport(playerid, false, unpackXYZ(businessData[businessid][business_position]), 0.0, 0, 0, false), 1;
    }
    return 1;
}