enum e_helpAdminCMD {
    cmd_name[25],
    cmd_description[128],
    cmd_admin,
    cmd_level
};

new const helpAdminCMD[][e_helpAdminCMD] = {
    {"/kick",       "Wyrzucasz jakiego� gracza",        MODERATOR,  1},
    {"/mute",       "Wyciszasz jakiego� gracza",        MODERATOR,  1},
    {"/delmute",    "Odciszasz jakiego� gracza",        MODERATOR,  2},
    {"/tt",         "Teleportujesz si� do kogo�",       MODERATOR,  1},
    {"/th",         "Teleportujesz kogo� do siebie",    MODERATOR,  1},
    {"/ip",         "Sprawdzasz kogo� adres IP",        MODERATOR,  1},
    {"/interiory",  "Teleportujesz si� do interioru",   MODERATOR,  1},
    {"/pedofil",    "Zostajesz serwerowym pedofilem",   MODERATOR,  1},
    {"/warn",       "Nadajesz komu� ostrze�enie",       MODERATOR,  1},
    {"/delwarn",    "Usuwasz czyje� ostrze�enie",       MODERATOR,  2},
    {"/jail",       "Wsadzasz kogo� do wi�zienia",      MODERATOR,  1},
    {"/deljail",    "Wypuszczasz kogo� na wolno��",     MODERATOR,  1},
    {"/evcreate",   "Tworzysz wydarzenie",              MODERATOR,  1},
    {"/dajmoney",   "Dajesz komu�/wszystkim got�wk�",   MODERATOR,  2},
    {"/cls",        "Czy�cisz ca�y czat",               MODERATOR,  1},
    {"/jetpack",    "U�ywasz jetpacka",                 MODERATOR,  1},

    {"/ban",        "Banujesz kogo�",                   ADMIN,      1},
    {"/offban",     "Banujesz kogo� offline",           ADMIN,      2},
    {"/god",        "Dajesz/zabierasz komu� goda",      ADMIN,      1},

    {"/banlist",    "Lista os�b zbanowanych",           VICE,       1},
    {"/checkban",   "Zarz�dzasz banem kogo�",           VICE,       1},
    {"/getuid",     "Sprawdzasz czyje� ID konta",       VICE,       1},
    {"/setvip",     "Dajesz komu�/wszystkim vipa",      VICE,       2},

    {"/rccreator",  "Kreator wy�cig�w",                 HEAD,       1},
    {"/bcreator",   "Kreator biznes�w",                 HEAD,       1},
    {"/apanel",     "Panel administratora",             HEAD,       2},
    {"/addveh",     "Tworzysz pojazd na mapie",         HEAD,       2},
    {"/csound",     "Sprawdzasz d�wi�k",                HEAD,       2}
};

CMD:acmd(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    s[0] = EOS;
    switch(playerData[playerid][player_admin]) {
        case MODERATOR: AddDialogListitem(playerid, "{00CA00}Komenda\t"WHITE"Wyja�nienie\n");
        case ADMIN: AddDialogListitem(playerid, "{206694}Komenda\t"WHITE"Wyja�nienie\n");
        case VICE: AddDialogListitem(playerid, "{3498DB}Komenda\t"WHITE"Wyja�nienie\n");
        case HEAD: AddDialogListitem(playerid, "{FD0000}Komenda\t"WHITE"Wyja�nienie\n");
    }

    for(new i = 0; i < sizeof(helpAdminCMD); i++) {
        if((playerData[playerid][player_admin] < helpAdminCMD[i][cmd_admin]) || (playerData[playerid][player_adminLevel] < helpAdminCMD[i][cmd_level])) continue;
        AddDialogListitem(playerid, "{%06x}%s\t"WHITE"%s\n", getRankColor(playerData[playerid][player_admin]) >>> 8, helpAdminCMD[i][cmd_name], helpAdminCMD[i][cmd_description]);
    }
    return showDialog(playerid, DIALOG_HELP_CMDLIST, DIALOG_STYLE_TABLIST_HEADERS, ""RED"Komendy administratora", #, "OK", "", 10), 1;
}
flags:acmd(MODERATOR)