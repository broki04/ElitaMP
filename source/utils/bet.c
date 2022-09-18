#include YSI_Coding\y_hooks

enum e_betData {
    bet_id,
    bet_name[50],
    bool:bet_gang
};

new 
    bool:pBetGang[MAX_PLAYERS],
    pBetTarget[MAX_PLAYERS],
    pBetReward[MAX_PLAYERS],
    pBetType[MAX_PLAYERS],
    bool:pBetAccept[MAX_PLAYERS],

    betData[][e_betData] = {
        {BET_DUEL,  "Wygrana w sol�wce",                false},
        {BET_GAME,  "Wygrana w zabawie",                false},
        {BET_EVENT, "Wygrana na evencie",               false},
        {BET_SPAR,  "Wygrana zespo�u na sparingu",      true},
        {BET_ZONE,  "Wygrana zespo�u w walce o teren",  true}
    }
;

hook function ResetPlayerData(playerid) {
    pBetGang[playerid] = pBetAccept[playerid] = false;
    pBetTarget[playerid] = pBetType[playerid] = -1;
    pBetReward[playerid] = 0;
    return continue(playerid);
}

stock ShowBetPanel(playerid, betid)
{
    if(betid == -1) return 1;
    pBetTarget[playerid] = betid;

    s[0] = EOS;
    catstr(s, C_BLUE2, -1, "Wybrany %s do zak�adu: {c}%s{/c}\n", (pBetGang[playerid]) ? ("gang") : ("gracz"), (pBetGang[playerid]) ? (form("%s [%s]", getGangName(betid), getGangTag(betid))) : (form("%s(%d)", getNick(betid), betid)));
    strcat(s, " \n");
    catstr(s, C_LGREEN, -1, "Postawiona kwota: {c}%s${/c}\n", strdot(pBetReward[playerid]));
    catstr(s, C_LGREEN, -1, "Kategoria zak�adu: {c}%s{/c}\n", (pBetType[playerid] == -1) ? ("---") : (form("%s", betData[pBetType[playerid]][bet_name])));
    strcat(s, " \n");
    catstr(s, C_LRED, -1, "Kliknij tutaj, aby postawi� zak�ad.");
    return showDialog(playerid, DIALOG_BET_REWARD, DIALOG_STYLE_LIST, ""BLUE"Zak�ady - waluta", s, "Postaw", "Cofnij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BET_SELECT: {
            if(!response) return callcmd::bet(playerid);
            if(listitem != INVALID_LISTITEM) return ShowBetPanel(playerid, playerData[playerid][player_saveData][listitem]);
            return 1;
        }

        case DIALOG_BET_MONEY: {
            if(!response) return ShowBetPanel(playerid, pBetTarget[playerid]);

            new amount;
            if(sscanf(inputtext, "d", amount)) {
                s[0] = EOS;
                catstr(s, C_RED, -1, "Wpisz poni�ej odpowiedni� ilo�� {c}got�wki{/c}, kt�r� chcesz postawi�:\n");
                catstr(s, C_LY, -1, "Mo�esz postawi� {c}$10.000-500.000{/c} w puli.");
                catstr(s, C_RED, -1, "Podana warto�� musi by� liczb�!");
                return showDialog(playerid, DIALOG_BET_MONEY, DIALOG_STYLE_INPUT, ""BLUE"Zak�ady - got�wka", s, "Postaw", "Cofnij");
            }

            if(amount < 10000 || amount > 500000) {
                s[0] = EOS;
                catstr(s, C_RED, -1, "Wpisz poni�ej odpowiedni� ilo�� {c}got�wki{/c}, kt�r� chcesz postawi�:\n");
                catstr(s, C_LY, -1, "Mo�esz postawi� {c}$10.000-500.000{/c} w puli.");
                return showDialog(playerid, DIALOG_BET_MONEY, DIALOG_STYLE_INPUT, ""BLUE"Zak�ady - got�wka", s, "Postaw", "Cofnij");
            }

            pBetReward[playerid] = amount;
            return ShowBetPanel(playerid, pBetTarget[playerid]), 1;
        }

        case DIALOG_BET_REWARD: {
            if(!response) return OnDialogResponse(playerid, DIALOG_BET_NEWBIE, 1, (pBetGang[playerid]) ? (2) : (1), "");

            switch(listitem) {
                case 2: {
                    s[0] = EOS;
                    catstr(s, C_RED, -1, "Wpisz poni�ej odpowiedni� ilo�� {c}got�wki{/c}, kt�r� chcesz postawi�:\n");
                    catstr(s, C_LY, -1, "Mo�esz postawi� {c}10.000$-500.000${/c} w puli.");
                    showDialog(playerid, DIALOG_BET_MONEY, DIALOG_STYLE_INPUT, ""BLUE"Zak�ady - got�wka", s, "Postaw", "Cofnij");
                }

                case 3: {
                    s[0] = EOS;

                    new x = 0;
                    for(new i = 0; i < sizeof(betData); i++) {
                        if(pBetGang[playerid] == betData[i][bet_gang]) {
                            catstr(s, C_LY, -1, ""WHITE"%s\n", betData[i][bet_name]);
                            playerData[playerid][player_betdata][x++] = i;
                        }
                    }
                    
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ii
                        if(!rr) return ShowBetPanel(playerid, pBetTarget[playerid]);
                        pBetType[playerid] = playerData[playerid][player_betdata][ll];
                        return ShowBetPanel(playerid, pBetTarget[playerid]), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""YELLOW"Zak�ady - kategoria", s, "Wybierz", "Cofnij");
                }

                case 5: {
                    if(pBetReward[playerid] < 1) return dialogBox(playerid, C_RED, "Nie postawi�e�(a�) �adnej kwoty w puli zak�adu.");
                    if(pBetType[playerid] == -1) return dialogBox(playerid, C_RED, "Nie wybra�e�(a�) {c}kategorii{/c} zak�adu.");

                    new betid = pBetTarget[playerid];
                    if(pBetGang[playerid] && (pGangSpar[betid] || gangData[betid][gang_battleZone] != -1)) return dialogBox(playerid, C_RED, "Nie mo�esz teraz {c}postawi�{/c} zak�adu na ten gang!");
                    if(!pBetGang[playerid] && NieMozeTeraz(betid)) return dialogBox(playerid, C_RED, "Nie mo�esz teraz {c}postawi�{/c} zak�adu na tego gracza!");

                    pBetAccept[playerid] = true;
                    msg(playerid, C_CORAL, "Postawi�e�(a�) {c}%s${/c} na %s: {c}%s{/c}.", strdot(pBetReward[playerid]), (pBetGang[playerid]) ? ("gang") : ("gracza"), (pBetGang[playerid]) ? (form("%s [%s]", getGangName(pBetTarget[playerid]), getGangTag(pBetTarget[playerid]))) : (form("%s(%d)", getNick(pBetTarget[playerid]), pBetTarget[playerid])));
                    msg(playerid, C_CORAL, "Kategoria zak�adu: {c}%s{/c}.", betData[pBetType[playerid]][bet_name]);
                }
                default: ShowBetPanel(playerid, pBetTarget[playerid]);
            }
            return 1;
        }

        case DIALOG_BET_NEWBIE: {
            if(!response) return 1;

            new x = 0;
            switch(listitem) {                
                case 2: {           
                    pBetGang[playerid] = true;

                    AddDialogListitem(playerid, ""YELLOW"Nazwa gangu\t"LY"Poziom\n");
                    foreach(new i : Gangs) {
                        if(countGangMembers(i) > 0 && !pGangSpar[i] && gangData[i][gang_battleZone] == -1 && pGang[playerid] != i) {
                            AddDialogListitem(playerid, "{%06x}%s [%s]\t"LY"%d lvl\n", getGangColor(i) >>> 8, getGangName(i), getGangTag(i), gangData[i][gang_level]); 
                            playerData[playerid][player_saveData][x++] = i;
                        }
                    }

                    if(!x) return dialogBox(playerid, C_RED, "Nie znaleziono �adnego gangu!");
                    showDialog(playerid, DIALOG_BET_SELECT, DIALOG_STYLE_TABLIST_HEADERS, ""CORAL"Zak�ady - wyb�r gangu", #, "Wybierz", "Cofnij", 10);
                }

                case 1: {
                    pBetGang[playerid] = false;

                    AddDialogListitem(playerid, ""YELLOW"Nick gracza\t"LY"Poziom\n");
                    foreach(new i : Player) {
                        if(!NieMozeTeraz(i) && i != playerid) {
                            AddDialogListitem(playerid, "{%06x}%s(%d)\t"LY"%d lvl\n", GetPlayerColor(i) >>> 8, getNick(i), i, playerData[i][player_level]); 
                            playerData[playerid][player_saveData][x++] = i;
                        }
                    }

                    if(!x) return dialogBox(playerid, C_RED, "Nie znaleziono �adnego gracza!");
                    showDialog(playerid, DIALOG_BET_SELECT, DIALOG_STYLE_TABLIST_HEADERS, ""CORAL"Zak�ady - wyb�r gracza", #, "Wybierz", "Cofnij", 10);
                }
                default: callcmd::bet(playerid);
            }
            return 1;
        }
    }
    return 0;
}

