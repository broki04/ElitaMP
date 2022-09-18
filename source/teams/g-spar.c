new 
    bool:pGangSpar[MAX_GANGS],
    pGangSparInitiator[MAX_GANGS],
    bool:pGangSparAccepted[MAX_GANGS],
    pGangSparMembers[MAX_GANGS],
    pGangSparPoints[MAX_GANGS],
    bool:pGangSparSide[MAX_GANGS],

    bool:pGangSparRanked[MAX_GANGS][MAX_GANGS],
    pGangSparArena[MAX_GANGS][MAX_GANGS],
    pGangSparTime[MAX_GANGS][MAX_GANGS],
    pGangSparLimit[MAX_GANGS],
    bool:pGangSparSettings[MAX_GANGS][MAX_GANGS][2],
    pGangSparCountDown[MAX_GANGS][MAX_GANGS],
    pGangSparTimer[MAX_GANGS][MAX_GANGS],
    pGangSparWorld[MAX_GANGS][MAX_GANGS],

    Text:pGangSparDraw[MAX_GANGS][MAX_GANGS][4],
    Text:pGangSparOdliczanie[MAX_GANGS][MAX_GANGS]
;

enum {
    SPARING_RANK_BRONZE = 0,
    SPARING_RANK_SILVER = 1,
    SPARING_RANK_GOLD,
    SPARING_RANK_PLATINUM,
    SPARING_RANK_DIAMOND,
    SPARING_RANK_CHAMPION,
};

enum e_sparingDataAreny {
    sparing_name[30],
    Float:sparing_att[4],
    Float:sparing_def[4],
    sparing_interior
};

new const sparingDataAreny[][e_sparingDataAreny] = {
    {"LVPD",            {287.7980, 176.3647, 1007.1190, 81.3421},   {243.2459, 143.4190, 1002.9630, 359.3548},      3},
    {"WH (puste)",      {1412.2317, 2.3631, 1000.9242, 128.7403},   {1362.2521, -45.6475, 1000.9232, 314.4032},     1},
    {"WH (pe³ne)",      {1303.7581, 4.2780, 1001.0267, 136.2599},   {1258.6636, -63.1336, 1002.5004, 326.9124},     18}
};

stock Sparing_Init(gangid)
{
    pGangSparMembers[gangid] = pGangSparPoints[gangid] = 0;
    pGangSparInitiator[gangid] = -1;
    pGangSpar[gangid] = pGangSparAccepted[gangid] = false;
}

