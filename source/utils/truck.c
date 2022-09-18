#include YSI_Coding\y_hooks

enum e_towaryData {
	towar_name[30],
	towar_money[2],
	bool:towar_premium
};

new 
	Float:truckData[][] = {
		{-2108.789, -2419.701, 30.625},
		{2786.773, -2455.987, 13.633},
		{-2725.229, -312.881, 7.039},
		{-513.408, -188.816, 78.161},
		{1124.311, 1455.541, 5.820},
		{-1931.995, 2383.598, 49.492},
		{1915.480, 959.368, 10.820},
		{-296.190, 1772.875, 42.687},
		{924.920, -1121.517, 24.014},
		{1248.877, -2037.259, 59.742},
		{428.971, -1804.682, 5.546},
		{1556.485, -1790.564, 13.546},
		{1336.717, 286.405, 19.561},
		{681.590, -441.307, 16.335},
		{-1707.225, 1336.220, 7.174}
	},

	towaryData[][e_towaryData] = {
		{"Elektronika", 	{10000, 30000}, 	false},
		{"¯ywnoœæ", 		{20000, 40000}, 	false},
		{"Zabawki", 		{10000, 30000}, 	false},
		{"Ksi¹¿ki",			{10000, 20000}, 	false},
		{"Bi¿uteria", 		{10000, 15000}, 	false},

		{"Odzie¿", 			{15000, 30000}, 	true},
		{"Budulec", 		{20000, 30000}, 	true},
		{"Uzbrojenie", 		{30000, 50000}, 	true},
		{"Narkotyki", 		{50000, 80000}, 	true},
		{"Medykamenty", 	{70000, 100000}, 	true}
	},

	truckCP,
	truckDay,

	bool:pTruck[MAX_PLAYERS],
	pTruckTowar[MAX_PLAYERS],
	pTruckReward[MAX_PLAYERS],
	pTruckData[MAX_PLAYERS][sizeof(truckData)],
	Float:pTruckDist[MAX_PLAYERS],

	pTruckVehicle[MAX_PLAYERS],
	Text3D:pTruckLabel[MAX_PLAYERS],
	pTruckTrailer[MAX_PLAYERS],
	pTruckIcon[MAX_PLAYERS],
	pTruckCheckpoint[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid)
{
	pTruckVehicle[playerid] = pTruckTrailer[playerid] = pTruckIcon[playerid] = pTruckCheckpoint[playerid] = -1;
	pTruckLabel[playerid] = Text3D:-1;

	pTruck[playerid] = false;
	pTruckTowar[playerid] = -1;
	pTruckReward[playerid] = 0;
	pTruckDist[playerid] = 0.0;
	for(new i = 0; i < sizeof(truckData); i++) pTruckData[playerid][i] = 0;
	return continue(playerid);
}

stock Init_Truck()
{
	truckCP = CreateDynamicCP(1727.3540, 2309.5391, 10.8203, 3.0, .worldid=0, .interiorid=0);
	CreateDynamic3DTextLabel(""BLUE"BAZA SPEDYCYJNA\n"LY"WjedŸ w "WHITE"punkt kontrolny"LY", by rozpocz¹æ spedycjê!", -1, 1727.3540, 2309.5391, 10.8203 + 0.25, 30.0, .worldid=0, .interiorid=0);
	
	truckDay = random(sizeof(towaryData));
	return printf(" >>> [TRUCK]: Wczytanych tras spedycyjnych: %d (%d towarów)", sizeof(truckData), sizeof(towaryData));
}

stock ShowTruckMenu(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Lokalizacja\t"GREEN"Odleg³oœæ\n";

    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    for(new i = 0; i < sizeof(truckData); i++) format(s, sizeof(s), "%s"WHITE"%s (%s)\t"GREEN"%.1f km\n", s, getZoneName(unpackXYZ(truckData[i])), getCityName(unpackXYZ(truckData[i])), (GetDistanceBetweenPoints3D(x, y, z, unpackXYZ(truckData[i])) / 1000));
    return showDialog(playerid, DIALOG_TRUCK_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""GREEN"Baza spedycyjna", s, "Wybierz", "Zamknij"), 1;
}

