#include YSI_Coding\y_hooks

enum e_raportData {
    raport_senderid,
    raport_targetid,
    raport_date[50],
    raport_reason[144],
    bool:raport_active
};
new raportData[MAX_PLAYERS + MAX_PLAYERS][e_raportData], RaportCount = 0;

stock CheckPlayerRaported(playerid)
{
    if(RaportCount < 1) return false;

    new bool:result = false;
    for(new i = 0; i < RaportCount; i++) {
        if(raportData[i][raport_active] && IsPlayerConnected(raportData[i][raport_targetid]) && raportData[i][raport_targetid] == playerid) {
            result = true;
            break;
        }
    }
    return result;
}

alias:raport("report", "zglos")
CMD:raport(playerid, params[])
{
    new id, reason[90];
    if(sscanf(params, "ds[90]", id, reason)) return syntaxError(playerid, "/raport [id] [powód]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Dozwolona d³ugoœæ powodu: "WHITE"3-20 "RED"znaków.");
    if(CheckPlayerRaported(id)) return sendError(playerid, ""RED"Ten gracz zosta³ ju¿ przez kogoœ innego zg³oszony do administracji!");

    new raportid = RaportCount;
    raportData[raportid][raport_senderid] = playerid;
    raportData[raportid][raport_targetid] = id;
    raportData[raportid][raport_active] = true;
    format(raportData[raportid][raport_reason], 144, "%s", reason);
    format(raportData[raportid][raport_date], 50, "%s", getLocalDate());
    RaportCount ++;

    msgAdmins(C_LY, ""RED"[RAPORT %d]: "LY"Gracz {c}%s(%d){/c} zg³osi³(a) {c}%s(%d){/c} za: {c}%s{/c}.", RaportCount, getNick(playerid), playerid, getNick(id), id, reason);
    return msg(playerid, C_BLUE, "Zg³osi³eœ(aœ) gracza {c}%s(%d){/c} za: {c}%s{/c}.", getNick(id), id, reason), 1;
}