stock Sparing_Config(gangid, enemyid, bool:td = false)
{
    if(td) {
        pGangSparDraw[gangid][enemyid][0] = TextDrawCreate(535.666503, 316.681396, "LD_OTB2:butnc");
        TextDrawTextSize(pGangSparDraw[gangid][enemyid][0], 81.000000, 76.000000);
        TextDrawAlignment(pGangSparDraw[gangid][enemyid][0], 1);
        TextDrawColor(pGangSparDraw[gangid][enemyid][0], 120);
        TextDrawSetShadow(pGangSparDraw[gangid][enemyid][0], 0);
        TextDrawBackgroundColor(pGangSparDraw[gangid][enemyid][0], 255);
        TextDrawFont(pGangSparDraw[gangid][enemyid][0], 4);
        TextDrawSetProportional(pGangSparDraw[gangid][enemyid][0], false);

        pGangSparDraw[gangid][enemyid][1] = TextDrawCreate(576.333435, 313.199920, "SPARING");
        TextDrawLetterSize(pGangSparDraw[gangid][enemyid][1], 0.165333, 0.886518);
        TextDrawAlignment(pGangSparDraw[gangid][enemyid][1], 2);
        TextDrawColor(pGangSparDraw[gangid][enemyid][1], -1);
        TextDrawSetShadow(pGangSparDraw[gangid][enemyid][1], 0);
        TextDrawSetOutline(pGangSparDraw[gangid][enemyid][1], 1);
        TextDrawBackgroundColor(pGangSparDraw[gangid][enemyid][1], 255);
        TextDrawFont(pGangSparDraw[gangid][enemyid][1], 2);
        TextDrawSetProportional(pGangSparDraw[gangid][enemyid][1], true);

        pGangSparDraw[gangid][enemyid][2] = TextDrawCreate(574.666748, 325.644470, "gang1 vs gang2");
        TextDrawLetterSize(pGangSparDraw[gangid][enemyid][2], 0.136000, 0.828444);
        TextDrawTextSize(pGangSparDraw[gangid][enemyid][2], 0.000000, 489.000000);
        TextDrawAlignment(pGangSparDraw[gangid][enemyid][2], 2);
        TextDrawColor(pGangSparDraw[gangid][enemyid][2], -1);
        TextDrawSetShadow(pGangSparDraw[gangid][enemyid][2], 0);
        TextDrawSetOutline(pGangSparDraw[gangid][enemyid][2], 1);
        TextDrawBackgroundColor(pGangSparDraw[gangid][enemyid][2], 255);
        TextDrawFont(pGangSparDraw[gangid][enemyid][2], 1);
        TextDrawSetProportional(pGangSparDraw[gangid][enemyid][2], true);

        pGangSparDraw[gangid][enemyid][3] = TextDrawCreate(575.333251, 339.748046, "wynik: 10-10~n~czas: 5 min 10 sek");
        TextDrawLetterSize(pGangSparDraw[gangid][enemyid][3], 0.155000, 0.799407);
        TextDrawTextSize(pGangSparDraw[gangid][enemyid][3], 0.000000, 1071.000000);
        TextDrawAlignment(pGangSparDraw[gangid][enemyid][3], 2);
        TextDrawColor(pGangSparDraw[gangid][enemyid][3], -1);
        TextDrawSetShadow(pGangSparDraw[gangid][enemyid][3], 0);
        TextDrawSetOutline(pGangSparDraw[gangid][enemyid][3], 1);
        TextDrawBackgroundColor(pGangSparDraw[gangid][enemyid][3], 255);
        TextDrawFont(pGangSparDraw[gangid][enemyid][3], 1);
        TextDrawSetProportional(pGangSparDraw[gangid][enemyid][3], true);

        pGangSparOdliczanie[gangid][enemyid] = TextDrawCreate(576.333312, 314.444488, "SPARING~n~~n~gang1 vs gang2~n~Miejsca: 5-5~n~Pozostalo: 10 sek~n~~n~Aby sie zapisac, wpisz: /spar");
        TextDrawLetterSize(pGangSparOdliczanie[gangid][enemyid], 0.151666, 0.795258);
        TextDrawTextSize(pGangSparOdliczanie[gangid][enemyid], 0.000000, 224.000000);
        TextDrawAlignment(pGangSparOdliczanie[gangid][enemyid], 2);
        TextDrawColor(pGangSparOdliczanie[gangid][enemyid], -1);
        TextDrawSetShadow(pGangSparOdliczanie[gangid][enemyid], 0);
        TextDrawSetOutline(pGangSparOdliczanie[gangid][enemyid], 1);
        TextDrawBackgroundColor(pGangSparOdliczanie[gangid][enemyid], 255);
        TextDrawFont(pGangSparOdliczanie[gangid][enemyid], 1);
        TextDrawSetProportional(pGangSparOdliczanie[gangid][enemyid], true);
    }

    pGangSparInitiator[gangid] = pGangSparInitiator[enemyid] = -1;
    pGangSparMembers[gangid] = pGangSparMembers[enemyid] = 0;
    pGangSparPoints[gangid] = pGangSparPoints[enemyid] = 0;
    pGangSparArena[gangid][enemyid] = pGangSparTimer[gangid][enemyid] = -1;
    pGangSparTime[gangid][enemyid] = pGangSparLimit[gangid] = pGangSparCountDown[gangid][enemyid] = 0;
    pGangSparRanked[gangid][enemyid] = false;
    pGangSparSettings[gangid][enemyid] = bool:{false, false};
}

