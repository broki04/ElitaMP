new
    Text:tdBox[5],
    Text:tdOnline[2],
    Text:tdMessage,
    Text:tdAreny,
    Text:tdPasek[MAX_PLAYERS],
    Text:tdLogo,
    Text:tdDate,
    Text:tdWelcome[4],
    Text:tdGameLimit,
    Text:tdGameZapisy[3],
    Text:tdEvent[4],
    Text:tdNoDM,
    Text:tdWalizka,
    Text:tdBomba,
    Text:tdCasinoButton[MAX_PLAYERS],
    Text:tdSlotButton[MAX_PLAYERS][2],
    Text:tdAnnounce,
    Text:tdEventAnn,

    PlayerText:tdMoney[2],
    PlayerText:tdLoginTime,
    PlayerText:tdPlayer[5],
    PlayerText:tdLicznik[4],
    PlayerText:tdLevel[3],
    PlayerText:tdPosition,
    PlayerText:tdPerformance[6],
    PlayerText:tdGameDraw,
    PlayerText:tdQuiz,
    PlayerText:tdAchievement[2],
    PlayerText:tdQuest,
    PlayerText:tdGameList,
    PlayerText:tdFishing[2],
    PlayerText:tdMissionDraw[2],
    PlayerText:tdFuel[5],
    PlayerText:tdFortune[10],
    PlayerText:tdCasinoSlot[14],
    PlayerText:tdInfo[3]
;