stock CheckBet(betid, typeid, bool:winner)
{
    foreach(new i : Player) {
        if(pBetAccept[i] && pBetTarget[i] == betid && pBetType[i] == typeid) {
            if(winner) {
                msg(i, C_CORAL, "Wygra�e�(a�) zak�ad postawiony na %s: {c}%s{/c}.", (pBetGang[i]) ? ("gang") : ("gracza"), (pBetGang[i]) ? (form("%s [%s]", getGangName(betid), getGangTag(betid))) : (form("%s(%d)", getNick(betid), betid)));
                msg(i, C_CORAL, "Otrzymujesz: {c}%s${/c}.", strdot(pBetReward[i]));

                giveMoney(i, pBetReward[i]);
                addLevelScore(i, math_random(300, 500));
            } else {
                msg(i, C_CORAL, "Przegra�e�(a�) zak�ad postawiony na %s: {c}%s{/c}.", (pBetGang[i]) ? ("gang") : ("gracza"), (pBetGang[i]) ? (form("%s [%s]", getGangName(betid), getGangTag(betid))) : (form("%s(%d)", getNick(betid), betid)));
                msg(i, C_CORAL, "Tracisz: {c}%s${/c}.", strdot(pBetReward[i]));
            
                giveMoney(i, -pBetReward[i]);
            }
            if(GetPlayerState(i) == PLAYER_STATE_SPECTATING && pSpectate[i] != -1) SpecToggle(i, pBetTarget[i], false);

            pBetTarget[i] = pBetType[i] = -1;
            pBetReward[i] = 0;
            pBetAccept[i] = pBetGang[i] = false;
        }
    }

    return 1;
}

