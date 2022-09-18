new const drawMessages[][] = {
    {"Wersja gamemode: ~r~~h~"#VERSION""},
    {"Zakup swoj prywatny pojazd! ~r~~h~/privcar"},
    {"Konfiguruj swoj HUD! ~r~~h~/hud"},
    {"Wykonuj zadania dzienne! ~r~~h~/quest"},
    {"Na calej mapie znajduja sie ~b~~h~~h~dyskietki~w~~h~!"},
    {"Na calej mapie znajduje sie ~r~~h~TYLKO ~w~~h~10 ~g~~h~~h~figurek~w~~h~!"},
    {"Potrzebujesz pomocy? Sprawdz: ~p~/help"},
    {"Dostepne osiagniecia pod: ~y~~h~/osiagniecia"},
    {"Ulepsz swoja postac o nowe dodatki! ~p~~h~/ulepszenia"},
    {"Na tym serwerze mozesz ~y~~h~wykonywac ~w~~h~prace! ~b~~h~~h~~h~/praca"},
    {"Autor serwera: ~r~~h~"#AUTHOR""},
    {"Nasz gamemode posiada ~r~~h~"#LINES" ~w~~h~linijek kodu!"},
    {"Zarzadzanie kontem pod: ~y~/konto"},
    {"Zaloz swoj zespol! ~r~~h~/teams"},
    {"Przejmuj tereny zespolowe w Los Santos! ~r~~h~/teams"},
    {"Wygrywaj sparingi ~y~~h~~h~rankingowe/nierankingowe ~w~~h~z zespolem! ~r~~h~/teams"},
    {"Wykonuj osiagniecia zespolowe! ~p~/teams"},
    {"Grajac duele, wbijasz range! ~r~~h~/duel"},
    {"Uwielbiasz nasz serwer? Okaz to: ~p~~h~/naj~w~~h~! :)"},
    {"Bron na wylacznosc kupisz w ~p~sklepach z bronia~w~~h~!"},
    {"Pamietaj, ze mozesz zglaszac innych! ~p~/report"},
    {"Chcesz zmienic swoj skin? ~r~~h~/skin"},
    {"Chcesz zmienic swoj kolor? ~p~~h~/kolor"},
    {"Chcesz stworzyc pojazd? ~g~~h~~h~/cars"},
    {"Lista teleportow pod: ~r~~h~/teles"},
    {"Head Administracja moze wlaczyc ~r~~h~napady na bank~w~~h~!"}
};

new task_timer[6];
timer SecondTimer[1000]()
{
    foreach(new i : Player) {
        if(pVehID[i] != -1) PrivCarTimer(i);

        UpdatePlayerLabel(i);
        CheckHeistVault(i);
        CheckMysliwyZone(i);
        CheckGrzybiarzZone(i);
        FishTimer(i);
        MineCheckTimer(i);
        CheckTruckTimer(i);
        CheckBaseTimer(i);

        if(playerData[i][player_countdown] > 0) {
            playerData[i][player_countdown] --;

            new cstr[50];
            if(playerData[i][player_countdown] > 10) format(cstr, sizeof(cstr), "~y~%d", playerData[i][player_countdown]);
            else if(playerData[i][player_countdown] < 11 && playerData[i][player_countdown] > 3) format(cstr, sizeof(cstr), "~g~~h~~h~%d", playerData[i][player_countdown]);
            else if(playerData[i][player_countdown] < 4 && playerData[i][player_countdown] > 0) format(cstr, sizeof(cstr), "~r~~h~%d", playerData[i][player_countdown]);
            else format(cstr, sizeof(cstr), "~y~~h~START~r~!!");

            GameTextForPlayer(i, cstr, 1000, 3);
            playSound(i, (playerData[i][player_countdown] > 0) ? (1056) : (1057));
        }

        if(playerData[i][player_floodTimer][0] > 0) {
            playerData[i][player_floodTimer][0] --;
            if(playerData[i][player_floodTimer][0] < 1) 
                playerData[i][player_floodTimer][1] = playerData[i][player_floodCounter] = 0;
        }

        if(playerData[i][player_jailtime] > 0) {
            playerData[i][player_jailtime] --;
            if(playerData[i][player_jailtime] < 1) {
                playSound(i, 1150);
                playerData[i][player_jailtime] = 0;
                LoadPlayerPosition(i);
            }
            else showGameDraw(i, form("~y~JESTES WE WIEZIENIU!~n~~y~WYJDZIESZ NA WOLNOSC ZA: ~r~%02d:%02d", floatround(((playerData[i][player_jailtime]) / 60) % 60), playerData[i][player_jailtime] % 60), 2);
        }

        if(playerData[i][player_loginTime] > 0) {
            playerData[i][player_loginTime] --;
            if(playerData[i][player_loginTime] < 1 && !playerData[i][player_logged]) return KickPlayer(i, -1, "Czas na logowanie min¹³"), 1; 
            PlayerTextDrawSetString(i, tdLoginTime, form("~w~~h~%d sek", playerData[i][player_loginTime]));
        }

        if(playerData[i][player_mutetime] > 0) {
            playerData[i][player_mutetime] --;
            if(playerData[i][player_mutetime] < 1) {
                playSound(i, 1150);
                msg(i, C_NICK, "Wyciszenie minê³o! Mo¿esz ju¿ {c}pisaæ{/c} na czacie.");
                playerData[i][player_mutetime] = 0;
            }
        }

        if(playerData[i][player_antySK] > 0) {
            playerData[i][player_antySK] --;
            if(playerData[i][player_antySK] < 1) {
                playerData[i][player_god] = false;
                playerData[i][player_antySK] = 0;

                if(!NieMozeTeraz(i)) {
                    SetPlayerHealth(i, 100.0);
                    SetPlayerArmour(i, (pUlepszenia[i][ULEPSZENIE_KAMIZELKA]) ? (100.0) : (0.0));

                    showGameDraw(i, "~n~~n~~n~~n~~n~~n~~n~~n~~y~ANTY ~y~~h~SPAWN-KILL:~n~~r~~h~NIEAKTYWNY!", 3);
                    for(new x = 0; x < 7; x++) giveWeapon(i, pWeapons[i][x], (x == 7) ? (10) : (1000));
                }
            }
        }

        if(playerData[i][player_logged]) {
            if(!pAFK[i]) playerData[i][player_online] ++;
            CheckPlayerAFK(i);

            if(IsPlayerSpawned(i) && pGang[i] != -1) UpdateGangBar(i, pGang[i]);
            if(playerData[i][player_pickupTimer] > 0) playerData[i][player_pickupTimer] --;
            if(playerData[i][player_teleportRequest] != -1) {
                playerData[i][player_teleportTimer] --;
                if(playerData[i][player_teleportTimer] < 1) playerData[i][player_teleportRequest] = -1;
            }
            
            if(IsPlayerInRangeOfPoint(i, 3.0, 2164.00, 1073.19, 12.45)) {
                playerData[i][player_attempts] ++;
                if(playerData[i][player_attempts] >= 5) {
                    msg(i, C_BLUE2, "Kliknij przycisk {c}H{/c}, by obsikaæ ten pomnik! Otrzymasz wtedy {c}ciekawy bonus{/c}!");
                    playerData[i][player_attempts] = 0;
                }
            }

            new hours = floatround(((playerData[i][player_online]) / 3600));
            new minutes = floatround(((playerData[i][player_online]) / 60) % 60);

            playerTextDrawSetString(i, tdPlayer[0], "~w~~h~%s (~y~%d~w~~h~)", getNick(i), i);
            playerTextDrawSetString(i, tdPlayer[1], "Poziom:~n~~w~~h~%s", strdot(playerData[i][player_level]));
            playerTextDrawSetString(i, tdPlayer[2], "Zabojstwa:~n~~w~~h~%s", strdot(GetAchievement(i, ACH_KILLS)));
            playerTextDrawSetString(i, tdPlayer[3], "Smierci:~n~~w~~h~%s", strdot(GetAchievement(i, ACH_DEATHS)));
            playerTextDrawSetString(i, tdPlayer[4], "Czas gry:~n~~w~~h~%02dh %02dmin", hours, minutes);

            playerTextDrawSetString(i, tdPerformance[1], "~g~~h~~h~fps:~n~~w~~h~%d", playerData[i][player_fps]);
            playerTextDrawSetString(i, tdPerformance[3], "~p~ping:~n~~w~~h~%d", GetPlayerPing(i));
            playerTextDrawSetString(i, tdPerformance[5], "~r~~h~lost:~n~~w~~h~%.1f", NetStats_PacketLossPercent(i));

            new Float:x, Float:y, Float:z; GetPlayerPos(i, x, y, z);
            playerTextDrawSetString(i, tdPosition, "(twoj kolor) ~w~~h~x: %.2f, y: %.2f, z: %.2f", x, y, z);

            SetPlayerProgressBarValue(i, LevelBar[i], float(playerData[i][player_score]));
            SetPlayerProgressBarMaxValue(i, LevelBar[i], float(playerData[i][player_score_needed]));

            if(hours > 0 && hours == playerData[i][player_hourGame]) {
                playerData[i][player_hourGame] ++;
                achievement(i, ACH_LOYAL);

                new money = math_random(30000, 50000), pstr[256];
                catstr(pstr, C_ZAJEBISTY, -1, "Grasz ju¿ {c}%d godzin{/c} na naszym serwerze!\n"ZAJEBISTY"Nagroda: {c}%s${/c}.", playerData[i][player_hourGame], strdot(money));
                showDialog(i, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Premia za godzinê", pstr, "OK", #);
            }
        }
    }

    if(task_timer[0] ++ >= 5) {
        textDrawSetString(tdMessage, "~w~~h~%s", drawMessages[random(sizeof(drawMessages))]);
        task_timer[0] = 0;
    }

    if(task_timer[3] ++ >= 3) {
        MoveWP();
        task_timer[3] = 0;
    }

	textDrawSetString(tdOnline[0], "~g~~h~~h~%d", Iter_Count(Player));
	textDrawSetString(tdOnline[1], "~y~%d~w~~h~, ~r~~h~%d", Iter_Count(Premium), Iter_Count(Permission));
    return 1;
}

new bool:zresetowane = false;
timer MinuteTimer[60000]()
{
    new year, month, day, hour, minute;
    getdate(year, month, day), gettime(hour, minute);
    TextDrawSetString(tdDate, form("~w~~h~%02d~g~~h~:~w~~h~%02d~n~~w~~h~%02d~g~~h~.~w~~h~%02d~g~~h~.~w~~h~%d", hour, minute, day, month, year));

    if(!strcmp(getDayName(getDay(day, month, year)), "Poniedzialek", false) && (hour == 23 && minute == 59) || (hour == 0 && minute == 0) || (hour == 0 && minute == 1) && !zresetowane) {
        m_pquery_format("UDPATE es_accounts SET admin_online=0, admin_points=0, event_points=0, event_count=0");
        foreach(new i : Permission) playerData[i][player_adminTime] = playerData[i][player_adminPoints] = playerData[i][player_eventCount] = playerData[i][player_eventPoints] = 0;
        zresetowane = true;
    }
    if((hour == 3 && minute == 00) || (hour == 2 && minute == 59)) { if(saveAll()) SendRconCommand("gmx"); }

    if(task_timer[1] ++ >= 3) {
        msgAll(-1, " ");

        switch(random(9)) {
            case 0: {
                msgAll(C_BLUE2, "{c}(i)*{/c}  Chcesz zostaæ {c}najlepszym gangsterem{/c} w okolicy?");
                msgAll(C_BLUE2, "{c}(i)*{/c}  Za³ó¿ ze swoimi znajomymi {c}/gang{/c} i podbijcie ca³y serwer!"); }
            case 1: {
                msgAll(C_NICK, "{c}(i)*{/c}  Chcesz {c}szybko i ³atwo{/c} siê dorobiæ maj¹tku?");
                msgAll(C_NICK, "{c}(i)*{/c}  Udaj siê do {c}/praca{/c} i zostañ najbogatszym graczem tutaj!"); }
            case 2: {
                msgAll(C_CORAL, "{c}(i)*{/c}  Niedaleko {c}/LV{/c} znajduj¹ siê pomniki {c}elitarnych{/c} cz³onków serwera!");
                msgAll(C_CORAL, "{c}(i)*{/c}  Jeden z nich posiada {c}ciekaw¹{/c} funkcjê, sprawdŸ sam jak¹!"); }
            case 3: {
                msgAll(C_MINT, "{c}(i)*{/c}  Na tym serwerze mo¿esz mieæ {c}3{/c} prywatne pojazdy, a z ulepszeniem: {c}5{/c}.");
                msgAll(C_MINT, "{c}(i)*{/c}  Zaciekawiony? SprawdŸ ju¿ teraz: {c}/privcar{/c}."); }
            case 4: {
                msgAll(C_ZAJEBISTY, "{c}(i)*{/c}  Szukaj na mapie {c}dyskietek{/c}, z których zgarniesz {c}exp{/c} do poziomu!");
                msgAll(C_ZAJEBISTY, "{c}(i)*{/c}  Aktualnie na mapie jest {c}%d{/c} %s.", diskCount, ConvertTimeFormat(diskCount, "dyskietek", "dyskietka", "dyskietki", "dyskietek")); }
            case 5: {
                msgAll(C_GREEN, "{c}(i)*{/c}  Szukaj na mapie {c}figurek{/c}, z których zgarniesz {c}ciekawe{/c} fanty!");
                msgAll(C_GREEN, "{c}(i)*{/c}  Œpiesz siê, jest ich aby {c}%d{/c} na ca³ej mapie!", figureCount); }
            case 6: {
                msgAll(C_LRED, "{c}(i)*{/c}  Czy wiesz, ¿e znajdziesz u nas {c}burdel, klub nocny, itp.{/c}?");
                msgAll(C_LRED, "{c}(i)*{/c}  Nie wiedzia³eœ(aœ)? SprawdŸ ju¿ teraz: {c}/biznes{/c}."); }
            case 7: {
                msgAll(C_CHAT, "{c}(i)*{/c}  Na tym serwerze znajduje siê {c}%d{/c} zabaw, których lista jest po {c}lewej{/c} stronie!", sizeof(gameName));
                msgAll(C_CHAT, "{c}(i)*{/c}  Zachêcamy braæ w nich udzia³, mo¿na {c}³atwo i przyjemnie{/c} siê dorobiæ!"); }
            case 8: {
                msgAll(C_LY, "{c}(i)*{/c}  Jedn¹ z {c}³atwiejszych{/c} metod zarobku jest: {c}³owienie{/c} ryb!");
                msgAll(C_LY, "{c}(i)*{/c}  Po wiêcej informacji udaj siê na: {c}/lowisko{/c}."); }
        }

        msgAll(-1, " ");
        task_timer[1] = 0;
    }

    if(task_timer[2] ++ >= 15) {
        Init_Disks();
        Init_Figure();
        Init_Mining();

        SetTimer("ZonesReward", 1000 * 15, false);

        m_pquery_format("UPDATE es_config SET records='%s', connects=%d, messages=%d, kicks=%d, bans=%d", GetRecords(), connects, messages, kicks, bans);
        task_timer[2] = 0;
    }

    if(task_timer[4] ++ >= 30) {
        if(KurczakEnabled) return 1;
        
        CreateKurczak(15000.0, math_random(70000, 100000));
        task_timer[4] = 0;
    }

    if(task_timer[5] ++ >= 10) {
        msgAll(C_ORANGE, "Respawn pojazdów nast¹pi za {c}10{/c} sekund!");
        SetTimer("PrepareVehicleRespawn", 1000 * 10, false);
        task_timer[5] = 0;
    }
    return 1;
}