hook OnPlayerDeath(playerid, killerid, reason) {
    if(pTruck[playerid]) Truck_Stop(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    if(pTruck[playerid]) Truck_Stop(playerid);
    return 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    if(checkpointid == truckCP && IsPlayerInAnyVehicle(playerid) && !pTruck[playerid]) {
        new modelid = GetVehicleModel(vehicleid);
        if(modelid != 515) return dialogBox(playerid, C_RED, "Aby wykonaæ spedycjê w tej {c}bazie{/c}, musisz byæ w {c}%s{/c}.", VehicleNames[515 - 400]);
        if(!IsTrailerAttachedToVehicle(vehicleid)) return dialogBox(playerid, C_RED, "Aby wykonaæ spedycjê w tej {c}bazie{/c}, musisz mieæ podpiêt¹ {c}naczepê{/c} z towarem!");
        return ShowTruckMenu(playerid);
    }

    if(pTruck[playerid] && IsPlayerInAnyVehicle(playerid) && checkpointid == pTruckCheckpoint[playerid]) {        
		new time = math_random(5, 10);
		if(!IsTrailerAttachedToVehicle(vehicleid)) return dialogBox(playerid, C_RED, "Aby ukoñczyæ spedycjê, musisz mieæ {c}podpiêt¹{/c} naczepê z towarem!");
		if(GetVehicleModel(vehicleid) != 515) return dialogBox(playerid, C_RED, "Aby ukoñczyæ spedycjê, musisz byæ w {c}%s{/c}.", VehicleNames[515 - 400]);

        s[0] = EOS;
        format(s, sizeof(s), ""BLUE2"Rozpoczêto roz³adowywanie towaru!\n"BLUE2"Roz³adunek zajmie "WHITE"%d "BLUE2"sekund!", time);
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Roz³adunek towaru", s, "OK", #);

        SetTimerEx("Truck_Rozladuj", 1000 * time, false, "d", playerid);
        return TogglePlayerControllable(playerid, false), 1;
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid) {
        case DIALOG_TRUCK_PANEL: {
            if(!response) return 1;
            SetPVarInt(playerid, "player_truck_trasa", listitem);

            s[0] = EOS;
            s = ""WHITE"Typ towaru\t"BLUE2"Przedzia³ gotówki\n";
            for(new i = 0; i < sizeof(towaryData); i++) format(s, sizeof(s), "%s{%06x}%s\t"BLUE2"%s-%s$\n", s, (IsPremium(playerid)) ? (GREEN) : (RED), towaryData[i][towar_name], strdot(towaryData[i][towar_money][0]), strdot(towaryData[i][towar_money][1]));
            return showDialog(playerid, DIALOG_TRUCK_TOWAR, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Wybierz typ towaru", s, "Wybierz", "Cofnij"), 1;
        }

        case DIALOG_TRUCK_TOWAR: {
            if(!response) return ShowTruckMenu(playerid);
            if(towaryData[listitem][towar_premium] && !IsPremium(playerid)) return dialogBox(playerid, C_RED, "Musisz posiadaæ {c}Konto Premium{/c}, by móc wybraæ ten towar!");
            pTruckTowar[playerid] = listitem;

            new destination = GetPVarInt(playerid, "player_truck_trasa"), Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
            new Float:km = GetDistanceBetweenPoints3D(x, y, z, unpackXYZ(truckData[destination]));
            SetPVarFloat(playerid, "player_truck_distance", km / 1000);

            validReward:
            pTruckReward[playerid] = floatround((math_random(towaryData[listitem][towar_money][0], towaryData[listitem][towar_money][1])) + (floatround(km, floatround_round) / 100), floatround_round);
            if(pTruckReward[playerid] < towaryData[listitem][towar_money][0] || pTruckReward[playerid] > towaryData[listitem][towar_money][1]) goto validReward;

            pTruck[playerid] = true;
            pTruckVehicle[playerid] = GetPlayerVehicleID(playerid);
            pTruckTrailer[playerid] = GetVehicleTrailer(pTruckVehicle[playerid]);
            pTruckLabel[playerid] = CreateDynamic3DTextLabel(""GREEN"£adowanie towaru"YELLOW"...", -1, -2000.0, -3000.0, 0.0, 30.0, .attachedvehicle=pTruckVehicle[playerid], .interiorid=0, .worldid=0);
            pTruckCheckpoint[playerid] = CreateDynamicCP(unpackXYZ(truckData[destination]), 3.0, .worldid=0, .interiorid=0, .priority=1, .streamdistance=99999.0, .playerid=playerid);
            pTruckIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(truckData[destination]), 51, -1, .worldid=0, .interiorid=0, .playerid=playerid, .style=MAPICON_GLOBAL, .streamdistance=99999.0);

            new time = math_random(5, 10);
            SavePlayerPosition(playerid);
            TogglePlayerControllable(playerid, false);
            DisableRemoteVehicleCollisions(playerid, true);
            SetTimerEx("Truck_Freeze", 1000 * time, false, "d", playerid);

			playerTextDrawSetString(playerid, tdInfo[1], "~p~Spedycja");
			playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Dystans do przebycia: ~r~~h~%.1f km~n~~w~~h~Nagroda: ~y~~h~%s$", km, strdot(pTruckReward[playerid]));
			for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdInfo[i]);

            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Twoim zadaniem jest dostarczenie towaru: "LY"%s"WHITE".\n"WHITE"Otrzymasz za nie: "RED"%s$"WHITE"!\n"WHITE"Odczekaj teraz "YELLOW"%d sekund"WHITE", bo trwa ³adowanie towaru!", towaryData[listitem][towar_name], strdot(pTruckReward[playerid]), time);
            return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"£adowanie towaru", s, "OK", #), 1;
        }

		case DIALOG_TRUCK_NEWBIE: {
			if(!response) return 1;

			switch(listitem) {
				case 0..2: callcmd::truck(playerid);
				
				case 3: {
					s[0] = EOS;
					catstr(s, C_CORAL, -1, "Spedycja jest {c}jedn¹ z lepszych{/c} metod zarobku na serwerze.\n"CORAL"Poni¿ej przedstawione zosta³o dzia³anie ca³ego systemu.\n\n");
					
					catstr(s, C_GREEN, -1, "Startowanie spedycji:\n");
					catstr(s, C_WHITE, -1, "Aby wystartowaæ spedycjê, musisz "RED"udaæ siê "WHITE"do bazy spedycyjnej "LY"(/tr)"WHITE".\n");
					catstr(s, C_WHITE, -1, "Nastêpnie bêdziesz musia³ wybraæ "LGREEN"trasê"WHITE" oraz "LGREEN"towar"WHITE", który chcesz przewieŸæ.\n\n");
					
					catstr(s, C_GREEN, -1, "Przebieg spedycji:\n");
					catstr(s, C_WHITE, -1, "Na pocz¹tku bêdziesz musia³ odczekaæ "YELLOW"okreœlon¹ "WHITE"iloœæ sekund - towar bêdzie za³adowywany.\n");
					catstr(s, C_WHITE, -1, "Po za³adowaniu, na radarze pojawi siê "BROWN"ciê¿arówka"WHITE", jedŸ do niej, by roz³adowaæ towar.\n");
					catstr(s, C_WHITE, -1, "Po roz³adowaniu otrzymasz "VIOLET"wylosowan¹ nagrodê "WHITE"oraz, w zale¿noœci od "LY"wybranego "WHITE"towaru - dodatkow¹ nagrodê!");
					showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""PINK"Spedycja", s, "OK", #);
				}
				case 4: EnterTeleport(playerid, "tr");

				case 5: {
					inline onFetchTop() {
						s[0] = EOS;
						s = ""BLACK"ID\t"WHITE"Nick\t"CORAL"Wynik\n";

						new id, nick[25], result;
						for(new i = 0; i < cache_num_rows(); i++) {
							cache_get_value_int(i, 0, id);
							cache_get_value(i, 1, nick, 25);
							cache_get_value_int(i, 2, result);

							format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s\n", s, i + 1, nick, strdot(result));
						}

						inline onTopDialog(pp, dd, rr, ll, string:ii[]) {
							#pragma unused pp, dd, rr, ll, ii
							return callcmd::truck(playerid); }
						return Dialog_ShowCallback(playerid, using inline onTopDialog, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Podium najlepszych spedytorów", s, "OK", #), 1;
					}

					dbstr[0] = EOS;
                	format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_TRUCK + 1);
					m_pquery_inline(dbconn, using inline onFetchTop, dbstr);
				}
			}
			return 1;
		}
	}
	return 0;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(pTruck[playerid] && pTruckVehicle[playerid] == vehicleid && !ispassenger) {
        SetPVarInt(playerid, "player_truck_timer", 0);
        showGameDraw(playerid, "_", 1);
    }
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(pTruck[playerid] && pTruckVehicle[playerid] == vehicleid) {
        SetPVarInt(playerid, "player_truck_timer", 10);
        showGameDraw(playerid, "~y~MASZ ~r~10 ~y~SEKUND, BY WROCIC DO POJAZDU!", 10);
    } 
    return 1;
}