stock Task:RunSparingOptions(playerid, gangid, enemyid) 
{
    new Task:t = task_new();
    playerData[playerid][player_saveData][0] = enemyid;

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"YELLOW"Rezultat\n";
    format(s, sizeof(s), "%s"WHITE"Limit osób:\t"LRED"%s\n", s, (pGangSparLimit[gangid] > 0) ? (form("%d osób", pGangSparLimit[gangid])) : ("BRAK"));
    format(s, sizeof(s), "%s"WHITE"Czas trwania:\t"PINK"%s\n", s, (pGangSparTime[gangid][enemyid] > 0) ? (form("%d min", pGangSparTime[gangid][enemyid])) : ("BRAK"));
    format(s, sizeof(s), "%s"WHITE"Arena:\t"BLUE"%s\n", s, (pGangSparArena[gangid][enemyid] != -1) ? (form("%s", sparingDataAreny[pGangSparArena[gangid][enemyid]])) : ("BRAK"));
    strcat(s, " \n");
    format(s, sizeof(s), "%s"WHITE"Synchronizacja:\t"ORANGE"%s\n", s, (pGangSparSettings[gangid][enemyid][0]) ? ("7/7") : ("5/5"));
    format(s, sizeof(s), "%s"WHITE"Strza³y w g³owê:\t%s\n", s, (pGangSparSettings[gangid][enemyid][1]) ? (""GREEN"ON") : (""RED"OFF"));
    
    if(pGangSparRank[gangid] == SPARING_RANK_CHAMPION) format(s, sizeof(s), "%s"WHITE"Rankingowy (%s, #%d):\t%s\n", s, getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], (pGangSparRanked[gangid][enemyid]) ? (""GREEN"TAK") : (""RED"NIE"));
    else format(s, sizeof(s), "%s"WHITE"Rankingowy (%s, %d/%d):\t%s\n", s, getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], getRankingPoints(pGangSparRank[gangid]), (pGangSparRanked[gangid][enemyid]) ? (""GREEN"TAK") : (""RED"NIE"));
    strcat(s, " \n");
    strcat(s, "{BB0000}Zaproœ");
    return showDialog(playerid, DIALOG_SPARING_OPTION, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Ustawienia sparingu", s, "Wybierz", "Cofnij"), t;
}

