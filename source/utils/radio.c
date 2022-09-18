enum e_radioData {
    radio_name[35],
    radio_url[255]
};

new const radioData[][e_radioData] = {
    {"Radio Party",                     "http://radioparty.pl/play/glowny.m3u"},
    {"Radio Party - Energy 2000",       "http://radioparty.pl/play/energy2000kat.m3u"},
    {"Radio Z³ote Przeboje",            "http://mainstream.radioagora.pl/tuba9-1.mp3"},
    {"RMF FM",                          "http://217.74.72.11:8000/rmf_fm"},
    {"RMF MAXXX",                       "http://217.74.72.10:8000/rmf_maxxx"}
};

CMD:radio(playerid)
{
    s[0] = EOS;
    for(new i = 0; i < sizeof(radioData); i++) format(s, sizeof(s), "%s"WHITE""ARROW"  "LY"%s\n", s, radioData[i][radio_name]);
    strcat(s, ""RED"Wy³¹cz radio");

    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii

        if(!rr) return 1;
        if(ll == sizeof(radioData)) return StopAudioStreamForPlayer(playerid);
        return PlayAudioStreamForPlayer(playerid, radioData[ll][radio_url]);
    }
    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""YELLOW"Radiostacje", s, "Wybierz", "Zamknij"), 1;
}