alias:raporty("reporty", "reports", "raports", "zgloszenia")
CMD:raporty(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(RaportCount < 1) return dialogBox(playerid, C_RED, ""RED"Brak zg³oszeñ.");

    new x = 0;
    AddDialogListitem(playerid, ""WHITE"ID\t"BLUE"Zg³aszaj¹cy\t"ORANGE"Zg³oszony\t"LY"Powód\n");

    for(new i = 0; i < RaportCount; i++) {
        if(raportData[i][raport_active]) {
            AddDialogListitem(playerid, ""WHITE"%d.\t"BLUE"%s(%d)\t"ORANGE"%s(%d)\t"LY"%s\n", i + 1, getNick(raportData[i][raport_senderid]), raportData[i][raport_senderid], getNick(raportData[i][raport_targetid]), raportData[i][raport_targetid], raportData[i][raport_reason]);
            playerData[playerid][player_raportData][x++] = i;
        }
    }
    return showDialog(playerid, DIALOG_RAPORTY, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Zg³oszenia", #, "Wybierz", "Zamknij", 10), 1;
}
flags:raporty(MODERATOR)

stock Task:RunRaportManage(playerid, raportid)
{   
    new Task:t = task_new();

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"LY"Rezultat\n";

    catstr(s, C_WHITE, -1, "Osoba zg³aszaj¹ca:\t"LY"%s(%d)\n", getNick(raportData[raportid][raport_senderid]), raportData[raportid][raport_senderid]);
    catstr(s, C_WHITE, -1, "Osoba zg³oszona:\t"LRED"%s(%d)\n", getNick(raportData[raportid][raport_targetid]), raportData[raportid][raport_targetid]);
    catstr(s, C_WHITE, -1, "Data zg³oszenia:\t"ORANGE"%s\n", raportData[raportid][raport_date]);
    catstr(s, C_WHITE, -1, "Powód zg³oszenia:\t"GREEN"%s\n", raportData[raportid][raport_reason]);
    strcat(s, " \n");
    catstr(s, C_YELLOW, -1, "Ukaraj gracza\n");
    catstr(s, C_LRED, -1, "Specuj gracza\n");
    catstr(s, C_BLUE, -1, "Oczyœæ gracza\n");
    catstr(s, C_RED, -1, "Usuñ zg³oszenie");

    new title[128]; format(title, sizeof(title), ""GOLD"Zg³oszenie na: "WHITE"%s(%d)", getNick(raportData[raportid][raport_targetid]), raportData[raportid][raport_targetid]);
    playerData[playerid][player_saveData][0] = raportid;
    return showDialog(playerid, DIALOG_RAPORT_PANEL, DIALOG_STYLE_TABLIST_HEADERS, title, s, "Wybierz", "Cofnij"), t;
}

stock OnDialogRaport(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_RAPORT_PANEL: {
            if(!response) return callcmd::raporty(playerid);

            new i = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 0..4: task_await(RunRaportManage(playerid, playerData[playerid][player_saveData][0]));
            
                case 5: showDialog(playerid, DIALOG_RAPORT_PUNISH, DIALOG_STYLE_LIST, ""RED"Wybierz typ kary", ""WHITE"Zbanuj gracza\n"WHITE"Wyrzuæ gracza\n"WHITE"Ucisz gracza\n"WHITE"Ostrze¿ gracza\n"WHITE"UwiêŸ gracza", "Wybierz", "Cofnij");
                case 6: callcmd::spec(playerid, form("%d", raportData[i][raport_targetid]));

                case 7: {
                    raportData[i][raport_active] = false;
                    raportData[i][raport_senderid] = raportData[i][raport_targetid] = -1;

                    msgAll(C_CORAL, ""BLUE"[RAPORT %d]: "CORAL"Oczyszczono gracza {c}%s(%d){/c} z zarzutów!", RaportCount, getNick(raportData[i][raport_targetid]), raportData[i][raport_targetid]);
                    callcmd::raporty(playerid);
                }

                case 8: {
                    raportData[i][raport_active] = false;
                    raportData[i][raport_senderid] = raportData[i][raport_targetid] = -1;

                    callcmd::raporty(playerid);
                }
            }
            return 1;
        }

        case DIALOG_RAPORT_PUNISH_PARAMS: {
            if(!response) return OnDialogResponse(playerid, DIALOG_RAPORT_PANEL, 1, 5, "");

            new optionid = GetPVarInt(playerid, "player_raport_listitem"), i = playerData[playerid][player_saveData][0], expire, reason[90];
            switch(optionid) {
                case 0: {
                    if(sscanf(inputtext, "ds[90]", expire, reason)) {
                        expire = -1;
                        if(sscanf(inputtext, "s[90]", reason)) {
                            return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Nadawanie bana", ""WHITE"Wpisz poni¿ej d³ugoœæ bana (-1: perm) oraz powód bana:\n\n"RED"Nieprawid³owe parametry. Pamiêtaj, by nie dodawaæ ¿adnego przecinka!", "Nadaj", "Cofnij");
                        }
                    }

                    if(expire != -1 && (expire < 1 || expire > 30)) return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Nadawanie bana", ""WHITE"Wpisz poni¿ej d³ugoœæ bana (-1: perm) oraz powód bana:\n\n"RED"Dozwolona d³ugoœæ bana: "WHITE"1-30 "RED"dni.", "Nadaj", "Cofnij");
                    if(strlen(reason) < 3 || strlen(reason) > 20) return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Nadawanie bana", ""WHITE"Wpisz poni¿ej d³ugoœæ bana (-1: perm) oraz powód bana:\n\n"RED"Dozwolona d³ugoœæ powodu: "WHITE"3-20 "RED"znaki.", "Nadaj", "Cofnij");

                    BanPlayer(raportData[i][raport_targetid], playerid, expire, reason);
                }

                case 2: {
                    if(sscanf(inputtext, "ds[90]", expire, reason)) return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Uciszanie gracza", ""WHITE"Wpisz poni¿ej d³ugoœæ uciszenia oraz powód uciszenia:\n\n"RED"Nieprawid³owe parametry. Pamiêtaj, by nie dodawaæ ¿adnego przecinka!", "Nadaj", "Cofnij");
                    if(expire < 1 || expire > 60) return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Uciszanie gracza", ""WHITE"Wpisz poni¿ej d³ugoœæ uciszenia oraz powód uciszenia:\n\n"RED"Dozwolona d³ugoœæ uciszenia: "WHITE"1-60 "RED"minut.", "Nadaj", "Cofnij");
                    if(strlen(reason) < 3 || strlen(reason) > 20) return showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Uciszanie gracza", ""WHITE"Wpisz poni¿ej d³ugoœæ uciszenia oraz powód uciszenia:\n\n"RED"Dozwolona d³ugoœæ powodu: "WHITE"3-20 "RED"znaki.", "Nadaj", "Cofnij");
                
                    MutePlayer(raportData[i][raport_targetid], playerid, expire, reason);
                }

                case 5: {
                    // ...
                }
            }

            raportData[i][raport_active] = false;
            raportData[i][raport_senderid] = raportData[i][raport_targetid] = -1;
            return callcmd::raporty(playerid), 1;
        }

        case DIALOG_RAPORT_PUNISH: {
            if(!response) return task_await(RunRaportManage(playerid, playerData[playerid][player_saveData][0]));

            new i = playerData[playerid][player_saveData][0];
            SetPVarInt(playerid, "player_raport_listitem", listitem);

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Nadawanie bana", ""WHITE"Wpisz poni¿ej d³ugoœæ bana (-1: perm) oraz powód bana:", "Nadaj", "Cofnij");
                case 1: KickPlayer(raportData[i][raport_targetid], playerid, "Kara z raportu");
                case 2: showDialog(playerid, DIALOG_RAPORT_PUNISH_PARAMS, DIALOG_STYLE_INPUT, ""RED"Uciszanie gracza", ""WHITE"Wpisz poni¿ej d³ugoœæ uciszenia oraz powód uciszenia:", "Nadaj", "Cofnij");
                case 3: WarnPlayer(raportData[i][raport_targetid], playerid, "Kara z raportu");
                case 5: {

                }
            }
            
            if(listitem == 1 || listitem == 3) {
                raportData[i][raport_active] = false;
                raportData[i][raport_senderid] = raportData[i][raport_targetid] = -1;
                callcmd::raporty(playerid);
            }
            return 1;
        }

        case DIALOG_RAPORTY: {
            if(!response) return 1;
            if(listitem != INVALID_LISTITEM) return task_await(RunRaportManage(playerid, playerData[playerid][player_raportData][listitem]));
        }
    }
    return 0;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(RaportCount > 0) {
        for(new i = 0; i < RaportCount; i++) {
            if(raportData[i][raport_active] && (raportData[i][raport_senderid] == playerid || raportData[i][raport_targetid] == playerid)) {
                raportData[i][raport_active] = false;
                raportData[i][raport_senderid] = raportData[i][raport_targetid] = -1;
            }
        }
    }
    return 1;
}