stock OnDialogSparing(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_SPARING_SPECTATE: {
            if(!response) return 1;

            new targetid = playerData[playerid][player_sparingData][listitem + 2];
            new gangid = playerData[playerid][player_sparingGangSpec];

            switch(listitem) {
                case 0: {
                    if(!playerData[playerid][player_sparspec]) return 1;

                    playerData[playerid][player_sparingGangSpec] = -1;
                    playerData[playerid][player_sparspec] = false;

                    SpecToggle(playerid, INVALID_PLAYER_ID, false);
                }

                case 1: ShowSparingPanel(playerid, gangid);
                default: {
                    if(!pGangSpar[gangid] || (!IsPlayerConnected(targetid) || !Iter_Contains(Sparing, targetid))) return 1;
                    playerData[playerid][player_sparspec] = true;

                    msg(playerid, C_CHAT, "Kliknij {c}H{/c}, aby otworzyæ panel sterowania obserwacji sparingiem.");
                    SpecToggle(playerid, targetid, true);
                }
            }
            return 1;
        }

        case DIALOG_SPARING_OPTION: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GANG, 1, GANG_DIALOG_SPARING, "");

            new gangid = pGang[playerid], enemyid = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 3, 7: task_await(RunSparingOptions(playerid, gangid, enemyid));

                case 0: {
                    new gangid_members = countGangMembers(gangid), enemyid_members = countGangMembers(enemyid);
                    #define addOption(%0,%1,%2)    if(%0 >= %2 && %1 >= %2) format(s, sizeof(s), "%s"WHITE"%d vs %d\n", s, %2, %2)
                    
                    s[0] = EOS;
                    addOption(gangid_members, enemyid_members, 1);
                    addOption(gangid_members, enemyid_members, 2);
                    addOption(gangid_members, enemyid_members, 3);
                    addOption(gangid_members, enemyid_members, 4);
                    addOption(gangid_members, enemyid_members, 5);
                    showDialog(playerid, DIALOG_SPARING_LIMIT, DIALOG_STYLE_LIST, ""YELLOW"Limit osób na sparingu", s, "Ustaw", "Cofnij");
                }

                case 1: showDialog(playerid, DIALOG_SPARING_TIME, DIALOG_STYLE_INPUT, ""BLUE"Czas trwania sparingu", ""WHITE"Wpisz poni¿ej, ile minut ma trwaæ sparing:", "Ustaw", "Cofnij");
                case 2: {
                    s[0] = EOS;
                    for(new i = 0; i < sizeof(sparingDataAreny); i++) format(s, sizeof(s), "%s"WHITE"%s\n", s, sparingDataAreny[i][sparing_name]);
                    showDialog(playerid, DIALOG_SPARING_ARENY, DIALOG_STYLE_LIST, ""VIOLET"Wybierz arenê na sparing", s, "Ustaw", "Cofnij");
                }

                case 4, 5: {
                    pGangSparSettings[gangid][enemyid][listitem - 4] = !pGangSparSettings[gangid][enemyid][listitem - 4];
                    task_await(RunSparingOptions(playerid, gangid, enemyid));
                }

                case 6: {
                    pGangSparRanked[gangid][enemyid] = !pGangSparRanked[gangid][enemyid];
                    task_await(RunSparingOptions(playerid, gangid, enemyid));
                }

                case 8: {
                    if(pGangSpar[gangid] || pGangSpar[enemyid] || pGangSparInitiator[gangid] != -1 || pGangSparInitiator[enemyid] != -1) return 1;
                    if(pGangSparArena[gangid][enemyid] == -1) return dialogBox(playerid, C_RED, ""RED"Nie wybrano areny na sparing.");
                    if(pGangSparLimit[gangid] == 0) return dialogBox(playerid, C_RED, ""RED"Nie wybrano limitu osób na sparingu.");
                    if(pGangSparTime[gangid][enemyid] == 0) return dialogBox(playerid, C_RED, ""RED"Nie ustalono d³ugoœci trwania sparingu.");

                    pGangSparInitiator[gangid] = gangid;
                    pGangSparInitiator[enemyid] = gangid;

                    pGangSparAccepted[gangid] = true;
                    pGangSparAccepted[enemyid] = false;

                    foreach(new i : GangMember[gangid]) {
                        msg(i, getGangColor(gangid), "{c}(%s)*  {/c}Twój zespó³ zaprosi³ zespó³ {c}%s [%s]{/c} na sparing %s. {c}(%d vs %d)", getGangTag(gangid), getGangName(enemyid), getGangTag(enemyid), (pGangSparRanked[gangid][enemyid]) ? ("rankingowy") : ("nierankingowy"), pGangSparLimit[gangid], pGangSparLimit[gangid]);
                        msg(i, getGangColor(gangid), "{c}(%s)*  {/c}Czas trwania: {c}%d min{/c}, {c}%s{/c}, {c}%s{/c}, HS: {c}%s{/c}.", getGangTag(gangid), pGangSparTime[gangid][enemyid], sparingDataAreny[pGangSparArena[gangid][enemyid]][sparing_name], (pGangSparSettings[gangid][enemyid][0]) ? ("7/7") : ("5/5"), (pGangSparSettings[gangid][enemyid][1]) ? ("ON") : ("OFF"));
                    }

                    foreach(new i : GangMember[enemyid]) {
                        msg(i, getGangColor(enemyid), "{c}(%s)*  {/c}Zespó³ {c}%s [%s]{/c} zaprosi³ Wasz zespó³ na sparing %s. {c}(%d vs %d)", getGangTag(enemyid), getGangName(gangid), getGangTag(gangid), (pGangSparRanked[gangid][enemyid]) ? ("rankingowy") : ("nierankingowy"), pGangSparLimit[gangid], pGangSparLimit[gangid]);
                        msg(i, getGangColor(enemyid), "{c}(%s)*  {/c}Czas trwania: {c}%d min{/c}, {c}%s{/c}, {c}%s{/c}, HS: {c}%s{/c}.", getGangTag(enemyid), pGangSparTime[gangid][enemyid], sparingDataAreny[pGangSparArena[gangid][enemyid]][sparing_name], (pGangSparSettings[gangid][enemyid][0]) ? ("7/7") : ("5/5"), (pGangSparSettings[gangid][enemyid][1]) ? ("ON") : ("OFF"));
                        msg(i, getGangColor(enemyid), "{c}(%s)*  {/c}Aby zaakceptowaæ zaproszenie, wpisz: {c}/sparaccept{/c}.", getGangTag(enemyid));
                    }
                }
            }
            return 1;
        }

        case DIALOG_SPARING_ARENY: {
            if(!response) return task_await(RunSparingOptions(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new gangid = pGang[playerid], enemyid = playerData[playerid][player_saveData][0];
            pGangSparArena[gangid][enemyid] = listitem;
            return task_await(RunSparingOptions(playerid, gangid, enemyid));
        }

        case DIALOG_SPARING_TIME: {
            if(!response) return task_await(RunSparingOptions(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new time;
            if(sscanf(inputtext, "d", time)) return showDialog(playerid, DIALOG_SPARING_TIME, DIALOG_STYLE_INPUT, ""BLUE"Czas trwania sparingu", ""WHITE"Wpisz poni¿ej, ile minut ma trwaæ sparing:\n\n"RED"Czas trwania sparingu musi byæ podany liczb¹.", "Ustaw", "Cofnij");
            if(time < 3 || time > 10) return showDialog(playerid, DIALOG_SPARING_TIME, DIALOG_STYLE_INPUT, ""BLUE"Czas trwania sparingu", ""WHITE"Wpisz poni¿ej, ile minut ma trwaæ sparing:\n\n"RED"Dozwolony czas trwania sparingu: "WHITE"3-10 "RED"minut.", "Ustaw", "Cofnij");

            new gangid = pGang[playerid], enemyid = playerData[playerid][player_saveData][0];
            pGangSparTime[gangid][enemyid] = time;
            return task_await(RunSparingOptions(playerid, gangid, enemyid));
        }

        case DIALOG_SPARING_LIMIT: {
            if(!response) return task_await(RunSparingOptions(playerid, pGang[playerid], playerData[playerid][player_saveData][0]));

            new gangid = pGang[playerid], enemyid = playerData[playerid][player_saveData][0];
            pGangSparLimit[gangid] = pGangSparLimit[enemyid] = listitem + 1;
            return task_await(RunSparingOptions(playerid, gangid, enemyid));
        }

        case DIALOG_SPARING_ZESTAW: {
            if(!response) return playerData[playerid][player_sparingZestaw] = 0;
            playerData[playerid][player_sparingZestaw] = listitem;
            return GiveSparingZestaw(playerid, playerData[playerid][player_sparingZestaw]);
        }

        case DIALOG_SPARING_PANEL: {
            if(!response) return callcmd::team(playerid);

            if(listitem != INVALID_LISTITEM) {
                new gangid = pGang[playerid], enemyid = playerData[playerid][player_gangManage][listitem];
                if(pGangSpar[enemyid] || pGangSpar[gangid] || pGangSparInitiator[gangid] != -1 || pGangSparInitiator[enemyid] != -1) return dialogBox(playerid, C_RED, ""RED"Nie mo¿esz teraz zaprosiæ na sparing.");

                Sparing_Config(gangid, enemyid, true);
                task_await(RunSparingOptions(playerid, gangid, enemyid));
            }
            return 1;
        }
    }
    return 0;
}

stock GiveSparingZestaw(playerid, i) 
{
    ResetPlayerWeapons(playerid);

    switch(i) {
        case 0: {
            giveWeapon(playerid, 24, 1000);
            giveWeapon(playerid, 25, 1000);
        }

        case 1: {
            giveWeapon(playerid, 24, 1000);
            giveWeapon(playerid, 34, 1000);
        }

        case 2: {
            giveWeapon(playerid, 24, 1000);
            giveWeapon(playerid, 31, 1000);
        }
    }
    return 1;
}

CMD:spar(playerid) 
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym zespole.");
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");

    new gangid = pGang[playerid];
    if(pGangSpar[gangid]) return sendError(playerid, ""RED"Twój zespó³ aktualnie gra sparing. Aby móc zobaczyæ sparing, wpisz: "WHITE"/sparspec.");
    if(pGangSparInitiator[gangid] == -1) return sendError(playerid, ""RED"Twój zespó³ aktualnie nie ma ¿adnego zaproszenia na sparing.");
    if(!pGangSparAccepted[gangid] && pGangSparInitiator[gangid] != -1) return sendError(playerid, ""RED"Twój zespó³ nie przyj¹³ jeszcze zaproszenia na sparing.");
    if(pGangSparMembers[gangid] >= pGangSparLimit[gangid]) return sendError(playerid, ""RED"Dozwolona liczba osób z Twojego zespo³u zapisa³a siê na sparing.");
    if(Iter_Contains(Sparing, playerid)) return sendError(playerid, ""RED"Zapisa³eœ(aœ) siê ju¿ na sparing.");

    Iter_Add(Sparing, playerid);
    pGangSparMembers[gangid] ++; 
    return msg(playerid, C_MINT, "Zapisa³eœ(aœ) siê na {c}sparing{/c}.");
}

CMD:sparaccept(playerid) 
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym zespole.");

    new enemyid = pGang[playerid];
    if(pGangSpar[enemyid]) return sendError(playerid, ""RED"Twój zespó³ gra ju¿ sparing.");
    if(pGangSparInitiator[enemyid] == -1) return sendError(playerid, ""RED"¯aden zespó³ nie zaprosi³ Twojego zespo³u na sparing.");
    if(pGangSparAccepted[enemyid]) return sendError(playerid, ""RED"Twój zespó³ zaakceptowa³ ju¿ propozycjê sparingu.");

    new gangid = pGangSparInitiator[enemyid];
    pGangSparAccepted[gangid] = pGangSparAccepted[enemyid] = true;
    
    foreach(new i : GangMember[gangid]) {
        msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Rozpoczê³o siê odliczanie do sparingu %s! Aby siê zapisaæ, u¿yj: {c}/spar{/c}.", getGangTag(gangid), (pGangSparRanked[gangid][enemyid]) ? ("rankingowego") : ("nierankingowego"));
        TextDrawShowForPlayer(i, pGangSparOdliczanie[gangid][enemyid]);
    }

    foreach(new i : GangMember[enemyid]) {
        msg(i, getGangColor(enemyid), "{c}(%s)*{/c}  Rozpoczê³o siê odliczanie do sparingu %s! Aby siê zapisaæ, u¿yj: {c}/spar{/c}.", getGangTag(enemyid), (pGangSparRanked[gangid][enemyid]) ? ("rankingowego") : ("nierankingowego"));
        TextDrawShowForPlayer(i, pGangSparOdliczanie[gangid][enemyid]);
    }

    pGangSparMembers[gangid] = pGangSparMembers[enemyid] = 0;
    pGangSparCountDown[gangid][enemyid] = 30;
    pGangSparTimer[gangid][enemyid] = SetTimerEx("SparingCountDown", 1000, true, "dd", gangid, enemyid);
    return 1;
}

