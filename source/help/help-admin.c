enum e_helpAdminCMD {
    cmd_name[25],
    cmd_description[128],
    cmd_admin,
    cmd_level
};

new const helpAdminCMD[][e_helpAdminCMD] = {
    {"/kick",       "Wyrzucasz jakiegoœ gracza",        MODERATOR,  1},
    {"/mute",       "Wyciszasz jakiegoœ gracza",        MODERATOR,  1},
    {"/delmute",    "Odciszasz jakiegoœ gracza",        MODERATOR,  2},
    {"/tt",         "Teleportujesz siê do kogoœ",       MODERATOR,  1},
    {"/th",         "Teleportujesz kogoœ do siebie",    MODERATOR,  1},
    {"/ip",         "Sprawdzasz kogoœ adres IP",        MODERATOR,  1},
    {"/interiory",  "Teleportujesz siê do interioru",   MODERATOR,  1},
    {"/pedofil",    "Zostajesz serwerowym pedofilem",   MODERATOR,  1},
    {"/warn",       "Nadajesz komuœ ostrze¿enie",       MODERATOR,  1},
    {"/delwarn",    "Usuwasz czyjeœ ostrze¿enie",       MODERATOR,  2},
    {"/jail",       "Wsadzasz kogoœ do wiêzienia",      MODERATOR,  1},
    {"/deljail",    "Wypuszczasz kogoœ na wolnoœæ",     MODERATOR,  1},
    {"/evcreate",   "Tworzysz wydarzenie",              MODERATOR,  1},
    {"/dajmoney",   "Dajesz komuœ/wszystkim gotówkê",   MODERATOR,  2},
    {"/cls",        "Czyœcisz ca³y czat",               MODERATOR,  1},
    {"/jetpack",    "U¿ywasz jetpacka",                 MODERATOR,  1},

    {"/ban",        "Banujesz kogoœ",                   ADMIN,      1},
    {"/offban",     "Banujesz kogoœ offline",           ADMIN,      2},
    {"/god",        "Dajesz/zabierasz komuœ goda",      ADMIN,      1},

    {"/banlist",    "Lista osób zbanowanych",           VICE,       1},
    {"/checkban",   "Zarz¹dzasz banem kogoœ",           VICE,       1},
    {"/getuid",     "Sprawdzasz czyjeœ ID konta",       VICE,       1},
    {"/setvip",     "Dajesz komuœ/wszystkim vipa",      VICE,       2},

    {"/rccreator",  "Kreator wyœcigów",                 HEAD,       1},
    {"/bcreator",   "Kreator biznesów",                 HEAD,       1},
    {"/apanel",     "Panel administratora",             HEAD,       2},
    {"/addveh",     "Tworzysz pojazd na mapie",         HEAD,       2},
    {"/csound",     "Sprawdzasz dŸwiêk",                HEAD,       2}
};

CMD:acmd(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    s[0] = EOS;
    switch(playerData[playerid][player_admin]) {
        case MODERATOR: AddDialogListitem(playerid, "{00CA00}Komenda\t"WHITE"Wyjaœnienie\n");
        case ADMIN: AddDialogListitem(playerid, "{206694}Komenda\t"WHITE"Wyjaœnienie\n");
        case VICE: AddDialogListitem(playerid, "{3498DB}Komenda\t"WHITE"Wyjaœnienie\n");
        case HEAD: AddDialogListitem(playerid, "{FD0000}Komenda\t"WHITE"Wyjaœnienie\n");
    }

    for(new i = 0; i < sizeof(helpAdminCMD); i++) {
        if((playerData[playerid][player_admin] < helpAdminCMD[i][cmd_admin]) || (playerData[playerid][player_adminLevel] < helpAdminCMD[i][cmd_level])) continue;
        AddDialogListitem(playerid, "{%06x}%s\t"WHITE"%s\n", getRankColor(playerData[playerid][player_admin]) >>> 8, helpAdminCMD[i][cmd_name], helpAdminCMD[i][cmd_description]);
    }
    return showDialog(playerid, DIALOG_HELP_CMDLIST, DIALOG_STYLE_TABLIST_HEADERS, ""RED"Komendy administratora", #, "OK", "", 10), 1;
}
flags:acmd(MODERATOR)