stock CheckTruckTimer(playerid)
{
    if(pTruck[playerid] && pTruckTowar[playerid] != -1) {
        if(GetPVarInt(playerid, "player_truck_timer") > 0) {
            SetPVarInt(playerid, "player_truck_timer", GetPVarInt(playerid, "player_truck_timer") - 1);
            if(GetPVarInt(playerid, "player_truck_timer") < 1) {
				SetPVarInt(playerid, "player_truck_timer", 0);

                Truck_Stop(playerid);
                msg(playerid, C_CHAT, "Nie wróci³eœ(aœ) na czas do {c}pojazdu{/c} - spedycja przerwana!");
            }
            showGameDraw(playerid, form("~y~MASZ ~r~%d ~y~SEKUND, BY WROCIC DO POJAZDU!", GetPVarInt(playerid, "player_truck_timer")), 10);
        }

		new trailer = IsTrailerAttachedToVehicle(pTruckVehicle[playerid]);
		if(!trailer) { if(!GetPVarInt(playerid, "player_truck_trailer")) SetPVarInt(playerid, "player_truck_trailer", 15); } 
		else {
			if(GetPVarInt(playerid, "player_truck_trailer") > 0) SetPVarInt(playerid, "player_truck_trailer", 0);
			SetPVarInt(playerid, "player_truck_trailer", 0);
		}

		if(GetPVarInt(playerid, "player_truck_trailer") > 0) {
			SetPVarInt(playerid, "player_truck_trailer", GetPVarInt(playerid, "player_truck_trailer") - 1);
			if(GetPVarInt(playerid, "player_truck_trailer") < 1) {
				SetPVarInt(playerid, "player_truck_trailer", 0);
				showGameDraw(playerid, "_", 1);

				Truck_Stop(playerid);
				msg(playerid, C_CHAT, "Nie podczepi³eœ(aœ) {c}naczepy{/c} na czas - spedycja przerwana!");
			}
			showGameDraw(playerid, form("~y~MASZ ~r~%d ~y~SEKUND, BY PODPIAC NACZEPE!", GetPVarInt(playerid, "player_truck_trailer")), 15);
		}
    }
    return 1;
}