function SparingCountDown(gangid, enemyid) 
{
    if(!Iter_Contains(Gangs, gangid) || !Iter_Contains(Gangs, enemyid)) return 1;

    pGangSparCountDown[gangid][enemyid] --;
    if(pGangSparCountDown[gangid][enemyid] < 1) {
        if((pGangSparMembers[gangid] + pGangSparMembers[enemyid]) < (pGangSparLimit[gangid] + pGangSparLimit[enemyid])) return StopSparing(gangid, enemyid, 1), 0;

        KillTimer(pGangSparTimer[gangid][enemyid]);

        new arenaid = pGangSparArena[gangid][enemyid];
        pGangSparWorld[gangid][enemyid] = math_random(10000, 30000) + gangid + enemyid;

        switch(random(2)) {
            case 0: { pGangSparSide[gangid] = true; pGangSparSide[enemyid] = false; }
            case 1: { pGangSparSide[enemyid] = true; pGangSparSide[gangid] = false; }
        }

        pGangSpar[gangid] = pGangSpar[enemyid] = true;
        foreach(new i : Sparing) {
            if(pGang[i] == gangid || pGang[i] == enemyid) {
                SavePlayerPosition(i);
                ResetPlayerWeapons(i);

                playerData[i][player_god] = false;

                SetPVarInt(i, "sparing_arena", arenaid);
                SetPVarInt(i, "sparing_world", pGangSparWorld[gangid][enemyid]);

                showDialog(i, DIALOG_SPARING_ZESTAW, DIALOG_STYLE_LIST, ""BLUE"Zestawy broni", ""WHITE"Deagle + Shotgun\n"WHITE"Deagle + Sniper\n"WHITE"Deagle + M4", "Wybierz", "Anuluj");
                SparingRespawn(i, pGang[i], false);

                for(new loopid = 0; loopid < 4; loopid++) TextDrawShowForPlayer(i, pGangSparDraw[gangid][enemyid][loopid]);
            }
        }

        foreach(new i : GangMember[gangid]) TextDrawHideForPlayer(i, pGangSparOdliczanie[gangid][enemyid]);
        foreach(new i : GangMember[enemyid]) TextDrawHideForPlayer(i, pGangSparOdliczanie[gangid][enemyid]);

        pGangSparCountDown[gangid][enemyid] = pGangSparTime[gangid][enemyid] * 60;
        pGangSparTimer[gangid][enemyid] = SetTimerEx("SparingTimer", 1000, true, "dd", gangid, enemyid);
    }

    TextDrawSetString(pGangSparOdliczanie[gangid][enemyid], form("~y~SPARING %s~n~~r~~h~%s ~w~~h~vs ~b~~h~%s~n~~w~~h~Miejsca: ~r~~h~%d~w~~h~:~b~~h~%d~n~~w~~h~Pozostalo ~y~~h~%d ~w~~h~sek.~n~~n~~w~~h~Aby sie zapisac, wpisz: ~p~/spar", (pGangSparRanked[gangid][enemyid]) ? ("RANKINGOWY") : ("NIERANKINGOWY"), getGangName(gangid), getGangName(enemyid), pGangSparMembers[gangid], pGangSparMembers[enemyid], pGangSparCountDown[gangid][enemyid]));
    return 1;
}

