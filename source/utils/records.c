new const recordNames[][] = {
    "Ogólna liczba graczy", 
    "Dzisiejsza liczba graczy", 
  
    "/OneDe", 
    "/Pompa", 
    "/Sniper", 
    "/Mini",

    "/CH",
    "/ZP",
    "/DA",
    "/RC",
    "/BTR",
    "/GG",
    "/DC",
    "/WP",
    "/SM",
    "/SK",
    "/WG"
};
new recordAmount[sizeof(recordNames)];
new connects, kicks, bans, messages;

enum {
    RECORD_PLAYERS = 0,
    RECORD_TODAY,
    RECORD_ONEDE,
    RECORD_POMPA,
    RECORD_SNIPER,
    RECORD_MINI,
    RECORD_CH,
    RECORD_ZP,
    RECORD_DA,
    RECORD_RC,
    RECORD_BTR,
    RECORD_GG,
    RECORD_WP,
    RECORD_SM,
    RECORD_SK,
    RECORD_WG
};

stock Init_Records()
{
    inline onFetchRecords() {
        if(!cache_num_rows()) {
            for(new i = 0; i < sizeof(recordNames); i++) recordAmount[i] = 0;
            connects = kicks = bans = messages = 0;
            return printf(" >>> [CONFIG]: Zresetowano rekordy serwera w wyniku b³êdu!"), 1;
        }

        cache_get_value_name_int(0, "connects", connects);
        cache_get_value_name_int(0, "messages", messages);
        cache_get_value_name_int(0, "kicks", kicks);
        cache_get_value_name_int(0, "bans", bans);

        new buffer[90];
        cache_get_value_name(0, "records", buffer);
        sscanf(buffer, form("p<,>a<d>[%d]", sizeof(recordNames)), recordAmount);

        recordAmount[RECORD_TODAY] = 0;
        return printf(" >>> [CONFIG]: Wczytano rekordy serwera! (G: %d, D: %d)", recordAmount[RECORD_PLAYERS], recordAmount[RECORD_TODAY]), 1;
    }

    m_tquery_inline(dbconn, using inline onFetchRecords, "SELECT * FROM es_config");
    return 1;
}

stock checkRecord(typeid, amount)
{
    if(amount > recordAmount[typeid]) {
        recordAmount[typeid] = amount;

        new record_name[30]; 
        switch(typeid) {
            case RECORD_PLAYERS: format(record_name, sizeof(record_name), "ogólnej liczby graczy");
            case RECORD_TODAY: format(record_name, sizeof(record_name), "dzisiejszej liczby graczy");
            case RECORD_ONEDE, RECORD_POMPA, RECORD_MINI, RECORD_SNIPER: format(record_name, sizeof(record_name), "areny %s", toLowerCase(recordNames[typeid]));
            case RECORD_CH, RECORD_ZP, RECORD_DA, RECORD_RC, RECORD_BTR, RECORD_GG, RECORD_WP, RECORD_SM, RECORD_SK, RECORD_WG: format(record_name, sizeof(record_name), "zabawy %s", recordNames[typeid]);
        }

        msgAll(C_ZAJEBISTY, "Pobiliœmy rekord {c}%s{/c}! "LY"(%d)", record_name, recordAmount[typeid]);
        m_pquery_format("UPDATE es_config SET records='%s'", GetRecords());
    }
    return 1;
}

stock GetRecords()
{
    new rstr[128];
    for(new i = 0; i < sizeof(recordNames); i++) format(rstr, sizeof(rstr), "%s%s%i", rstr, i == 0 ? ("") : (","), recordAmount[i]);
    return rstr;
}

alias:rekordy("records")
CMD:rekordy(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Rekord\t"GOLD"Wynik\n";
    for(new i = 0; i < sizeof(recordNames); i++) format(s, sizeof(s), "%s"WHITE"%s\t"GOLD"%d\n", s, recordNames[i], recordAmount[i]);
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, ""GREEN"Rekordy "#SERVER_TAG"", s, "OK", #), 1;
}