alias:bet("betclic")
CMD:bet(playerid)
{
    if(pBetAccept[playerid] && pBetTarget[playerid] != -1) {
        s[0] = EOS;
        catstr(s, C_CORAL, -1, "Postawi�e�(a�) ju� zak�ad na %s: {c}%s{/c}.\n", (pBetGang[playerid]) ? ("gang") : ("gracza"), (pBetGang[playerid]) ? (form("%s [%s]", getGangName(pBetTarget[playerid]), getGangTag(pBetTarget[playerid]))) : (form("%s(%d)", getNick(pBetTarget[playerid]), pBetTarget[playerid])));
        catstr(s, C_CORAL, -1, "Kwota zak�adu: {c}%s${/c}\n", strdot(pBetReward[playerid]));
        catstr(s, C_CORAL, -1, "Kategoria zak�adu: {c}%s{/c}\n\n", betData[pBetType[playerid]][bet_name]);
        catstr(s, C_LRED, -1, "Kliknij poni�ej odpowiedni przycisk, aby anulowa� zak�ad.");

        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
            #pragma unused pp, dd, ll, ii
            if(!rr) return 1;

            new targetid = pBetTarget[playerid], bool:gang = pBetGang[playerid];
            pBetTarget[playerid] = pBetType[playerid] = -1;
            pBetReward[playerid] = 0;
            pBetAccept[playerid] = pBetGang[playerid] = false;
            return msg(playerid, C_LORANGE, "Anulowa�e�(a�) postawiony wcze�niej zak�ad na %s: {c}%s{/c}.", (gang) ? ("gang") : ("gracza"), (gang) ? (form("%s [%s]", getGangName(targetid), getGangTag(targetid))) : (form("%s(%d)", getNick(targetid), targetid))), 1;
        }
        Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""GOLD"Anulowanie zak�adu", s, "Tak", "Nie");
    }

    s[0] = EOS;
    strcat(s, ""WHITE"Wybierz poni�ej, na co chcesz postawi�:\n");
    strcat(s, ""BLUE""ARROW"  "LY"Postaw na gracza\n");
    strcat(s, ""BLUE""ARROW"  "LY"Postaw na gang\n");
    return showDialog(playerid, DIALOG_BET_NEWBIE, DIALOG_STYLE_LIST, ""GREEN"Zak�ady", s, "Wybierz", "Zamknij"), 1;
}

CMD:betspec(playerid) // przerobi� na gang jeszcze tako
{
    if(!pBetAccept[playerid] && pBetTarget[playerid] == -1) return sendError(playerid, ""RED"Nie postawi�e�(a�) na nikogo zak�adu!");
    SpecToggle(playerid, pBetTarget[playerid], true);
    return msg(playerid, C_BLUE2, "Rozpocz�to obserwacje {c}%s(%d){/c}, na kt�rego postawi�e�(a�) zak�ad! "LY"(/betspec)", getNick(pBetTarget[playerid]), pBetTarget[playerid]), 1;
}