stock SparingRespawn(playerid, gangid, bool:weapons = true)
{
    ResetPlayerWeapons(playerid);
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    SetPlayerColor(playerid, gangData[gangid][gang_color]);

    new arenaid = GetPVarInt(playerid, "sparing_arena");
    if(pGangSparSide[gangid]) Teleport(playerid, false, unpackXYZR(sparingDataAreny[arenaid][sparing_att]), sparingDataAreny[arenaid][sparing_interior], GetPVarInt(playerid, "sparing_world"));
    else Teleport(playerid, false, unpackXYZR(sparingDataAreny[arenaid][sparing_def]), sparingDataAreny[arenaid][sparing_interior], GetPVarInt(playerid, "sparing_world"));

    playerData[playerid][player_god] = true;
    SetTimerEx("SparingSK", 1000 * 2, false, "db", playerid, weapons);
    return 1;
}

function SparingSK(playerid, bool:weapons) 
{
    playerData[playerid][player_god] = false;
    if(weapons) GiveSparingZestaw(playerid, playerData[playerid][player_sparingZestaw]); 

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    return 1;
}

function SparingTimer(gangid, enemyid) 
{
    if(!Iter_Contains(Gangs, gangid) || !Iter_Contains(Gangs, enemyid)) return 1;
    if(pGangSparMembers[gangid] < 1 || pGangSparMembers[enemyid] < 1) return StopSparing(gangid, enemyid, 2);

    pGangSparCountDown[gangid][enemyid] --;
    if(pGangSparCountDown[gangid][enemyid] < 1) {
        KillTimer(pGangSparTimer[gangid][enemyid]);

        if(pGangSparPoints[gangid] > pGangSparPoints[enemyid]) CheckSparing(gangid, enemyid, gangid, enemyid, false);
        else if(pGangSparPoints[enemyid] > pGangSparPoints[gangid]) CheckSparing(enemyid, gangid, gangid, enemyid, false);
        else CheckSparing(gangid, enemyid, gangid, enemyid, true);
    }

    TextDrawSetString(pGangSparDraw[gangid][enemyid][1], form("~w~~h~SPARING %s", (pGangSparRanked[gangid][enemyid]) ? ("RANKINGOWY") : ("NIERANKINGOWY")));
    TextDrawSetString(pGangSparDraw[gangid][enemyid][2], form("~r~~h~%s ~w~~h~vs ~b~~h~%s", getGangName(gangid), getGangName(enemyid)));
    TextDrawSetString(pGangSparDraw[gangid][enemyid][3], form("~w~~h~Wynik: ~r~~h~~h~%d~w~~h~:~b~~h~~h~%d~n~~w~~h~Czas: ~p~~h~%02d min %02d sek", pGangSparPoints[gangid], pGangSparPoints[enemyid], floatround(((pGangSparCountDown[gangid][enemyid]) / 60) % 60), pGangSparCountDown[gangid][enemyid] % 60));
    return 1;
}