function Truck_Freeze(playerid)
{
    new towarid = pTruckTowar[playerid], destination = GetPVarInt(playerid, "player_truck_trasa");

    s[0] = EOS;
    format(s, sizeof(s), ""LBLUE"Towar "LORANGE"%s "LBLUE"zosta³ za³adowany!\n"LBLUE"Twoim zadaniem jest udanie siê do: "LORANGE"%s (%s)"LBLUE".", towaryData[towarid][towar_name], getZoneName(unpackXYZ(truckData[destination])), getCityName(unpackXYZ(truckData[destination])));
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GREEN"Towar za³adowany", s, "OK", #);

    TogglePlayerControllable(playerid, true);
    DisableRemoteVehicleCollisions(playerid, true);

    new tstr[128]; format(tstr, sizeof(tstr), ""GREEN"%s "LY"(%s, %s)\n"CORAL"%s$", towaryData[towarid][towar_name], getZoneName(unpackXYZ(truckData[destination])), getCityName(unpackXYZ(truckData[destination])), strdot(pTruckReward[playerid]));
    return UpdateDynamic3DTextLabelText(pTruckLabel[playerid], -1, tstr), 1;
}

function Truck_Rozladuj(playerid)
{
    new towarid = pTruckTowar[playerid], money = (pTruckReward[playerid] + ((truckDay == towarid) ? (10000) : (0)));
    giveMoney(playerid, money);
    achievement(playerid, ACH_TRUCK);

    s[0] = EOS;
    format(s, sizeof(s), ""LIME"Dostarczy³eœ(aœ) "LY"%s "LIME"towar do klienta!\n"LIME"Dowo¿ony towar: "LY"%s"LIME".\n"LIME"Nagroda: "LY"%s$"LIME".", strdot(GetAchievement(playerid, ACH_TRUCK)), towaryData[towarid][towar_name], strdot(pTruckReward[playerid]));
    if(truckDay == towarid) format(s, sizeof(s), "%s"LIME"Otrzymujesz dodatkowo "LY"10.000$"LIME" za dowiezienie towaru dnia!", s);
	showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""CORAL"Towar dostarczony", s, "OK", #);

    pTruckDist[playerid] += GetPVarFloat(playerid, "player_truck_distance");
    msgAll(C_NICK, "{c}%s(%d){/c} ukoñczy³(a) {c}%s{/c} spedycjê! "LY"(%.1f km, %s$)", getNick(playerid), playerid, strdot(GetAchievement(playerid, ACH_TRUCK)), GetPVarFloat(playerid, "player_truck_distance"), strdot(money));
    ShowMissionDraw(playerid, "~y~TRUCK COMPLETED", "Dostarczono towar do klienta!", money);
    return Truck_Stop(playerid), 1;
}

