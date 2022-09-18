enum e_ulepszeniaPojazdData {
    ulep_id,
    ulep_name[50],
    ulep_description[255],
    ulep_money
};

#define ULEPSZENIE_FUEL         0
#define ULEPSZENIE_DIST         1
#define ULEPSZENIE_SLOTS        2

new const ulepszeniaPojazdData[][e_ulepszeniaPojazdData] = {
    {ULEPSZENIE_FUEL,       "Wolniejsze spalanie paliwa",       "Zakup tego ulepszenia gwarantuje o wiele mniejsze spalanie paliwa, ni¿ dotychczas.",   150000},
    {ULEPSZENIE_DIST,       "Szybsze nabijanie przebiegu",      "Prêdkoœæ nabijania przebiegu wzrasta dwukrotnie za zakupie tego ulepszenia.",          100000},
    {ULEPSZENIE_SLOTS,      "Wiêcej przyczepianych obiektów",   "Dziêki temu ulepszeniu, zwiêksza siê limit obiektów z 10 do 15 na pojeŸdzie.",         500000}
};
new bool:pVehUlepszenie[MAX_PRIVCARS][MAX_PRIVCAR_ULEPSZENIA];

stock CountPojazdUlepszenia(privcarid)
{
    new count = 0;
    for(new i = 0; i < sizeof(ulepszeniaPojazdData); i++) { if(pVehUlepszenie[privcarid][i]) count ++; }
    return count;
}

stock ShowPrivCarUlepszenia(playerid, privcarid)
{
    if(privcarid == -1 || !pVehSpawned[privcarid]) return 1;

    s[0] = EOS;
    s = ""WHITE"Nazwa ulepszenia\t"BLUE"Koszt\t"GREEN"Sta"RED"tus\n";
    for(new i = 0; i < sizeof(ulepszeniaPojazdData); i++) catstr(s, -1, -1, "%s\t"BLUE"%s$\t%s\n", ulepszeniaPojazdData[i][ulep_name], strdot(ulepszeniaPojazdData[i][ulep_money]), (pVehUlepszenie[privcarid][i]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
    return showDialog(playerid, DIALOG_PRIVCAR_ULEPSZENIA, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Ulepszenia pojazdu", s, "Wybierz", "Cofnij"), 1;
}

stock getPrivCarUlepszenia(privcarid)
{
    new str[128];
    for(new i = 0; i < sizeof(ulepszeniaPojazdData); i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pVehUlepszenie[privcarid][i]);
    return str;
}

stock OnDialogPrivCarUlepszenia(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_PRIVCAR_ULEPSZENIA: {
            if(!response) return task_await(RunPrivCarManage(playerid, pVehManage[playerid]));

            new privcarid = pVehManage[playerid];
            if(pVehUlepszenie[privcarid][listitem]) return dialogBox(playerid, C_RED, ""RED"Wybrane ulepszenie jest ju¿ zakupione!");
            if(getMoney(playerid) < ulepszeniaPojazdData[listitem][ulep_money]) return valueError(playerid, ulepszeniaPojazdData[listitem][ulep_money]);

            SetPVarInt(playerid, "privcar_ulepszenie_listitem", listitem);

            s[0] = EOS;
            catstr(s, C_BLUE, -1, "Czy napewno chcesz zakupiæ ulepszenie: {c}%s{/c}?\n", ulepszeniaPojazdData[listitem][ulep_name]);
            catstr(s, C_BLUE, -1, "Bêdzie Ciê to kosztowa³o: {c}%s${/c}.\n", strdot(ulepszeniaPojazdData[listitem][ulep_money]));
            catstr(s, C_LRED, -1, "Zwrot transakcji nie bêdzie mo¿liwy!");

            new title[150]; format(title, sizeof(title), ""ORANGE"Zakup ulepszenia: "WHITE"%s", ulepszeniaPojazdData[listitem][ulep_name]);
            return showDialog(playerid, DIALOG_PRIVCAR_ULEPSZENIA_BUY, DIALOG_STYLE_MSGBOX, title, s, "Zakup", "Cofnij"), 1;
        }

        case DIALOG_PRIVCAR_ULEPSZENIA_BUY: {
            if(!response) return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 8, "");

            new ulepid = GetPVarInt(playerid, "privcar_ulepszenie_listitem"), privcarid = pVehManage[playerid];
            if(pVehUlepszenie[privcarid][ulepid]) return dialogBox(playerid, C_RED, ""RED"Wybrane ulepszenie jest ju¿ zakupione!");
            if(getMoney(playerid) < ulepszeniaPojazdData[ulepid][ulep_money]) return valueError(playerid, ulepszeniaPojazdData[ulepid][ulep_money]);

            pVehUlepszenie[privcarid][ulepid] = true;
            giveMoney(playerid, -ulepszeniaPojazdData[ulepid][ulep_money]);
            addLevelScore(playerid, math_random(10, 30));
            
            msg(playerid, C_BLUE, "Zakupi³eœ(aœ) ulepszenie: {c}\"%s\"{/c} za: {c}%s${/c}.", ulepszeniaPojazdData[ulepid][ulep_name], strdot(ulepszeniaPojazdData[ulepid][ulep_money]));
            m_pquery_format("UPDATE es_privcars SET ulepszenia='%s' WHERE id=%d LIMIT 1;", getPrivCarUlepszenia(privcarid), privcarid);
            return OnDialogResponse(playerid, DIALOG_PRIVCAR_MANAGE, 1, 8, ""), 1;
        }
    }
    return 0;
}