stock CreateDesign()
{
    tdWelcome[0] = TextDrawCreate(16.999990, 214.059188, "_");
    TextDrawLetterSize(tdWelcome[0], 0.797666, 12.335408);
    TextDrawTextSize(tdWelcome[0], 167.000000, 0.000000);
    TextDrawAlignment(tdWelcome[0], 1);
    TextDrawColor(tdWelcome[0], -1);
    TextDrawUseBox(tdWelcome[0], true);
    TextDrawBoxColor(tdWelcome[0], 120);
    TextDrawSetShadow(tdWelcome[0], 0);
    TextDrawBackgroundColor(tdWelcome[0], 255);
    TextDrawFont(tdWelcome[0], 1);
    TextDrawSetProportional(tdWelcome[0], true);

    tdWelcome[1] = TextDrawCreate(47.333339, 207.837020, "~y~~h~Elita~r~~h~News ~w~~h~"#VERSION"");
    TextDrawLetterSize(tdWelcome[1], 0.223666, 1.048295);
    TextDrawTextSize(tdWelcome[1], 0.000000, 512.000000);
    TextDrawAlignment(tdWelcome[1], 2);
    TextDrawColor(tdWelcome[1], -1);
    TextDrawSetShadow(tdWelcome[1], 0);
    TextDrawSetOutline(tdWelcome[1], 1);
    TextDrawBackgroundColor(tdWelcome[1], 255);
    TextDrawFont(tdWelcome[1], 1);
    TextDrawSetProportional(tdWelcome[1], true);

    s[0] = EOS;
    strcat(s, "~w~~h~Wlasnie rozpoczely sie ~r~~h~testy serwera~w~~h~.~n~");
    strcat(s, "~w~~h~Napotkane bledy nalezy zglosic ~r~~h~wlascicielowi~w~~h~.~n~~n~");

    strcat(s, "~w~~h~Nowosci na serwerze:~n~");
    strcat(s, "~p~- ~y~~h~System gangow, a w nim: ~w~~h~sparingi, przejmowanie stref...~n~");
    strcat(s, "~p~- ~y~~h~Prywatne pojazdy, a w nich: ~w~~h~gielda, obiekty, ulepszenia...~n~");
    strcat(s, "~p~- ~y~~h~Rankingowe ~w~~h~mecze sparingowe ~y~~h~tylko u nas!~n~~n~");

    strcat(s, "~w~~h~Autor aktualizacji: ~p~~h~"#AUTHOR"~n~");
    strcat(s, "~w~~h~Wersja serwera: ~p~~h~"#VERSION"~n~");

    tdWelcome[2] = TextDrawCreate(94.000030, 222.355758, s);
    TextDrawLetterSize(tdWelcome[2], 0.131666, 0.840888);
    TextDrawTextSize(tdWelcome[2], 0.000000, 796.000000);
    TextDrawAlignment(tdWelcome[2], 2);
    TextDrawColor(tdWelcome[2], -1);
    TextDrawSetShadow(tdWelcome[2], 0);
    TextDrawSetOutline(tdWelcome[2], 1);
    TextDrawBackgroundColor(tdWelcome[2], 255);
    TextDrawFont(tdWelcome[2], 1);
    TextDrawSetProportional(tdWelcome[2], true);

    tdWelcome[3] = TextDrawCreate(91.666656, 306.977661, "_");
    TextDrawLetterSize(tdWelcome[3], 0.112665, 0.869924);
    TextDrawTextSize(tdWelcome[3], 0.000000, 411.000000);
    TextDrawAlignment(tdWelcome[3], 2);
    TextDrawColor(tdWelcome[3], -1);
    TextDrawSetShadow(tdWelcome[3], 0);
    TextDrawSetOutline(tdWelcome[3], 1);
    TextDrawBackgroundColor(tdWelcome[3], 255);
    TextDrawFont(tdWelcome[3], 1);
    TextDrawSetProportional(tdWelcome[3], true);

	tdAnnounce = TextDrawCreate(321.666412, 165.110961, "broki.):~n~zapraszam na /onede");
	TextDrawLetterSize(tdAnnounce, 0.265332, 1.293037);
	TextDrawAlignment(tdAnnounce, 2);
	TextDrawColor(tdAnnounce, -1);
	TextDrawSetShadow(tdAnnounce, 0);
	TextDrawSetOutline(tdAnnounce, 1);
	TextDrawBackgroundColor(tdAnnounce, 255);
	TextDrawFont(tdAnnounce, 1);
	TextDrawSetProportional(tdAnnounce, true);
	TextDrawSetShadow(tdAnnounce, 0);

	tdEventAnn = TextDrawCreate(322.000000, 292.000000, "beka:~n~chuj do dupy");
	TextDrawAlignment(tdEventAnn, 2);
	TextDrawBackgroundColor(tdEventAnn, 255);
	TextDrawFont(tdEventAnn, 1);
	TextDrawLetterSize(tdEventAnn, 0.310000, 1.399999);
	TextDrawColor(tdEventAnn, -1);
	TextDrawSetOutline(tdEventAnn, 1);
	TextDrawSetProportional(tdEventAnn, true);
	TextDrawSetSelectable(tdEventAnn, false);

    tdWalizka = TextDrawCreate(105.666603, 341.822296, "_");
    TextDrawLetterSize(tdWalizka, 0.142000, 0.695703);
    TextDrawTextSize(tdWalizka, 441.000000, 0.000000);
    TextDrawAlignment(tdWalizka, 1);
    TextDrawColor(tdWalizka, -1);
    TextDrawSetShadow(tdWalizka, 0);
    TextDrawSetOutline(tdWalizka, 1);
    TextDrawBackgroundColor(tdWalizka, 255);
    TextDrawFont(tdWalizka, 1);
    TextDrawSetProportional(tdWalizka, true);

    tdBomba = TextDrawCreate(109.333274, 348.874176, "_");
    TextDrawLetterSize(tdBomba, 0.142000, 0.695703);
    TextDrawTextSize(tdBomba, 441.000000, 0.000000);
    TextDrawAlignment(tdBomba, 1);
    TextDrawColor(tdBomba, -1);
    TextDrawSetShadow(tdBomba, 0);
    TextDrawSetOutline(tdBomba, 1);
    TextDrawBackgroundColor(tdBomba, 255);
    TextDrawFont(tdBomba, 1);
    TextDrawSetProportional(tdBomba, true);

	tdNoDM = TextDrawCreate(520.333374, 45.644474, "~w~~h~no~r~~h~DM");
	TextDrawLetterSize(tdNoDM, 0.253000, 1.205925);
	TextDrawAlignment(tdNoDM, 2);
	TextDrawColor(tdNoDM, -1);
	TextDrawSetShadow(tdNoDM, 0);
	TextDrawSetOutline(tdNoDM, 1);
	TextDrawBackgroundColor(tdNoDM, 255);
	TextDrawFont(tdNoDM, 1);
	TextDrawSetProportional(tdNoDM, true);
	TextDrawSetShadow(tdNoDM, 0);

    tdGameLimit = TextDrawCreate(18.000000, 185.000000, "(0/1)~n~(0/1)~n~(0/1)~n~(0/1)~n~");
    TextDrawLetterSize(tdGameLimit, 0.143333, 0.670813);
    TextDrawTextSize(tdGameLimit, 376.000000, 0.000000);
    TextDrawAlignment(tdGameLimit, 1);
    TextDrawColor(tdGameLimit, -1);
    TextDrawSetShadow(tdGameLimit, 0);
    TextDrawSetOutline(tdGameLimit, 1);
    TextDrawBackgroundColor(tdGameLimit, 255);
    TextDrawFont(tdGameLimit, 1);
    TextDrawSetProportional(tdGameLimit, true);

    tdGameZapisy[0] = TextDrawCreate(406.000000, 367.000000, "_");
    TextDrawLetterSize(tdGameZapisy[0], 0.377666, 3.064296);
    TextDrawTextSize(tdGameZapisy[0], 509.000000, 0.000000);
    TextDrawAlignment(tdGameZapisy[0], 1);
    TextDrawColor(tdGameZapisy[0], -1);
    TextDrawUseBox(tdGameZapisy[0], true);
    TextDrawBoxColor(tdGameZapisy[0], 120);
    TextDrawSetShadow(tdGameZapisy[0], 0);
    TextDrawBackgroundColor(tdGameZapisy[0], 255);
    TextDrawFont(tdGameZapisy[0], 1);
    TextDrawSetProportional(tdGameZapisy[0], true);

    tdGameZapisy[1] = TextDrawCreate(458.000000, 369.000000, "Startuje zabawa ~y~~h~Chowany (/ch)~w~~h~.~n~Zapisy wciaz trwaja - ~r~~h~10 sekund~w~~h~.~n~Zapisanych osob: ~p~~h~10 osob");
    TextDrawLetterSize(tdGameZapisy[1], 0.154333, 0.853333);
    TextDrawTextSize(tdGameZapisy[1], 0.000000, 318.000000);
    TextDrawAlignment(tdGameZapisy[1], 2);
    TextDrawColor(tdGameZapisy[1], -1);
    TextDrawSetShadow(tdGameZapisy[1], 0);
    TextDrawSetOutline(tdGameZapisy[1], 1);
    TextDrawBackgroundColor(tdGameZapisy[1], 255);
    TextDrawFont(tdGameZapisy[1], 1);
    TextDrawSetProportional(tdGameZapisy[1], true);

    tdGameZapisy[2] = TextDrawCreate(458.000000, 359.000000, "ZABAWA STARTUJE!");
    TextDrawLetterSize(tdGameZapisy[2], 0.146000, 0.932148);
    TextDrawTextSize(tdGameZapisy[2], 0.000000, 1060.000000);
    TextDrawAlignment(tdGameZapisy[2], 2);
    TextDrawColor(tdGameZapisy[2], -1);
    TextDrawSetShadow(tdGameZapisy[2], 0);
    TextDrawSetOutline(tdGameZapisy[2], 1);
    TextDrawBackgroundColor(tdGameZapisy[2], 255);
    TextDrawFont(tdGameZapisy[2], 2);
    TextDrawSetProportional(tdGameZapisy[2], true);

    tdBox[0] = TextDrawCreate(-1.333276, 435.570404, "_");
    TextDrawLetterSize(tdBox[0], 0.400000, 1.600000);
    TextDrawTextSize(tdBox[0], 814.000000, 0.000000);
    TextDrawAlignment(tdBox[0], 1);
    TextDrawColor(tdBox[0], -1);
    TextDrawUseBox(tdBox[0], true);
    TextDrawBoxColor(tdBox[0], 120);
    TextDrawSetShadow(tdBox[0], 0);
    TextDrawBackgroundColor(tdBox[0], 255);
    TextDrawFont(tdBox[0], 1);
    TextDrawSetProportional(tdBox[0], true);

    for(new i = 0; i < MAX_PLAYERS; i++) {
        tdPasek[i] = TextDrawCreate(0.333319, 434.325958, "_");
        TextDrawLetterSize(tdPasek[i], 0.375665, -0.138073);
        TextDrawTextSize(tdPasek[i], 1109.000000, 0.000000);
        TextDrawAlignment(tdPasek[i], 1);
        TextDrawColor(tdPasek[i], -1);
        TextDrawUseBox(tdPasek[i], true);
        TextDrawBoxColor(tdPasek[i], -16776961);
        TextDrawSetShadow(tdPasek[i], 0);
        TextDrawBackgroundColor(tdPasek[i], 255);
        TextDrawFont(tdPasek[i], 1);
        TextDrawSetProportional(tdPasek[i], true);

        tdCasinoButton[i] = TextDrawCreate(310.333496, 301.429595, "ld_beat:cross");
        TextDrawTextSize(tdCasinoButton[i], 20.000000, 17.000000);
        TextDrawAlignment(tdCasinoButton[i], 1);
        TextDrawColor(tdCasinoButton[i], -1);
        TextDrawSetShadow(tdCasinoButton[i], 0);
        TextDrawBackgroundColor(tdCasinoButton[i], 255);
        TextDrawFont(tdCasinoButton[i], 4);
        TextDrawSetProportional(tdCasinoButton[i], false);
        TextDrawSetSelectable(tdCasinoButton[i], true);

        tdSlotButton[i][0] = TextDrawCreate(231.000000, 320.000000, "LD_CARD:cd1c");
        TextDrawTextSize(tdSlotButton[i][0], 65.000000, 12.000000);
        TextDrawAlignment(tdSlotButton[i][0], 1);
        TextDrawColor(tdSlotButton[i][0], 155);
        TextDrawSetShadow(tdSlotButton[i][0], 0);
        TextDrawBackgroundColor(tdSlotButton[i][0], 255);
        TextDrawFont(tdSlotButton[i][0], 4);
        TextDrawSetProportional(tdSlotButton[i][0], false);
        TextDrawSetSelectable(tdSlotButton[i][0], true);

        tdSlotButton[i][1] = TextDrawCreate(339.000000, 320.000000, "LD_CARD:cd1c");
        TextDrawTextSize(tdSlotButton[i][1], 65.000000, 12.000000);
        TextDrawAlignment(tdSlotButton[i][1], 1);
        TextDrawColor(tdSlotButton[i][1], 155);
        TextDrawSetShadow(tdSlotButton[i][1], 0);
        TextDrawBackgroundColor(tdSlotButton[i][1], 255);
        TextDrawFont(tdSlotButton[i][1], 4);
        TextDrawSetProportional(tdSlotButton[i][1], false);
        TextDrawSetSelectable(tdSlotButton[i][1], true);
    }

    tdBox[1] = TextDrawCreate(95.000000, 426.000000, "hud:radar_race");
    TextDrawTextSize(tdBox[1], 8.000000, 10.000000);
    TextDrawAlignment(tdBox[1], 1);
    TextDrawColor(tdBox[1], -1);
    TextDrawSetShadow(tdBox[1], 0);
    TextDrawBackgroundColor(tdBox[1], 255);
    TextDrawFont(tdBox[1], 4);
    TextDrawSetProportional(tdBox[1], false);

    tdBox[2] = TextDrawCreate(137.000000, 426.000000, "hud:radar_ammuGun");
    TextDrawTextSize(tdBox[2], 8.000000, 8.000000);
    TextDrawAlignment(tdBox[2], 1);
    TextDrawColor(tdBox[2], -1);
    TextDrawSetShadow(tdBox[2], 0);
    TextDrawBackgroundColor(tdBox[2], 255);
    TextDrawFont(tdBox[2], 4);
    TextDrawSetProportional(tdBox[2], false);

    tdBox[3] = TextDrawCreate(177.000000, 426.000000, "hud:radar_triads");
    TextDrawTextSize(tdBox[3], 7.000000, 8.000000);
    TextDrawAlignment(tdBox[3], 1);
    TextDrawColor(tdBox[3], -1);
    TextDrawSetShadow(tdBox[3], 0);
    TextDrawBackgroundColor(tdBox[3], 255);
    TextDrawFont(tdBox[3], 4);
    TextDrawSetProportional(tdBox[3], false);

    tdBox[4] = TextDrawCreate(216.000000, 426.000000, "hud:radar_dateDisco");
    TextDrawTextSize(tdBox[4], 7.000000, 8.000000);
    TextDrawAlignment(tdBox[4], 1);
    TextDrawColor(tdBox[4], -1);
    TextDrawSetShadow(tdBox[4], 0);
    TextDrawBackgroundColor(tdBox[4], 255);
    TextDrawFont(tdBox[4], 4);
    TextDrawSetProportional(tdBox[4], false);

    tdEvent[0] = TextDrawCreate(129.999847, 369.614715, "_");
    TextDrawLetterSize(tdEvent[0], 0.370666, 4.196740);
    TextDrawTextSize(tdEvent[0], 231.000000, 0.000000);
    TextDrawAlignment(tdEvent[0], 1);
    TextDrawColor(tdEvent[0], -1);
    TextDrawUseBox(tdEvent[0], true);
    TextDrawBoxColor(tdEvent[0], 120);
    TextDrawSetShadow(tdEvent[0], 0);
    TextDrawBackgroundColor(tdEvent[0], 255);
    TextDrawFont(tdEvent[0], 1);
    TextDrawSetProportional(tdEvent[0], true);

    tdEvent[1] = TextDrawCreate(182.333724, 363.807556, "STARTUJE /EVENT!");
    TextDrawLetterSize(tdEvent[1], 0.217666, 1.081481);
    TextDrawTextSize(tdEvent[1], 0.000000, 321.000000);
    TextDrawAlignment(tdEvent[1], 2);
    TextDrawColor(tdEvent[1], -1);
    TextDrawSetShadow(tdEvent[1], 0);
    TextDrawSetOutline(tdEvent[1], 1);
    TextDrawBackgroundColor(tdEvent[1], 255);
    TextDrawFont(tdEvent[1], 1);
    TextDrawSetProportional(tdEvent[1], true);

    tdEvent[2] = TextDrawCreate(181.333389, 376.251953, "Wchodzimy jak w matke orena!~n~~n~Eventowicz: broki.)~n~Liczba osob: 10");
    TextDrawLetterSize(tdEvent[2], 0.137999, 0.795259);
    TextDrawTextSize(tdEvent[2], 0.000000, 429.000000);
    TextDrawAlignment(tdEvent[2], 2);
    TextDrawColor(tdEvent[2], -1);
    TextDrawSetShadow(tdEvent[2], 0);
    TextDrawSetOutline(tdEvent[2], 1);
    TextDrawBackgroundColor(tdEvent[2], 255);
    TextDrawFont(tdEvent[2], 1);
    TextDrawSetProportional(tdEvent[2], true);

    tdEvent[3] = TextDrawCreate(234.999969, 365.051849, "20");
    TextDrawLetterSize(tdEvent[3], 0.132999, 0.720592);
    TextDrawAlignment(tdEvent[3], 3);
    TextDrawColor(tdEvent[3], -1);
    TextDrawSetShadow(tdEvent[3], 0);
    TextDrawSetOutline(tdEvent[3], 1);
    TextDrawBackgroundColor(tdEvent[3], 255);
    TextDrawFont(tdEvent[3], 1);
    TextDrawSetProportional(tdEvent[3], true);

    tdLogo = TextDrawCreate(1.333346, 428.103607, "~y~~h~n~w~~h~ew~r~~h~D~w~~h~eath~r~~h~M~w~~h~atch "VERSION"");
    TextDrawLetterSize(tdLogo, 0.142000, 0.703999);
    TextDrawTextSize(tdLogo, 782.000000, 0.000000);
    TextDrawAlignment(tdLogo, 1);
    TextDrawColor(tdLogo, -1);
    TextDrawSetShadow(tdLogo, 0);
    TextDrawSetOutline(tdLogo, 1);
    TextDrawBackgroundColor(tdLogo, 255);
    TextDrawFont(tdLogo, 1);
    TextDrawSetProportional(tdLogo, true);

    tdMessage = TextDrawCreate(638.666503, 426.444396, "Zachecamy do picia wody!");
    TextDrawLetterSize(tdMessage, 0.125999, 0.778666);
    TextDrawAlignment(tdMessage, 3);
    TextDrawColor(tdMessage, -1);
    TextDrawSetShadow(tdMessage, 0);
    TextDrawSetOutline(tdMessage, 1);
    TextDrawBackgroundColor(tdMessage, 255);
    TextDrawFont(tdMessage, 1);
    TextDrawSetProportional(tdMessage, true);

	tdOnline[0] = TextDrawCreate(15.366655, 391.749145, "155");
	TextDrawLetterSize(tdOnline[0], 0.208000, 1.040000);
	TextDrawAlignment(tdOnline[0], 2);
	TextDrawColor(tdOnline[0], -1);
	TextDrawSetShadow(tdOnline[0], 0);
	TextDrawSetOutline(tdOnline[0], 1);
	TextDrawBackgroundColor(tdOnline[0], 255);
	TextDrawFont(tdOnline[0], 1);
	TextDrawSetProportional(tdOnline[0], true);

	tdOnline[1] = TextDrawCreate(15.333330, 401.511199, "10, 10");
	TextDrawLetterSize(tdOnline[1], 0.160666, 0.815999);
	TextDrawTextSize(tdOnline[1], 0.000000, 278.000000);
	TextDrawAlignment(tdOnline[1], 2);
	TextDrawColor(tdOnline[1], -1);
	TextDrawSetShadow(tdOnline[1], 0);
	TextDrawSetOutline(tdOnline[1], 1);
	TextDrawBackgroundColor(tdOnline[1], 255);
	TextDrawFont(tdOnline[1], 1);
	TextDrawSetProportional(tdOnline[1], true);

    tdDate = TextDrawCreate(575.333190, 19.959026, "17:26~n~06.01.2022");
    TextDrawLetterSize(tdDate, 0.198666, 0.915555);
    TextDrawAlignment(tdDate, 2);
    TextDrawColor(tdDate, -1);
    TextDrawSetShadow(tdDate, 0);
    TextDrawSetOutline(tdDate, 1);
    TextDrawBackgroundColor(tdDate, 255);
    TextDrawFont(tdDate, 1);
    TextDrawSetProportional(tdDate, true);

    tdAreny = TextDrawCreate(636.000000, 438.000000, "/onede (0) - /pompa (0) - /mini (0) - /sniper (0) - /tgl");
    TextDrawLetterSize(tdAreny, 0.121999, 0.786962);
    TextDrawTextSize(tdAreny, 1022.000000, 0.000000);
    TextDrawAlignment(tdAreny, 3);
    TextDrawColor(tdAreny, -1);
    TextDrawSetShadow(tdAreny, 0);
    TextDrawSetOutline(tdAreny, 1);
    TextDrawBackgroundColor(tdAreny, 255);
    TextDrawFont(tdAreny, 2);
    TextDrawSetProportional(tdAreny, true);
}

stock CreatePlayerDesign(playerid) 
{
    tdPlayer[0] = CreatePlayerTextDraw(playerid, 2.333350, 436.814849, "broki.) (1)");
    PlayerTextDrawLetterSize(playerid, tdPlayer[0], 0.159333, 1.002665);
    PlayerTextDrawTextSize(playerid, tdPlayer[0], 500.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdPlayer[0], 1);
    PlayerTextDrawColor(playerid, tdPlayer[0], -1);
    PlayerTextDrawSetShadow(playerid, tdPlayer[0], 0);
    PlayerTextDrawSetOutline(playerid, tdPlayer[0], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPlayer[0], 255);
    PlayerTextDrawFont(playerid, tdPlayer[0], 1);
    PlayerTextDrawSetProportional(playerid, tdPlayer[0], true);

    tdPlayer[1] = CreatePlayerTextDraw(playerid, 99.000000, 435.000000, "Pkt. respektu:~n~100.000");
    PlayerTextDrawLetterSize(playerid, tdPlayer[1], 0.128665, 0.666665);
    PlayerTextDrawTextSize(playerid, tdPlayer[1], 0.000000, 423.000000);
    PlayerTextDrawAlignment(playerid, tdPlayer[1], 2);
    PlayerTextDrawColor(playerid, tdPlayer[1], -1);
    PlayerTextDrawSetShadow(playerid, tdPlayer[1], 0);
    PlayerTextDrawSetOutline(playerid, tdPlayer[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPlayer[1], 255);
    PlayerTextDrawFont(playerid, tdPlayer[1], 1);
    PlayerTextDrawSetProportional(playerid, tdPlayer[1], true);

    tdPlayer[2] = CreatePlayerTextDraw(playerid, 141.000000, 435.000000, "Poziom:~n~100");
    PlayerTextDrawLetterSize(playerid, tdPlayer[2], 0.128665, 0.666665);
    PlayerTextDrawTextSize(playerid, tdPlayer[2], 0.000000, 423.000000);
    PlayerTextDrawAlignment(playerid, tdPlayer[2], 2);
    PlayerTextDrawColor(playerid, tdPlayer[2], -1);
    PlayerTextDrawSetShadow(playerid, tdPlayer[2], 0);
    PlayerTextDrawSetOutline(playerid, tdPlayer[2], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPlayer[2], 255);
    PlayerTextDrawFont(playerid, tdPlayer[2], 1);
    PlayerTextDrawSetProportional(playerid, tdPlayer[2], true);

    tdPlayer[3] = CreatePlayerTextDraw(playerid, 180.000000, 435.000000, "Zabojstwa:~n~100.000");
    PlayerTextDrawLetterSize(playerid, tdPlayer[3], 0.128665, 0.666665);
    PlayerTextDrawTextSize(playerid, tdPlayer[3], 0.000000, 423.000000);
    PlayerTextDrawAlignment(playerid, tdPlayer[3], 2);
    PlayerTextDrawColor(playerid, tdPlayer[3], -1);
    PlayerTextDrawSetShadow(playerid, tdPlayer[3], 0);
    PlayerTextDrawSetOutline(playerid, tdPlayer[3], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPlayer[3], 255);
    PlayerTextDrawFont(playerid, tdPlayer[3], 1);
    PlayerTextDrawSetProportional(playerid, tdPlayer[3], true);

    tdPlayer[4] = CreatePlayerTextDraw(playerid, 220.000000, 435.000000, "Smierci:~n~1.000");
    PlayerTextDrawLetterSize(playerid, tdPlayer[4], 0.128665, 0.666665);
    PlayerTextDrawTextSize(playerid, tdPlayer[4], 0.000000, 423.000000);
    PlayerTextDrawAlignment(playerid, tdPlayer[4], 2);
    PlayerTextDrawColor(playerid, tdPlayer[4], -1);
    PlayerTextDrawSetShadow(playerid, tdPlayer[4], 0);
    PlayerTextDrawSetOutline(playerid, tdPlayer[4], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPlayer[4], 255);
    PlayerTextDrawFont(playerid, tdPlayer[4], 1);
    PlayerTextDrawSetProportional(playerid, tdPlayer[4], true);

    tdLevel[0] = CreatePlayerTextDraw(playerid, 275.000000, 3.000000, "1");
    PlayerTextDrawLetterSize(playerid, tdLevel[0], 0.145999, 0.708148);
    PlayerTextDrawAlignment(playerid, tdLevel[0], 3);
    PlayerTextDrawColor(playerid, tdLevel[0], -1);
    PlayerTextDrawSetShadow(playerid, tdLevel[0], 0);
    PlayerTextDrawSetOutline(playerid, tdLevel[0], 1);
    PlayerTextDrawBackgroundColor(playerid, tdLevel[0], 255);
    PlayerTextDrawFont(playerid, tdLevel[0], 3);
    PlayerTextDrawSetProportional(playerid, tdLevel[0], true);

    tdLevel[1] = CreatePlayerTextDraw(playerid, 394.000000, 3.000000, "2");
    PlayerTextDrawLetterSize(playerid, tdLevel[1], 0.145999, 0.708148);
    PlayerTextDrawAlignment(playerid, tdLevel[1], 1);
    PlayerTextDrawColor(playerid, tdLevel[1], -1);
    PlayerTextDrawSetShadow(playerid, tdLevel[1], 0);
    PlayerTextDrawSetOutline(playerid, tdLevel[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdLevel[1], 255);
    PlayerTextDrawFont(playerid, tdLevel[1], 3);
    PlayerTextDrawSetProportional(playerid, tdLevel[1], true);

    tdLevel[2] = CreatePlayerTextDraw(playerid, 334.000000, 11.000000, "10/100 exp ~g~~h~~h~(+10)");
    PlayerTextDrawLetterSize(playerid, tdLevel[2], 0.126665, 0.538074);
    PlayerTextDrawTextSize(playerid, tdLevel[2], 0.000000, 315.000000);
    PlayerTextDrawAlignment(playerid, tdLevel[2], 2);
    PlayerTextDrawColor(playerid, tdLevel[2], -1);
    PlayerTextDrawSetShadow(playerid, tdLevel[2], 0);
    PlayerTextDrawSetOutline(playerid, tdLevel[2], 1);
    PlayerTextDrawBackgroundColor(playerid, tdLevel[2], 255);
    PlayerTextDrawFont(playerid, tdLevel[2], 1);
    PlayerTextDrawSetProportional(playerid, tdLevel[2], true);

	tdQuiz = CreatePlayerTextDraw(playerid, 387.000000, 436.000000, "ROZSYPANKA:~n~broki");
	PlayerTextDrawLetterSize(playerid, tdQuiz, 0.108999, 0.616887);
	PlayerTextDrawTextSize(playerid, tdQuiz, 0.000000, 1121.000000);
	PlayerTextDrawAlignment(playerid, tdQuiz, 2);
	PlayerTextDrawColor(playerid, tdQuiz, -1);
	PlayerTextDrawSetShadow(playerid, tdQuiz, 0);
	PlayerTextDrawSetOutline(playerid, tdQuiz, 1);
	PlayerTextDrawBackgroundColor(playerid, tdQuiz, 255);
	PlayerTextDrawFont(playerid, tdQuiz, 1);
	PlayerTextDrawSetProportional(playerid, tdQuiz, true);

	tdQuest = CreatePlayerTextDraw(playerid, 303.000000, 436.000000, "ZADANIE DNIA:~n~Zabij 10 osob na dowolnej arenie.");
	PlayerTextDrawLetterSize(playerid, tdQuest, 0.108999, 0.616887);
	PlayerTextDrawTextSize(playerid, tdQuest, 0.000000, 1121.000000);
	PlayerTextDrawAlignment(playerid, tdQuest, 2);
	PlayerTextDrawColor(playerid, tdQuest, -1);
	PlayerTextDrawSetShadow(playerid, tdQuest, 0);
	PlayerTextDrawSetOutline(playerid, tdQuest, 1);
	PlayerTextDrawBackgroundColor(playerid, tdQuest, 255);
	PlayerTextDrawFont(playerid, tdQuest, 1);
	PlayerTextDrawSetProportional(playerid, tdQuest, true);

	tdLicznik[0] = CreatePlayerTextDraw(playerid, 536.000000, 376.000000, "LD_OTB2:butnc");
	PlayerTextDrawTextSize(playerid, tdLicznik[0], 80.000000, 50.000000);
	PlayerTextDrawAlignment(playerid, tdLicznik[0], 1);
	PlayerTextDrawColor(playerid, tdLicznik[0], 120);
	PlayerTextDrawSetShadow(playerid, tdLicznik[0], 0);
	PlayerTextDrawBackgroundColor(playerid, tdLicznik[0], 255);
	PlayerTextDrawFont(playerid, tdLicznik[0], 4);
	PlayerTextDrawSetProportional(playerid, tdLicznik[0], false);

	tdLicznik[1] = CreatePlayerTextDraw(playerid, 576.000000, 380.000000, "BULLET");
	PlayerTextDrawLetterSize(playerid, tdLicznik[1], 0.162666, 0.882370);
	PlayerTextDrawAlignment(playerid, tdLicznik[1], 2);
	PlayerTextDrawColor(playerid, tdLicznik[1], -1);
	PlayerTextDrawSetShadow(playerid, tdLicznik[1], 0);
	PlayerTextDrawSetOutline(playerid, tdLicznik[1], 1);
	PlayerTextDrawBackgroundColor(playerid, tdLicznik[1], 255);
	PlayerTextDrawFont(playerid, tdLicznik[1], 2);
	PlayerTextDrawSetProportional(playerid, tdLicznik[1], true);

	tdLicznik[2] = CreatePlayerTextDraw(playerid, 547.000000, 390.000000, "100 HP");
	PlayerTextDrawLetterSize(playerid, tdLicznik[2], 0.147999, 0.874074);
	PlayerTextDrawTextSize(playerid, tdLicznik[2], 773.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, tdLicznik[2], 1);
	PlayerTextDrawColor(playerid, tdLicznik[2], -1);
	PlayerTextDrawSetShadow(playerid, tdLicznik[2], 0);
	PlayerTextDrawSetOutline(playerid, tdLicznik[2], 1);
	PlayerTextDrawBackgroundColor(playerid, tdLicznik[2], 255);
	PlayerTextDrawFont(playerid, tdLicznik[2], 2);
	PlayerTextDrawSetProportional(playerid, tdLicznik[2], true);

	tdLicznik[3] = CreatePlayerTextDraw(playerid, 605.000000, 390.000000, "100 KM");
	PlayerTextDrawLetterSize(playerid, tdLicznik[3], 0.147999, 0.874074);
	PlayerTextDrawTextSize(playerid, tdLicznik[3], 773.000000, 0.000000);
	PlayerTextDrawAlignment(playerid, tdLicznik[3], 3);
	PlayerTextDrawColor(playerid, tdLicznik[3], -1);
	PlayerTextDrawSetShadow(playerid, tdLicznik[3], 0);
	PlayerTextDrawSetOutline(playerid, tdLicznik[3], 1);
	PlayerTextDrawBackgroundColor(playerid, tdLicznik[3], 255);
	PlayerTextDrawFont(playerid, tdLicznik[3], 2);
	PlayerTextDrawSetProportional(playerid, tdLicznik[3], true);

	tdFishing[0] = CreatePlayerTextDraw(playerid, 319.333465, 331.036956, "PROCES LOWIENIA");
	PlayerTextDrawLetterSize(playerid, tdFishing[0], 0.148999, 0.919703);
	PlayerTextDrawAlignment(playerid, tdFishing[0], 2);
	PlayerTextDrawColor(playerid, tdFishing[0], -1);
	PlayerTextDrawSetShadow(playerid, tdFishing[0], 0);
	PlayerTextDrawSetOutline(playerid, tdFishing[0], 1);
	PlayerTextDrawBackgroundColor(playerid, tdFishing[0], 255);
	PlayerTextDrawFont(playerid, tdFishing[0], 2);
	PlayerTextDrawSetProportional(playerid, tdFishing[0], true);
	PlayerTextDrawSetShadow(playerid, tdFishing[0], 0);

	tdFishing[1] = CreatePlayerTextDraw(playerid, 317.999969, 349.703735, "Aby lowic, klikaj ~r~~h~LPM~n~~w~~h~Szansa na trafienie legendarnej rybki: ~y~3%");
	PlayerTextDrawLetterSize(playerid, tdFishing[1], 0.138000, 0.973630);
	PlayerTextDrawAlignment(playerid, tdFishing[1], 2);
	PlayerTextDrawColor(playerid, tdFishing[1], -1);
	PlayerTextDrawSetShadow(playerid, tdFishing[1], 0);
	PlayerTextDrawSetOutline(playerid, tdFishing[1], 1);
	PlayerTextDrawBackgroundColor(playerid, tdFishing[1], 255);
	PlayerTextDrawFont(playerid, tdFishing[1], 1);
	PlayerTextDrawSetProportional(playerid, tdFishing[1], true);
	PlayerTextDrawSetShadow(playerid, tdFishing[1], 0);

	tdLoginTime = CreatePlayerTextDraw(playerid, 171.999969, 208.251800, "9 sek");
	PlayerTextDrawLetterSize(playerid, tdLoginTime, 0.137997, 0.791109);
	PlayerTextDrawAlignment(playerid, tdLoginTime, 3);
	PlayerTextDrawColor(playerid, tdLoginTime, -1);
	PlayerTextDrawSetShadow(playerid, tdLoginTime, 0);
	PlayerTextDrawSetOutline(playerid, tdLoginTime, 1);
	PlayerTextDrawBackgroundColor(playerid, tdLoginTime, 255);
	PlayerTextDrawFont(playerid, tdLoginTime, 1);
	PlayerTextDrawSetProportional(playerid, tdLoginTime, true);

    tdPosition = CreatePlayerTextDraw(playerid, 556.666809, 8.725932, "(twoj kolor) - x: 10.0, y: 10.0, z: 10.0");
    PlayerTextDrawLetterSize(playerid, tdPosition, 0.141000, 0.728889);
    PlayerTextDrawTextSize(playerid, tdPosition, 0.000000, 582.000000);
    PlayerTextDrawAlignment(playerid, tdPosition, 2);
    PlayerTextDrawColor(playerid, tdPosition, -1);
    PlayerTextDrawSetShadow(playerid, tdPosition, 0);
    PlayerTextDrawSetOutline(playerid, tdPosition, 1);
    PlayerTextDrawBackgroundColor(playerid, tdPosition, 255);
    PlayerTextDrawFont(playerid, tdPosition, 1);
    PlayerTextDrawSetProportional(playerid, tdPosition, true);

	tdMissionDraw[0] = CreatePlayerTextDraw(playerid, 320.000000, 180.000000, "~r~mission failed!");
	PlayerTextDrawAlignment(playerid, tdMissionDraw[0], 2);
	PlayerTextDrawBackgroundColor(playerid, tdMissionDraw[0], 255);
	PlayerTextDrawFont(playerid, tdMissionDraw[0], 3);
	PlayerTextDrawLetterSize(playerid, tdMissionDraw[0], 0.800000, 2.000000);
	PlayerTextDrawColor(playerid, tdMissionDraw[0], -1);
	PlayerTextDrawSetOutline(playerid, tdMissionDraw[0], 1);
	PlayerTextDrawSetProportional(playerid, tdMissionDraw[0], true);
	PlayerTextDrawSetSelectable(playerid, tdMissionDraw[0], false);

	tdMissionDraw[1] = CreatePlayerTextDraw(playerid, 317.000000, 200.000000, "wyjebales sie zjebie brawo kurwa");
	PlayerTextDrawAlignment(playerid, tdMissionDraw[1], 2);
	PlayerTextDrawBackgroundColor(playerid, tdMissionDraw[1], 255);
	PlayerTextDrawFont(playerid, tdMissionDraw[1], 1);
	PlayerTextDrawLetterSize(playerid, tdMissionDraw[1], 0.220000, 1.100000);
	PlayerTextDrawColor(playerid, tdMissionDraw[1], -1);
	PlayerTextDrawSetOutline(playerid, tdMissionDraw[1], 1);
	PlayerTextDrawSetProportional(playerid, tdMissionDraw[1], true);
	PlayerTextDrawSetSelectable(playerid, tdMissionDraw[1], false);

    tdPerformance[0] = CreatePlayerTextDraw(playerid, 617.333496, 21.844440, "ld_beat:circle");
    PlayerTextDrawTextSize(playerid, tdPerformance[0], 18.000000, 19.000000);
    PlayerTextDrawAlignment(playerid, tdPerformance[0], 1);
    PlayerTextDrawColor(playerid, tdPerformance[0], 100);
    PlayerTextDrawSetShadow(playerid, tdPerformance[0], 0);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[0], 255);
    PlayerTextDrawFont(playerid, tdPerformance[0], 4);
    PlayerTextDrawSetProportional(playerid, tdPerformance[0], false);

    tdPerformance[1] = CreatePlayerTextDraw(playerid, 626.000000, 24.000000, "fps:~n~100");
    PlayerTextDrawLetterSize(playerid, tdPerformance[1], 0.130666, 0.737185);
    PlayerTextDrawAlignment(playerid, tdPerformance[1], 2);
    PlayerTextDrawColor(playerid, tdPerformance[1], -1);
    PlayerTextDrawSetShadow(playerid, tdPerformance[1], 0);
    PlayerTextDrawSetOutline(playerid, tdPerformance[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[1], 255);
    PlayerTextDrawFont(playerid, tdPerformance[1], 1);
    PlayerTextDrawSetProportional(playerid, tdPerformance[1], true);

    tdPerformance[2] = CreatePlayerTextDraw(playerid, 618.000000, 45.000000, "ld_beat:circle");
    PlayerTextDrawTextSize(playerid, tdPerformance[2], 18.000000, 19.000000);
    PlayerTextDrawAlignment(playerid, tdPerformance[2], 1);
    PlayerTextDrawColor(playerid, tdPerformance[2], 100);
    PlayerTextDrawSetShadow(playerid, tdPerformance[2], 0);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[2], 255);
    PlayerTextDrawFont(playerid, tdPerformance[2], 4);
    PlayerTextDrawSetProportional(playerid, tdPerformance[2], false);

    tdPerformance[3] = CreatePlayerTextDraw(playerid, 627.000000, 48.000000, "ping:~n~100");
    PlayerTextDrawLetterSize(playerid, tdPerformance[3], 0.130666, 0.737185);
    PlayerTextDrawAlignment(playerid, tdPerformance[3], 2);
    PlayerTextDrawColor(playerid, tdPerformance[3], -1);
    PlayerTextDrawSetShadow(playerid, tdPerformance[3], 0);
    PlayerTextDrawSetOutline(playerid, tdPerformance[3], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[3], 255);
    PlayerTextDrawFont(playerid, tdPerformance[3], 1);
    PlayerTextDrawSetProportional(playerid, tdPerformance[3], true);

    tdPerformance[4] = CreatePlayerTextDraw(playerid, 618.000000, 70.000000, "ld_beat:circle");
    PlayerTextDrawTextSize(playerid, tdPerformance[4], 18.000000, 19.000000);
    PlayerTextDrawAlignment(playerid, tdPerformance[4], 1);
    PlayerTextDrawColor(playerid, tdPerformance[4], 100);
    PlayerTextDrawSetShadow(playerid, tdPerformance[4], 0);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[4], 255);
    PlayerTextDrawFont(playerid, tdPerformance[4], 4);
    PlayerTextDrawSetProportional(playerid, tdPerformance[4], false);

    tdPerformance[5] = CreatePlayerTextDraw(playerid, 627.000000, 73.000000, "lost:~n~0.0");
    PlayerTextDrawLetterSize(playerid, tdPerformance[5], 0.130666, 0.737185);
    PlayerTextDrawAlignment(playerid, tdPerformance[5], 2);
    PlayerTextDrawColor(playerid, tdPerformance[5], -1);
    PlayerTextDrawSetShadow(playerid, tdPerformance[5], 0);
    PlayerTextDrawSetOutline(playerid, tdPerformance[5], 1);
    PlayerTextDrawBackgroundColor(playerid, tdPerformance[5], 255);
    PlayerTextDrawFont(playerid, tdPerformance[5], 1);
    PlayerTextDrawSetProportional(playerid, tdPerformance[5], true);

    tdAchievement[0] = CreatePlayerTextDraw(playerid, 67.999946, 307.807525, "MISTRZ SOLOWEK");
    PlayerTextDrawLetterSize(playerid, tdAchievement[0], 0.184000, 0.799407);
    PlayerTextDrawTextSize(playerid, tdAchievement[0], 0.000000, 582.000000);
    PlayerTextDrawAlignment(playerid, tdAchievement[0], 2);
    PlayerTextDrawColor(playerid, tdAchievement[0], -1);
    PlayerTextDrawSetShadow(playerid, tdAchievement[0], 0);
    PlayerTextDrawSetOutline(playerid, tdAchievement[0], 1);
    PlayerTextDrawBackgroundColor(playerid, tdAchievement[0], 255);
    PlayerTextDrawFont(playerid, tdAchievement[0], 1);
    PlayerTextDrawSetProportional(playerid, tdAchievement[0], true);

    tdAchievement[1] = CreatePlayerTextDraw(playerid, 68.333244, 316.104003, "Postep osiagniecia (poz. 1): 10/100");
    PlayerTextDrawLetterSize(playerid, tdAchievement[1], 0.139666, 0.691555);
    PlayerTextDrawTextSize(playerid, tdAchievement[1], 0.000000, 124.000000);
    PlayerTextDrawAlignment(playerid, tdAchievement[1], 2);
    PlayerTextDrawColor(playerid, tdAchievement[1], -1);
    PlayerTextDrawUseBox(playerid, tdAchievement[1], true);
    PlayerTextDrawBoxColor(playerid, tdAchievement[1], 120);
    PlayerTextDrawSetShadow(playerid, tdAchievement[1], 0);
    PlayerTextDrawSetOutline(playerid, tdAchievement[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdAchievement[1], 255);
    PlayerTextDrawFont(playerid, tdAchievement[1], 1);
    PlayerTextDrawSetProportional(playerid, tdAchievement[1], true);

    tdFuel[0] = CreatePlayerTextDraw(playerid, 268.000091, 356.755676, "box");
    PlayerTextDrawLetterSize(playerid, tdFuel[0], 0.000000, 5.899999);
    PlayerTextDrawTextSize(playerid, tdFuel[0], 377.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdFuel[0], 1);
    PlayerTextDrawColor(playerid, tdFuel[0], -1);
    PlayerTextDrawUseBox(playerid, tdFuel[0], true);
    PlayerTextDrawBoxColor(playerid, tdFuel[0], 100);
    PlayerTextDrawSetShadow(playerid, tdFuel[0], 0);
    PlayerTextDrawBackgroundColor(playerid, tdFuel[0], 255);
    PlayerTextDrawFont(playerid, tdFuel[0], 1);
    PlayerTextDrawSetProportional(playerid, tdFuel[0], true);

    tdFuel[1] = CreatePlayerTextDraw(playerid, 323.000030, 349.288757, "TANKOWANIE POJAZDU");
    PlayerTextDrawLetterSize(playerid, tdFuel[1], 0.199666, 0.981925);
    PlayerTextDrawTextSize(playerid, tdFuel[1], 0.000000, 828.000000);
    PlayerTextDrawAlignment(playerid, tdFuel[1], 2);
    PlayerTextDrawColor(playerid, tdFuel[1], -1);
    PlayerTextDrawSetShadow(playerid, tdFuel[1], 0);
    PlayerTextDrawSetOutline(playerid, tdFuel[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFuel[1], 255);
    PlayerTextDrawFont(playerid, tdFuel[1], 1);
    PlayerTextDrawSetProportional(playerid, tdFuel[1], true);

    tdFuel[2] = CreatePlayerTextDraw(playerid, 323.333404, 360.074096, "Typ paliwa: benzyna (100$)~n~zmiana typu paliwa: H");
    PlayerTextDrawLetterSize(playerid, tdFuel[2], 0.144666, 0.816000);
    PlayerTextDrawTextSize(playerid, tdFuel[2], 0.000000, 462.000000);
    PlayerTextDrawAlignment(playerid, tdFuel[2], 2);
    PlayerTextDrawColor(playerid, tdFuel[2], -1);
    PlayerTextDrawSetShadow(playerid, tdFuel[2], 0);
    PlayerTextDrawSetOutline(playerid, tdFuel[2], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFuel[2], 255);
    PlayerTextDrawFont(playerid, tdFuel[2], 1);
    PlayerTextDrawSetProportional(playerid, tdFuel[2], true);

    tdFuel[3] = CreatePlayerTextDraw(playerid, 323.333404, 394.088775, "Uzyj ~r~~h~H~w~~h~, aby zatankowac litr paliwa~n~~w~~h~Uzyj ~r~~h~Y~w~~h~, aby zatankowac do pelna");
    PlayerTextDrawLetterSize(playerid, tdFuel[3], 0.139333, 0.824296);
    PlayerTextDrawTextSize(playerid, tdFuel[3], 0.000000, 428.000000);
    PlayerTextDrawAlignment(playerid, tdFuel[3], 2);
    PlayerTextDrawColor(playerid, tdFuel[3], -1);
    PlayerTextDrawSetShadow(playerid, tdFuel[3], 0);
    PlayerTextDrawSetOutline(playerid, tdFuel[3], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFuel[3], 255);
    PlayerTextDrawFont(playerid, tdFuel[3], 1);
    PlayerTextDrawSetProportional(playerid, tdFuel[3], true);

    tdFuel[4] = CreatePlayerTextDraw(playerid, 322.666442, 387.451812, "10/100 L");
    PlayerTextDrawLetterSize(playerid, tdFuel[4], 0.133000, 0.587852);
    PlayerTextDrawTextSize(playerid, tdFuel[4], 0.000000, 753.000000);
    PlayerTextDrawAlignment(playerid, tdFuel[4], 2);
    PlayerTextDrawColor(playerid, tdFuel[4], -1);
    PlayerTextDrawSetShadow(playerid, tdFuel[4], 0);
    PlayerTextDrawSetOutline(playerid, tdFuel[4], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFuel[4], 255);
    PlayerTextDrawFont(playerid, tdFuel[4], 1);
    PlayerTextDrawSetProportional(playerid, tdFuel[4], true);

    tdFortune[0] = CreatePlayerTextDraw(playerid, 225.666732, 213.644485, "box");
    PlayerTextDrawLetterSize(playerid, tdFortune[0], 0.000000, 12.233332);
    PlayerTextDrawTextSize(playerid, tdFortune[0], 416.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[0], 1);
    PlayerTextDrawColor(playerid, tdFortune[0], -1);
    PlayerTextDrawUseBox(playerid, tdFortune[0], true);
    PlayerTextDrawBoxColor(playerid, tdFortune[0], 155);
    PlayerTextDrawSetShadow(playerid, tdFortune[0], 0);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[0], 255);
    PlayerTextDrawFont(playerid, tdFortune[0], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[0], true);

    tdFortune[1] = CreatePlayerTextDraw(playerid, 321.333312, 207.837066, "KOLO FORTUNY");
    PlayerTextDrawLetterSize(playerid, tdFortune[1], 0.194333, 0.961184);
    PlayerTextDrawTextSize(playerid, tdFortune[1], 0.000000, 418.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[1], 2);
    PlayerTextDrawColor(playerid, tdFortune[1], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[1], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[1], 255);
    PlayerTextDrawFont(playerid, tdFortune[1], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[1], true);

    tdFortune[2] = CreatePlayerTextDraw(playerid, 320.333343, 219.866668, "Nastepne losowanie mozesz wykonac za: bla bla bla~n~aby rozpoczac losowanie, kliknij chuj dupa cycki");
    PlayerTextDrawLetterSize(playerid, tdFortune[2], 0.142999, 0.778665);
    PlayerTextDrawTextSize(playerid, tdFortune[2], 0.000000, 233.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[2], 2);
    PlayerTextDrawColor(playerid, tdFortune[2], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[2], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[2], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[2], 255);
    PlayerTextDrawFont(playerid, tdFortune[2], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[2], true);

    tdFortune[3] = CreatePlayerTextDraw(playerid, 320.666595, 241.022186, "box");
    PlayerTextDrawLetterSize(playerid, tdFortune[3], 0.000000, 5.566680);
    PlayerTextDrawTextSize(playerid, tdFortune[3], 321.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[3], 1);
    PlayerTextDrawColor(playerid, tdFortune[3], -1);
    PlayerTextDrawUseBox(playerid, tdFortune[3], true);
    PlayerTextDrawBoxColor(playerid, tdFortune[3], 255);
    PlayerTextDrawSetShadow(playerid, tdFortune[3], 0);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[3], 255);
    PlayerTextDrawFont(playerid, tdFortune[3], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[3], true);

    tdFortune[4] = CreatePlayerTextDraw(playerid, 242.999938, 240.192565, "box");
    PlayerTextDrawLetterSize(playerid, tdFortune[4], 0.000000, 0.066667);
    PlayerTextDrawTextSize(playerid, tdFortune[4], 399.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[4], 1);
    PlayerTextDrawColor(playerid, tdFortune[4], -1);
    PlayerTextDrawUseBox(playerid, tdFortune[4], true);
    PlayerTextDrawBoxColor(playerid, tdFortune[4], 255);
    PlayerTextDrawSetShadow(playerid, tdFortune[4], 0);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[4], 255);
    PlayerTextDrawFont(playerid, tdFortune[4], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[4], true);

    tdFortune[5] = CreatePlayerTextDraw(playerid, 274.666717, 255.126052, "Darmowe konto VIP~n~Dodatkowe zetony do kasyna~n~Kilkukrotny wzrost poziomu~n~Darmowa dotacja~n~inne nagrody");
    PlayerTextDrawLetterSize(playerid, tdFortune[5], 0.143666, 0.703999);
    PlayerTextDrawTextSize(playerid, tdFortune[5], 0.000000, 1023.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[5], 2);
    PlayerTextDrawColor(playerid, tdFortune[5], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[5], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[5], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[5], 255);
    PlayerTextDrawFont(playerid, tdFortune[5], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[5], true);

    tdFortune[6] = CreatePlayerTextDraw(playerid, 274.000061, 244.340744, "NAGRODY:");
    PlayerTextDrawLetterSize(playerid, tdFortune[6], 0.166333, 0.786962);
    PlayerTextDrawAlignment(playerid, tdFortune[6], 2);
    PlayerTextDrawColor(playerid, tdFortune[6], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[6], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[6], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[6], 255);
    PlayerTextDrawFont(playerid, tdFortune[6], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[6], true);

    tdFortune[7] = CreatePlayerTextDraw(playerid, 366.000000, 244.000000, "TWOJA NAGRODA:");
    PlayerTextDrawLetterSize(playerid, tdFortune[7], 0.166333, 0.786962);
    PlayerTextDrawTextSize(playerid, tdFortune[7], 0.000000, 257.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[7], 2);
    PlayerTextDrawColor(playerid, tdFortune[7], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[7], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[7], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[7], 255);
    PlayerTextDrawFont(playerid, tdFortune[7], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[7], true);

    tdFortune[8] = CreatePlayerTextDraw(playerid, 367.000000, 255.000000, "Dziekujemy za udzial w losowaniu!~n~Zachecamy czesciej losowac!~n~~n~Twoja wylosowana nagroda to:~n~VIP na 3 dni!");
    PlayerTextDrawLetterSize(playerid, tdFortune[8], 0.143666, 0.703999);
    PlayerTextDrawTextSize(playerid, tdFortune[8], 0.000000, 1023.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[8], 2);
    PlayerTextDrawColor(playerid, tdFortune[8], -1);
    PlayerTextDrawSetShadow(playerid, tdFortune[8], 0);
    PlayerTextDrawSetOutline(playerid, tdFortune[8], 1);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[8], 255);
    PlayerTextDrawFont(playerid, tdFortune[8], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[8], true);

    tdFortune[9] = CreatePlayerTextDraw(playerid, 243.000000, 291.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdFortune[9], 0.000000, 0.033333);
    PlayerTextDrawTextSize(playerid, tdFortune[9], 399.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdFortune[9], 1);
    PlayerTextDrawColor(playerid, tdFortune[9], -1);
    PlayerTextDrawUseBox(playerid, tdFortune[9], true);
    PlayerTextDrawBoxColor(playerid, tdFortune[9], 255);
    PlayerTextDrawSetShadow(playerid, tdFortune[9], 0);
    PlayerTextDrawBackgroundColor(playerid, tdFortune[9], 255);
    PlayerTextDrawFont(playerid, tdFortune[9], 1);
    PlayerTextDrawSetProportional(playerid, tdFortune[9], true);

    tdCasinoSlot[0] = CreatePlayerTextDraw(playerid, 224.000000, 215.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[0], 0.000000, 13.500000);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[0], 412.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[0], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[0], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[0], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[0], 125);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[0], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[0], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[0], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[0], true);

    tdCasinoSlot[1] = CreatePlayerTextDraw(playerid, 317.000000, 220.000000, "AUTOMAT DO SLOTOW");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[1], 0.217333, 1.268146);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[1], 0.000000, 1128.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[1], 2);
    PlayerTextDrawColor(playerid, tdCasinoSlot[1], C_LY);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[1], 0);
    PlayerTextDrawSetOutline(playerid, tdCasinoSlot[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[1], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[1], 2);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[1], true);

    tdCasinoSlot[2] = CreatePlayerTextDraw(playerid, 306.000061, 245.014831, "hud:fist");
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[2], 23.000000, 28.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[2], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[2], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[2], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[2], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[2], 4);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[2], false);

    tdCasinoSlot[3] = CreatePlayerTextDraw(playerid, 266.000000, 245.000000, "hud:fist");
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[3], 23.000000, 28.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[3], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[3], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[3], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[3], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[3], 4);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[3], false);

    tdCasinoSlot[4] = CreatePlayerTextDraw(playerid, 346.000000, 245.000000, "hud:fist");
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[4], 23.000000, 28.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[4], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[4], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[4], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[4], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[4], 4);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[4], false);

    tdCasinoSlot[5] = CreatePlayerTextDraw(playerid, 258.000000, 242.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[5], 0.000000, 3.899993);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[5], 258.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[5], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[5], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[5], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[5], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[5], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[5], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[5], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[5], true);

    tdCasinoSlot[6] = CreatePlayerTextDraw(playerid, 297.000000, 242.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[6], 0.000000, 3.899993);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[6], 297.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[6], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[6], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[6], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[6], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[6], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[6], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[6], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[6], true);

    tdCasinoSlot[7] = CreatePlayerTextDraw(playerid, 338.000000, 242.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[7], 0.000000, 3.899993);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[7], 338.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[7], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[7], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[7], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[7], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[7], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[7], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[7], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[7], true);

    tdCasinoSlot[8] = CreatePlayerTextDraw(playerid, 378.000000, 242.000000, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[8], 0.000000, 3.899993);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[8], 378.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[8], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[8], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[8], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[8], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[8], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[8], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[8], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[8], true);

    tdCasinoSlot[9] = CreatePlayerTextDraw(playerid, 258.000000, 241.437072, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[9], 0.000000, -0.066665);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[9], 378.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[9], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[9], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[9], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[9], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[9], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[9], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[9], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[9], true);

    tdCasinoSlot[10] = CreatePlayerTextDraw(playerid, 258.000000, 279.185363, "box");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[10], 0.000000, -0.066665);
    PlayerTextDrawTextSize(playerid, tdCasinoSlot[10], 378.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[10], 1);
    PlayerTextDrawColor(playerid, tdCasinoSlot[10], -1);
    PlayerTextDrawUseBox(playerid, tdCasinoSlot[10], true);
    PlayerTextDrawBoxColor(playerid, tdCasinoSlot[10], 255);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[10], 0);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[10], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[10], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[10], true);

    tdCasinoSlot[11] = CreatePlayerTextDraw(playerid, 316.000000, 295.000000, "0/10");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[11], 0.130666, 0.612739);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[11], 2);
    PlayerTextDrawColor(playerid, tdCasinoSlot[11], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[11], 0);
    PlayerTextDrawSetOutline(playerid, tdCasinoSlot[11], 1);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[11], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[11], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[11], true);

    tdCasinoSlot[12] = CreatePlayerTextDraw(playerid, 263.000000, 322.000000, "UZUPELNIJ");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[12], 0.228332, 0.890666);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[12], 2);
    PlayerTextDrawColor(playerid, tdCasinoSlot[12], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[12], 0);
    PlayerTextDrawSetOutline(playerid, tdCasinoSlot[12], 1);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[12], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[12], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[12], true);

    tdCasinoSlot[13] = CreatePlayerTextDraw(playerid, 372.000000, 322.000000, "ZAKREC");
    PlayerTextDrawLetterSize(playerid, tdCasinoSlot[13], 0.228332, 0.890666);
    PlayerTextDrawAlignment(playerid, tdCasinoSlot[13], 2);
    PlayerTextDrawColor(playerid, tdCasinoSlot[13], -1);
    PlayerTextDrawSetShadow(playerid, tdCasinoSlot[13], 0);
    PlayerTextDrawSetOutline(playerid, tdCasinoSlot[13], 1);
    PlayerTextDrawBackgroundColor(playerid, tdCasinoSlot[13], 255);
    PlayerTextDrawFont(playerid, tdCasinoSlot[13], 1);
    PlayerTextDrawSetProportional(playerid, tdCasinoSlot[13], true);

    tdGameDraw = CreatePlayerTextDraw(playerid, 313.666595, 243.096328, "JAKIS TAM TEKST WY KURWY JEBANE~n~JEBAC MATKE ORENA JEBANA DZIWKE K");
    PlayerTextDrawLetterSize(playerid, tdGameDraw, 0.202999, 0.994370);
    PlayerTextDrawTextSize(playerid, tdGameDraw, 0.000000, 558.000000);
    PlayerTextDrawAlignment(playerid, tdGameDraw, 2);
    PlayerTextDrawColor(playerid, tdGameDraw, -1);
    PlayerTextDrawSetShadow(playerid, tdGameDraw, 0);
    PlayerTextDrawSetOutline(playerid, tdGameDraw, 1);
    PlayerTextDrawBackgroundColor(playerid, tdGameDraw, 255);
    PlayerTextDrawFont(playerid, tdGameDraw, 1);
    PlayerTextDrawSetProportional(playerid, tdGameDraw, true);

    tdGameList = CreatePlayerTextDraw(playerid, 2.000004, 185.022201, RefreshGameList());
    PlayerTextDrawLetterSize(playerid, tdGameList, 0.143333, 0.670814);
    PlayerTextDrawTextSize(playerid, tdGameList, 376.000000, 0.000000);
    PlayerTextDrawAlignment(playerid, tdGameList, 1);
    PlayerTextDrawColor(playerid, tdGameList, -1);
    PlayerTextDrawSetShadow(playerid, tdGameList, 0);
    PlayerTextDrawSetOutline(playerid, tdGameList, 1);
    PlayerTextDrawBackgroundColor(playerid, tdGameList, 255);
    PlayerTextDrawFont(playerid, tdGameList, 1);
    PlayerTextDrawSetProportional(playerid, tdGameList, true);

    tdInfo[0] = CreatePlayerTextDraw(playerid, 553.000000, 103.000000, "~n~~n~Posiadane zetony: 100~n~Dochod biznesu: 100.000$");
    PlayerTextDrawLetterSize(playerid, tdInfo[0], 0.137333, 0.703999);
    PlayerTextDrawTextSize(playerid, tdInfo[0], 0.000000, 108.000000);
    PlayerTextDrawAlignment(playerid, tdInfo[0], 2);
    PlayerTextDrawColor(playerid, tdInfo[0], -1);
    PlayerTextDrawUseBox(playerid, tdInfo[0], true);
    PlayerTextDrawBoxColor(playerid, tdInfo[0], 150);
    PlayerTextDrawSetShadow(playerid, tdInfo[0], 0);
    PlayerTextDrawSetOutline(playerid, tdInfo[0], 1);
    PlayerTextDrawBackgroundColor(playerid, tdInfo[0], 255);
    PlayerTextDrawFont(playerid, tdInfo[0], 1);
    PlayerTextDrawSetProportional(playerid, tdInfo[0], true);

    tdInfo[1] = CreatePlayerTextDraw(playerid, 552.000000, 104.000000, "KASYNO");
    PlayerTextDrawLetterSize(playerid, tdInfo[1], 0.168999, 0.932148);
    PlayerTextDrawTextSize(playerid, tdInfo[1], 0.000000, 513.000000);
    PlayerTextDrawAlignment(playerid, tdInfo[1], 2);
    PlayerTextDrawColor(playerid, tdInfo[1], -1);
    PlayerTextDrawSetShadow(playerid, tdInfo[1], 0);
    PlayerTextDrawSetOutline(playerid, tdInfo[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdInfo[1], 255);
    PlayerTextDrawFont(playerid, tdInfo[1], 2);
    PlayerTextDrawSetProportional(playerid, tdInfo[1], true);

    tdInfo[2] = CreatePlayerTextDraw(playerid, 612.000000, 98.000000, "30");
    PlayerTextDrawLetterSize(playerid, tdInfo[2], 0.137666, 0.703999);
    PlayerTextDrawAlignment(playerid, tdInfo[2], 3);
    PlayerTextDrawColor(playerid, tdInfo[2], -1);
    PlayerTextDrawSetShadow(playerid, tdInfo[2], 0);
    PlayerTextDrawSetOutline(playerid, tdInfo[2], 1);
    PlayerTextDrawBackgroundColor(playerid, tdInfo[2], 255);
    PlayerTextDrawFont(playerid, tdInfo[2], 1);
    PlayerTextDrawSetProportional(playerid, tdInfo[2], true);

    tdMoney[0] = CreatePlayerTextDraw(playerid, 576.333618, 55.185153, "$100.000.000");
    PlayerTextDrawLetterSize(playerid, tdMoney[0], 0.259999, 1.015110);
    PlayerTextDrawAlignment(playerid, tdMoney[0], 2);
    PlayerTextDrawColor(playerid, tdMoney[0], -1);
    PlayerTextDrawSetShadow(playerid, tdMoney[0], 0);
    PlayerTextDrawSetOutline(playerid, tdMoney[0], 1);
    PlayerTextDrawBackgroundColor(playerid, tdMoney[0], 255);
    PlayerTextDrawFont(playerid, tdMoney[0], 3);
    PlayerTextDrawSetProportional(playerid, tdMoney[0], true);

    tdMoney[1] = CreatePlayerTextDraw(playerid, 609.999572, 64.311149, "~r~~h~-~w~~h~$100.000");
    PlayerTextDrawLetterSize(playerid, tdMoney[1], 0.119666, 0.625185);
    PlayerTextDrawAlignment(playerid, tdMoney[1], 3);
    PlayerTextDrawColor(playerid, tdMoney[1], -1);
    PlayerTextDrawSetShadow(playerid, tdMoney[1], 0);
    PlayerTextDrawSetOutline(playerid, tdMoney[1], 1);
    PlayerTextDrawBackgroundColor(playerid, tdMoney[1], 255);
    PlayerTextDrawFont(playerid, tdMoney[1], 1);
    PlayerTextDrawSetProportional(playerid, tdMoney[1], true);
}