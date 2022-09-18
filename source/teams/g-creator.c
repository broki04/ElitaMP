stock ShowGangCreator(playerid, type) 
{
    if(getMoney(playerid) < GANG_PRICE_MONEY) return valueError(playerid, GANG_PRICE_MONEY);

    s[0] = EOS;
    switch(type) {
        case 1: showDialog(playerid, DIALOG_GANG_CREATOR_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Kreator zespo�u - nazwa", ""WHITE"Wpisz poni�ej nazw� dla Twojego zespo�u:", "Dalej", "Cofnij");
        case 2: showDialog(playerid, DIALOG_GANG_CREATOR_TAG, DIALOG_STYLE_INPUT, ""YELLOW"Kreator zespo�u - tag", ""WHITE"Wpisz poni�ej tag dla Twojego zespo�u:", "Dalej", "Cofnij");
        case 3: {
            for(new i = 0; i < sizeof(KoloryGraczy); i++) AddDialogListitem(playerid, "{%06x}#%d\n", KoloryGraczy[i] >>> 8, i + 1);
            showDialog(playerid, DIALOG_GANG_CREATOR_COLOR, DIALOG_STYLE_LIST, ""YELLOW"Kreator zespo�u - kolor", #, "Wybierz", "Cofnij", 10);
        }
    }
    return 1;
}

stock OnDialogGangCreator(playerid, dialogid, response, listitem, inputtext[]) 
{
    switch(dialogid) {
        case DIALOG_GANG_CREATOR_NAME: {
            if(!response || Iter_Free(Gangs) == -1) return 1;
            if(!IsValidGangBuffer(inputtext, 5, 25)) return showDialog(playerid, DIALOG_GANG_CREATOR_NAME, DIALOG_STYLE_INPUT, ""YELLOW"Kreator zespo�u - nazwa", ""WHITE"Wpisz poni�ej nazw� dla Twojego zespo�u:\n\n"RED"Wprowadzono nieprawid�owe znaki, b�d� przekroczono limit znak�w "WHITE"5-20.", "Dalej", "Cofnij");

            format(pGangCreatorName[playerid], 50, "%s", inputtext);
            return ShowGangCreator(playerid, 2), 1;
        }
        
        case DIALOG_GANG_CREATOR_TAG: {
            if(!response || Iter_Free(Gangs) == -1) return ShowGangCreator(playerid, 1);
            if(!IsValidGangBuffer(inputtext, 2, 5)) return showDialog(playerid, DIALOG_GANG_CREATOR_TAG, DIALOG_STYLE_INPUT, ""YELLOW"Kreator zespo�u - tag", ""WHITE"Wpisz poni�ej tag dla Twojego zespo�u:\n\n"RED"Wprowadzono nieprawid�owe znaki, b�d� przekroczono limit znak�w "WHITE"2-5.", "Dalej", "Cofnij");
        
            format(pGangCreatorTag[playerid], 20, "%s", inputtext);
            return ShowGangCreator(playerid, 3), 1;
        }
        
        case DIALOG_GANG_CREATOR_COLOR: {
            if(!response) return ShowGangCreator(playerid, 2);
            if(listitem != INVALID_LISTITEM) {
                pGangCreatorColor[playerid] = KoloryGraczy[listitem];

                s[0] = EOS;
                strcat(s, ""YELLOW"Podsumowanie kreatora\n\n");
                format(s, sizeof(s), "%s"WHITE"Nazwa zespo�u: {%06x}%s [%s]\n", s, pGangCreatorColor[playerid] >>> 8, pGangCreatorName[playerid], pGangCreatorTag[playerid]);
                format(s, sizeof(s), "%s"WHITE"Cena zespo�u: {%06x}%s$\n", s, pGangCreatorColor[playerid] >>> 8, strdot(GANG_PRICE_MONEY));
                strcat(s, " \n");
                strcat(s, ""LGREEN"Kliknij poni�ej odpowiedni przycisk, by m�c za�o�y� gang.\n");
                strcat(s, ""LGREEN"Zwrot p�atno�ci po za�o�eniu zespo�u nie b�dzie mo�liwy!");
                return showDialog(playerid, DIALOG_GANG_CREATOR_RESULT, DIALOG_STYLE_MSGBOX, ""GOLD"Podsumowanie kreatora zespo�u", s, "Za��", "Anuluj");
            }
        }

        case DIALOG_GANG_CREATOR_RESULT: {
            if(!response) return 1;
            
            if(Iter_Free(Gangs) == -1) return dialogBox(playerid, C_RED, ""RED"Limit zespo��w wyczerpany.");
            if(getMoney(playerid) < GANG_PRICE_MONEY) return valueError(playerid, GANG_PRICE_MONEY);

            inline onCreateGang()
            {
                pGang[playerid] = cache_insert_id();

                new gangid = pGang[playerid];
                gangData[gangid][gang_ownerid] = playerData[playerid][player_accountID];
                gangData[gangid][gang_color] = pGangCreatorColor[playerid];     
                
                format(gangData[gangid][gang_name], 50, "%s", pGangCreatorName[playerid]);
                format(gangData[gangid][gang_tag], 20, "%s", pGangCreatorTag[playerid]);
                
                if(FetchGang(gangid, 0, true)) {
                    LoadPlayerGang(playerid, true);
                    giveMoney(playerid, -GANG_PRICE_MONEY);

                    new const rankTitle[][] = {"Boss", "Szef", "Za�o�yciel", "W�a�ciciel"};
                    format(pGangRankName[playerid], 50, "%s", rankTitle[random(sizeof(rankTitle))]);
                    pGangRankColor[playerid] = KoloryGraczy[random(sizeof(KoloryGraczy))];

                    new permstr[256];
                    for(new i = 0; i < sizeof(gangRankPermissions); i++) {
                        pGangRankPerm[playerid][i] = true;
                        format(permstr, sizeof(permstr), "%s%s%d", permstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);
                    }
                    m_pquery_format("UPDATE es_accounts SET gang=%d, gang_rank='%s', gang_rank_color=%d, gang_rank_permission='%s' WHERE id=%d LIMIT 1;", gangid, pGangRankName[playerid], pGangRankColor[playerid], permstr, playerData[playerid][player_accountID]);

                    s[0] = EOS;
                    format(s, sizeof(s), ""WHITE"Gratulujemy - za�o�y�e�(a�) sw�j zesp�: {%06x}%s [%s]\n", getGangColor(gangid) >>> 8, getGangName(gangid), getGangTag(gangid));
                    format(s, sizeof(s), "%s"WHITE"Pami�taj, by zaprosi� innych graczy do swojego zespo�u!\n"WHITE"Wi�cej informacji o systemie zespo��w znajdziesz pod "RED"/teams.", s);
                    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Za�o�enie zespo�u", s, "OK", #);
                }
                return 1;
            }

            m_tquery_inline(dbconn, using inline onCreateGang, "INSERT INTO es_gangs (name, tag, color, ownerid, date_created) VALUES ('%s', '%s', %d, %d, '%s');", pGangCreatorName[playerid], pGangCreatorTag[playerid], pGangCreatorColor[playerid], playerData[playerid][player_accountID], getLocalDate());
            return 1;
        }
    }
    return 0;
}