stock CheckSparing(winner, losser, initiator, rivality, bool:remis) 
{
    if(!Iter_Contains(Gangs, winner) || !Iter_Contains(Gangs, losser)) return 1;

    s[0] = EOS;

    new nagroda[2] = {0, 0};
    if(remis) {
        format(s, sizeof(s), ""WHITE"SPARING ZAKOÑCZONY!\n"WHITE"Zwyciêzca: {%06x}RE{%06x}MIS\n\n", getGangColor(winner) >>> 8, getGangColor(losser) >>> 8);
        msgAll(C_LBLUE, "Sparing {c}%s vs %s{/c} zakoñczony remisem! "LY"(%d:%d)", getGangName(winner), getGangName(losser), pGangSparPoints[winner], pGangSparPoints[losser]);
   
        giveGangBudget(winner, math_random(5000, 10000));
        giveGangBudget(losser, math_random(5000, 10000));

        CheckBet(winner, BET_SPAR, false);
        CheckBet(losser, BET_SPAR, false);

        gangData[winner][gang_spar_remis] ++;
        gangData[losser][gang_spar_remis] ++;
    } else {
        format(s, sizeof(s), ""WHITE"SPARING ZAKOÑCZONY!\n"WHITE"Zwyciêzca: {%06x}%s [%s]\n\n", getGangColor(winner) >>> 8, getGangName(winner), getGangTag(winner));
        msgAll(C_LBLUE, "Sparing {c}%s vs %s{/c} wygrywa {c}%s{/c}. "LY"(%d:%d)", getGangName(winner), getGangName(losser), getGangName(winner), pGangSparPoints[winner], pGangSparPoints[losser]);

        gangData[winner][gang_spar_win] ++;
        gangData[losser][gang_spar_loss] ++;

        CheckBet(winner, BET_SPAR, true);
        CheckBet(losser, BET_SPAR, false);

        giveGangBudget(winner, math_random(10000, 30000));
        giveGangBudget(losser, -math_random(5000, 10000));

        if(pGangSparRanked[initiator][rivality]) {
            nagroda[0] = math_random(10, 30);
            nagroda[1] = math_random(5, 15);

            CheckRankedMatch(winner, losser, nagroda[0], nagroda[1]);
        }
    }

    new place = 0;
    SortDeepArray(sparingKillerData, sparing_damage, .order=SORT_DESC);
    for(new i = 0; i < (pGangSparLimit[winner] + pGangSparLimit[losser]); i++) {
        if(!IsPlayerConnected(sparingKillerData[i][sparing_playerid])) continue;
        new playerid = sparingKillerData[i][sparing_playerid], kills = sparingKillerData[i][sparing_kills], Float:damage = sparingKillerData[i][sparing_damage];
        
        place ++;
        format(s, sizeof(s), "%s"BLUE"%d. {%06x}%s(%d) "WHITE"- "LY"(%d kills, %.2f damage)\n", s, place, getGangColor(pGang[playerid]) >>> 8, getNick(playerid), playerid, kills, damage);
    }

    foreach(new i : Sparing) {
        if(pGang[i] == winner || pGang[i] == losser) {
            showDialog(i, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Podsumowanie sparingu", s, "OK", #);
            LoadPlayerPosition(i);
        }
    }
    return StopSparing(initiator, rivality, -1);
}

stock StopSparing(gangid, enemyid, reason)
{
    if(!Iter_Contains(Gangs, gangid) || !Iter_Contains(Gangs, enemyid)) return 1;

    KillTimer(pGangSparTimer[gangid][enemyid]);

    Sparing_Init(gangid);
    Sparing_Init(enemyid);
    Sparing_Config(gangid, enemyid);

    new str[256];
    switch(reason) {
        case 1: strcat(str, "brak kompletnej liczby osób");
        case 2: strcat(str, "jedna z dru¿yn jest niekompletna");
        default: strcat(str, "wyst¹pi³ nieoczekiwany problem");
    }

    static const resetSparingData[e_sparingKillerData];
    foreach(new i : Player) { 
        if(pGang[i] == gangid || pGang[i] == enemyid) {
            if(reason != -1) { msg(i, getGangColor(pGang[i]), "{c}(%s)*{/c}  Sparing nie wystartowa³ - {c}%s{/c}.", getGangTag(pGang[i]), str); }
            Iter_Remove(Sparing, i); 

            sparingKillerData[i] = resetSparingData;
        }
    }

    for(new i = 0; i < 4; i++) TextDrawDestroy(pGangSparDraw[gangid][enemyid][i]);
    return TextDrawDestroy(pGangSparOdliczanie[gangid][enemyid]), 1;
}

//

CMD:sparspec(playerid)
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym zespole.");
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");

    new gangid = pGang[playerid];
    if(!pGangSpar[gangid]) return sendError(playerid, ""RED"Twój zespó³ obecnie nie gra ¿adnego sparingu.");
    return ShowSparingPanel(playerid, gangid), 1;
}

stock ShowSparingPanel(playerid, gangid)
{
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");
    if(!pGangSpar[gangid]) return sendError(playerid, ""RED"Twój zespó³ obecnie nie gra ¿adnego sparingu.");

    s[0] = EOS;
    strcat(s, ""DRED"Przestañ obserwowaæ sparing\n");
    strcat(s, " \n");

    playerData[playerid][player_sparingGangSpec] = gangid;

    new x = 0;
    foreach(new i : Sparing) {
        if(pGang[i] == gangid) {
            format(s, sizeof(s), "%s"WHITE""ARROW"  {%06x}%s(%d)\n", s, getGangColor(gangid) >>> 8, getNick(i), i);
            playerData[playerid][player_sparingData][x++] = i;
        }
    }
    return showDialog(playerid, DIALOG_SPARING_SPECTATE, DIALOG_STYLE_LIST, ""BLUE2"Ogl¹danie sparingu", s, "Wybierz", "Zamknij"), 1;
}