stock Truck_Stop(playerid)
{
    if(IsValidVehicle(pTruckVehicle[playerid])) DestroyVehicle(pTruckVehicle[playerid]);
    if(IsValidDynamic3DTextLabel(pTruckLabel[playerid])) DestroyDynamic3DTextLabel(pTruckLabel[playerid]);
    if(IsValidDynamicCP(pTruckCheckpoint[playerid])) DestroyDynamicCP(pTruckCheckpoint[playerid]);
    if(IsValidDynamicMapIcon(pTruckIcon[playerid])) DestroyDynamicMapIcon(pTruckIcon[playerid]);
    if(pTruckTrailer[playerid] != -1) SetVehicleToRespawn(pTruckTrailer[playerid]);

	pTruck[playerid] = false;
	pTruckTowar[playerid] = -1;
	pTruckReward[playerid] = 0;

    RemovePlayerFromVehicle(playerid);
    TogglePlayerControllable(playerid, false);
    DisableRemoteVehicleCollisions(playerid, false);

	for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
    return LoadPlayerPosition(playerid), 1;
}

CMD:truck(playerid)
{
	s[0] = EOS;
	catstr(s, C_WHITE, -1, "Premiowany towar dnia:\t"LORANGE"%s\n", towaryData[truckDay][towar_name]);
	catstr(s, C_WHITE, -1, "Przejechany dystans:\t"CORAL"%.1f km\n", pTruckDist[playerid]);
	strcat(s, " \n");
	strcat(s, ""WHITE""ARROW"  "BLUE2"Informacje o systemie spedycji\n");
	strcat(s, ""WHITE""ARROW"  "BLUE2"Teleport do bazy spedycyjnej\n");
	strcat(s, ""WHITE""ARROW"  "BLUE2"Podium najlepszych spedytorów\n");
	return showDialog(playerid, DIALOG_TRUCK_NEWBIE, DIALOG_STYLE_TABLIST, ""YELLOW"Spedycja", s, "Wybierz", "Zamknij"), 1;
}
