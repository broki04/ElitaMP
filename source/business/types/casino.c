#include YSI_Coding\y_hooks

#define ROULETTE_GREEN  0
#define ROULETTE_RED    1
#define ROULETTE_BLACK  2

new
    pCasinoTokens[MAX_PLAYERS],

    // ruletka
    Float:roulettePosition[][] = {
        {1958.0258, 1010.0102, 992.4688},
        {1962.3414, 1009.8656, 992.4688},
        {1958.0271, 1025.4302, 992.4688},
        {1962.3391, 1025.5510, 992.4688}
    },
    Text3D:rouletteLabel[MAX_BUSINESS][sizeof(roulettePosition)],
    roulettePlayer[MAX_BUSINESS][sizeof(roulettePosition)],

    pRouletteCount[MAX_PLAYERS],
    pRouletteColor[MAX_PLAYERS],
    pRouletteTokens[MAX_PLAYERS],
    pRouletteTimer[MAX_PLAYERS],
    pRouletteNumber[MAX_PLAYERS],

    // ko³o fortuny
    Float:fortunePosition[][] = {
        {1938.0469, 986.6250, 992.8828},
        {1940.6875, 989.1719, 992.8828},
        {1943.2188, 986.5234, 992.8828}
    },
    Text3D:fortuneLabel[MAX_BUSINESS][sizeof(fortunePosition)],
    fortunePlayer[MAX_BUSINESS][sizeof(fortunePosition)],

    pFortuneCount[MAX_PLAYERS],
    pFortuneTimer[MAX_PLAYERS],
    pFortuneBlock[MAX_PLAYERS],
    pFortuneID[MAX_PLAYERS],

    // sloty
    Float:slotsPosition[][] = {
        {1956.8965, 1047.3502, 992.4688},
        {1961.2942, 1042.9294, 992.4688},
        {1963.8268, 1037.0852, 992.4688},
        {1965.8442, 1037.5328, 992.4688},
        {1963.0109, 1044.0803, 992.4688},
        {1958.0287, 1049.0822, 992.4688},
        {1963.8364, 998.5648, 992.4745},
        {1961.3123, 992.5794, 992.4688},
        {1957.0450, 988.3089, 992.4688},
        {1958.1746, 986.5806, 992.4688},
        {1963.0402, 991.4472, 992.4688},
        {1965.8638, 998.1828, 992.4688}
    },
    slotsTypeData[][] = {"hud:radar_mafiaCasino", "hud:radar_pizza", "hud:radar_race", "hud:radar_tshirt", "hud:fist", "hud:radar_chicken", "hud:radar_burgerShot", "hud:radar_dateDisco", "hud:radar_bulldozer"},

    Text3D:slotsLabel[MAX_BUSINESS][sizeof(slotsPosition)],
    slotsPlayer[MAX_BUSINESS][sizeof(slotsPosition)],

    pSlotsSpin[MAX_PLAYERS],
    pSlotsCount[MAX_PLAYERS],
    pSlotsNumber[MAX_PLAYERS][3],
    pSlotsTimer[MAX_PLAYERS],
    pSlotsID[MAX_PLAYERS]
;

hook function ResetPlayerData(playerid) {
    pCasinoTokens[playerid] = 0;

    pRouletteCount[playerid] = pRouletteTokens[playerid] = 0;
    pRouletteColor[playerid] = pRouletteTimer[playerid] = pRouletteNumber[playerid] = -1;
    
    pFortuneCount[playerid] = pFortuneBlock[playerid] = 0;
    pFortuneID[playerid] = pFortuneTimer[playerid] = -1;

    pSlotsCount[playerid] = pSlotsSpin[playerid] = 0;
    pSlotsTimer[playerid] = pSlotsID[playerid] = -1;
    pSlotsNumber[playerid] = {-1, -1, -1};
    return continue(playerid);
}

stock FetchCasino(businessid)
{
    if(businessid == -1) return 1;
    
    businessData[businessid][business_icon] = CreateDynamicMapIcon(unpackXYZ(businessData[businessid][business_position]), 44, -1, 0, 0, .streamdistance=99999.0);
    CreateDynamic3DTextLabel(""BLUE"KASYNO\n"WHITE"Wyjœcie", -1, 2018.6947, 1017.8609, 996.8750 + 0.25, 30.0, .worldid=businessData[businessid][business_worldid]);
    
    businessData[businessid][business_pickupExit] = CreatePickup(1318, 1, 2018.6947, 1017.8609, 996.8750, businessData[businessid][business_worldid]);
    SetPickupVirtualWorld(businessData[businessid][business_pickupExit], businessData[businessid][business_worldid]); // zobaczymy bo sie peirdoli czasem kekw

    CreateDynamicActor(11, 1953.7616, 1019.0192, 992.4688, 269.2051, .worldid=GetBusinessWorld(businessid));
    CreateDynamic3DTextLabel(""RED"KASJERKA\n"WHITE"Kliknij "GREEN"Y"WHITE", aby zakupiæ/wymieniæ ¿etony!", -1, 1953.7616, 1019.0192, 992.4688 + 0.25, 30.0, .worldid=GetBusinessWorld(businessid));

    for(new i = 0; i < sizeof(roulettePosition); i++) {
        rouletteLabel[businessid][i] = CreateDynamic3DTextLabel(form(""CORAL"Stó³ do ruletki "LY"(#%d)\n"LORANGE"U¿ycie: "WHITE"Y", i + 1), -1, unpackXYZ(roulettePosition[i]), 30.0, .worldid=GetBusinessWorld(businessid));
        roulettePlayer[businessid][i] = -1;
    }

    for(new i = 0; i < sizeof(fortunePosition); i++) {
        fortuneLabel[businessid][i] = CreateDynamic3DTextLabel(form(""LBLUE"Ko³o fortuny "LY"(#%d)\n"LIME"U¿ycie: "WHITE"Y", i + 1), -1, unpackXYZ(fortunePosition[i]), 30.0, .worldid=GetBusinessWorld(businessid));
        fortunePlayer[businessid][i] = -1;
    }

    for(new i = 0; i < sizeof(slotsPosition); i++) {
        slotsLabel[businessid][i] = CreateDynamic3DTextLabel(form(""LIME"Automat do gry "LY"(#%d)\n"YELLOW"U¿ycie: "WHITE"Y", i + 1), -1, unpackXYZ(slotsPosition[i]), 30.0, .worldid=GetBusinessWorld(businessid));
        slotsPlayer[businessid][i] = -1;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(playerData[playerid][player_business] != -1) {
        new 
            businessid = playerData[playerid][player_business], 
            rouletteid = GetNearestRoulette(playerid),
            fortuneid = GetNearestFortune(playerid),
            slotid = GetNearestSlots(playerid);

        if(fortuneid != -1 && PRESSED(KEY_YES)) return Fortune_Start(playerid, businessid, fortuneid);
        if(rouletteid != -1 && PRESSED(KEY_YES)) return ShowRoulettePanel(playerid, businessid, rouletteid);
        if(slotid != -1 && PRESSED(KEY_YES)) return Slots_Start(playerid, businessid, slotid);

        if(PRESSED(KEY_YES) && IsPlayerInRangeOfPoint(playerid, 2.0, 1953.7616, 1019.0192, 992.4688)) return ShowCasinoShop(playerid, businessid);
    }
    return 1;
}

// slot machine
stock GetNearestSlots(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(slotsPosition); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 1.25, unpackXYZ(slotsPosition[i]))) {
            id = i;
            break;
        }
    }
    return id;
}

stock Slots_Start(playerid, businessid, slotid)
{
    if(businessid == -1 || slotid == -1 || slotsPlayer[businessid][slotid] != -1) return 1;

    pSlotsNumber[playerid][0] = random(sizeof(slotsTypeData));
    pSlotsNumber[playerid][1] = random(sizeof(slotsTypeData));
    pSlotsNumber[playerid][2] = random(sizeof(slotsTypeData));

    playerTextDrawSetString(playerid, tdCasinoSlot[2], "%s", slotsTypeData[pSlotsNumber[playerid][0]]);
    playerTextDrawSetString(playerid, tdCasinoSlot[3], "%s", slotsTypeData[pSlotsNumber[playerid][1]]);
    playerTextDrawSetString(playerid, tdCasinoSlot[4], "%s", slotsTypeData[pSlotsNumber[playerid][2]]);
    playerTextDrawSetString(playerid, tdCasinoSlot[11], "~w~~h~%d~r~~h~/~w~~h~10", pSlotsSpin[playerid]);

    pSlotsID[playerid] = slotid;
    SetPlayerProgressBarValue(playerid, SlotsBar[playerid], pSlotsSpin[playerid]);
    SetPlayerProgressBarMaxValue(playerid, SlotsBar[playerid], 10);
    ShowPlayerProgressBar(playerid, SlotsBar[playerid]);

    for(new i = 0; i < 14; i++) PlayerTextDrawShow(playerid, tdCasinoSlot[i]);
    for(new i = 0; i < 2; i++) TextDrawShowForPlayer(playerid, tdSlotButton[playerid][i]);
    return SelectTextDraw(playerid, C_CORAL), 1;
}

stock Slots_Hide(playerid)
{
    pSlotsCount[playerid] = 0;

    for(new i = 0; i < 14; i++) PlayerTextDrawHide(playerid, tdCasinoSlot[i]);
    for(new i = 0; i < 2; i++) TextDrawHideForPlayer(playerid, tdSlotButton[playerid][i]);
    return HidePlayerProgressBar(playerid, SlotsBar[playerid]), 1;
}

function Slots_Timer(playerid, businessid, slotid)
{
    if(businessid == -1 || slotid == -1 || slotsPlayer[businessid][slotid] != playerid) return KillTimer(pSlotsTimer[playerid]), 1;

    if(pSlotsCount[playerid] > 0) {
        pSlotsNumber[playerid][0] = random(sizeof(slotsTypeData));
        pSlotsNumber[playerid][1] = random(sizeof(slotsTypeData));
        pSlotsNumber[playerid][2] = random(sizeof(slotsTypeData));

        playerTextDrawSetString(playerid, tdCasinoSlot[2], "%s", slotsTypeData[pSlotsNumber[playerid][0]]);
        playerTextDrawSetString(playerid, tdCasinoSlot[3], "%s", slotsTypeData[pSlotsNumber[playerid][1]]);
        playerTextDrawSetString(playerid, tdCasinoSlot[4], "%s", slotsTypeData[pSlotsNumber[playerid][2]]);

        pSlotsCount[playerid] --;
        if(pSlotsCount[playerid] < 1) {
            new count = 0;
            if(pSlotsNumber[playerid][2] == pSlotsNumber[playerid][0]) count ++;
            if(pSlotsNumber[playerid][0] == pSlotsNumber[playerid][1]) count ++;
            if(pSlotsNumber[playerid][1] == pSlotsNumber[playerid][2]) count ++;
        
            new reward = 0, bool:win = (count > 0) ? (true) : (false);
            switch(count) {
                case 1: reward = math_random(10, 30);
                case 2: reward = math_random(20, 40);
                case 3: reward = math_random(40, 70);
                default: reward = 0;
            }

            if(win) {
                playSound(playerid, 4201);
                playerTextDrawSetString(playerid, tdCasinoSlot[1], "~g~~h~~h~%d ZETONOW", reward);
                SetTimerEx("Slots_RefreshTD", 2500, false, "d", playerid);
            }
            else playSound(playerid, 31203);
            pCasinoTokens[playerid] += reward;

            pSlotsCount[playerid] = 0;
            KillTimer(pSlotsTimer[playerid]);
        }
    }
    else playSound(playerid, 6401);
    return 1;
}
function Slots_RefreshTD(playerid) return PlayerTextDrawSetString(playerid, tdCasinoSlot[1], "AUTOMAT DO SLOTOW"), 1;

// ko³o fortuny
stock GetNearestFortune(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(fortunePosition); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, unpackXYZ(fortunePosition[i]))) {
            id = i;
            break;
        }
    }
    return id;
}

stock Fortune_Start(playerid, businessid, fortuneid)
{
    if(businessid == -1 || fortuneid == -1 || fortunePlayer[businessid][fortuneid] != -1) return 1;
    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Rozpocznij losowanie juz teraz!~n~~w~~h~Przyjdz rowniez jutro.");
    playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");

    if(pFortuneBlock[playerid] > gettime()) {
        playerTextDrawSetString(playerid, tdFortune[2], "~w~~h~Przychodz tutaj ~r~~h~codziennie~w~~h~, by odebrac nagrode!~n~~w~~h~Nastepne losowanie mozesz wykonac za: ~p~~h~%s~w~~h~.", DeletePolish(ConvertTime(pFortuneBlock[playerid], TIME_TYPE_UNIX)));

        PlayerTextDrawBoxColor(playerid, tdFortune[3], setAlpha(C_RED, 255));
        PlayerTextDrawBoxColor(playerid, tdFortune[4], setAlpha(C_RED, 255));
        PlayerTextDrawBoxColor(playerid, tdFortune[9], setAlpha(C_RED, 255));
    } else {
        playerTextDrawSetString(playerid, tdFortune[2], "~w~~h~Przychodz tutaj ~r~~h~codziennie~w~~h~, by odebrac nagrode!~n~~w~~h~Kliknij ponizej przycisk ~y~~h~X~w~~h~, aby rozpoczac losowanie.");

        PlayerTextDrawBoxColor(playerid, tdFortune[3], setAlpha(C_DGREEN, 255));
        PlayerTextDrawBoxColor(playerid, tdFortune[4], setAlpha(C_DGREEN, 255));
        PlayerTextDrawBoxColor(playerid, tdFortune[9], setAlpha(C_DGREEN, 255));
    }
    pFortuneID[playerid] = fortuneid;

    TextDrawShowForPlayer(playerid, tdCasinoButton[playerid]);
    for(new i = 0; i < 10; i++) PlayerTextDrawShow(playerid, tdFortune[i]);
    return SelectTextDraw(playerid, C_CORAL), 1;
}

function Fortune_Timer(playerid, businessid, fortuneid)
{
    if(businessid == -1 || fortuneid == -1 || fortunePlayer[businessid][fortuneid] != playerid) return KillTimer(pFortuneTimer[playerid]);

    if(pFortuneCount[playerid] > 0) {
        pFortuneCount[playerid] --;

        new number = random(5);
        switch(number) {
            case 0: playerTextDrawSetString(playerid, tdFortune[5], "~b~~h~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
            case 1: playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~b~~h~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
            case 2: playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~b~~h~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
            case 3: playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~b~~h~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
            case 4: playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~b~~h~~h~Dodatkowe punkty pracy");
        }

        if(pFortuneCount[playerid] < 1) {
            playSound(playerid, 4201);

            switch(number) {
                case 0: {
                    new days = math_random(5, 12);
                    givePremium(playerid, days);

                    msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} wylosowa³(a) {c}Konto VIP{/c} na okres {c}%d dni{/c} w kasynie!", getNick(playerid), playerid, days);
                    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Dziekujemy za udzial w losowaniu!~n~~w~~h~Zachecamy czesciej losowac!~n~~n~~w~~h~Twoja wylosowana nagroda to:~n~~r~~h~VIP na %d dni!", days);
                    playerTextDrawSetString(playerid, tdFortune[5], "~g~~h~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
                }
               
                case 1: {
                    new token = math_random(500, 800);
                    pCasinoTokens[playerid] += token;

                    msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} wylosowa³(a) {c}%d ¿etonów{/c} w kasynie!", getNick(playerid), playerid, token);
                    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Dziekujemy za udzial w losowaniu!~n~~w~~h~Zachecamy czesciej losowac!~n~~n~~w~~h~Twoja wylosowana nagroda to:~n~~r~~h~%d zetonow!", token);
                    playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~g~~h~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
                }
               
                case 2: {
                    new level = math_random(8, 15);
                    playerData[playerid][player_level] += level;

                    msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} wylosowa³(a) {c}%d-krotny{/c} wzrost poziomu w kasynie!", getNick(playerid), playerid, level);
                    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Dziekujemy za udzial w losowaniu!~n~~w~~h~Zachecamy czesciej losowac!~n~~n~~w~~h~Twoja wylosowana nagroda to:~n~~r~~h~%d-krotny wzrost poziomu!", level);
                    playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~g~~h~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
                }
               
                case 3: {
                    new money = math_random(70000, 100000);
                    giveMoney(playerid, money);

                    msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} wylosowa³(a) {c}%s${/c} w kasynie!", getNick(playerid), playerid, strdot(money));
                    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Dziekujemy za udzial w losowaniu!~n~~w~~h~Zachecamy czesciej losowac!~n~~n~~w~~h~Twoja wylosowana nagroda to:~n~~r~~h~%s$!", strdot(money));
                    playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~g~~h~~h~Darmowa dotacja~n~~w~~h~Dodatkowe punkty pracy");
                }
                
                case 4: {
                    new points = math_random(6, 10);
                    playerData[playerid][player_job_points] += points;

                    msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} wylosowa³(a) {c}%d pkt. pracy{/c} w kasynie!", getNick(playerid), playerid, points);
                    playerTextDrawSetString(playerid, tdFortune[8], "~w~~h~Dziekujemy za udzial w losowaniu!~n~~w~~h~Zachecamy czesciej losowac!~n~~n~~w~~h~Twoja wylosowana nagroda to:~n~~r~~h~%d pkt. pracy!", points);
                    playerTextDrawSetString(playerid, tdFortune[5], "~w~~h~Darmowe konto VIP~n~~w~~h~Dodatkowe zetony do kasyna~n~~w~~h~Kilkukrotny wzrost poziomu~n~~w~~h~Darmowa dotacja~n~~g~~h~~h~Dodatkowe punkty pracy");
                }
            }
            AddBusinessMoney(businessid, math_random(1000, 3000));

            fortunePlayer[businessid][fortuneid] = -1;
            pFortuneBlock[playerid] = gettime() + UnixTime('d', 1);
            m_pquery_format("UPDATE es_accounts SET fortune_block=%d WHERE id=%d LIMIT 1;", pFortuneBlock[playerid], playerData[playerid][player_accountID]);

            new fstr[128]; format(fstr, sizeof(fstr), ""LBLUE"Ko³o fortuny "LY"(#%d)\n"LIME"U¿ycie: "WHITE"Y", fortuneid + 1);
            UpdateDynamic3DTextLabelText(fortuneLabel[businessid][fortuneid], -1, fstr);
            
            SetTimerEx("Fortune_Hide", 3000, false, "d", playerid);
            KillTimer(pFortuneTimer[playerid]);
        }
        else playSound(playerid, 1056);
    }
    return 1;
}

function Fortune_Hide(playerid) 
{ 
    for(new i = 0; i < 10; i++) PlayerTextDrawHide(playerid, tdFortune[i]); 
    return TextDrawHideForPlayer(playerid, tdCasinoButton[playerid]), 1;
}

// ruletka
stock GetNearestRoulette(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(roulettePosition); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, unpackXYZ(roulettePosition[i]))) {
            id = i;
            break;
        }
    }
    return id;
}

stock ShowRoulettePanel(playerid, businessid, rouletteid)
{
    if(businessid == -1 || roulettePlayer[businessid][rouletteid] != -1 || pRouletteColor[playerid] != -1) return 1;
    playerData[playerid][player_saveData][0] = businessid;
    playerData[playerid][player_saveData][1] = rouletteid;

    s[0] = EOS;
    s = ""BLUE2"Typ\t"WHITE"Mno¿nik\n";

    catstr(s, C_FAJNY, -1, "Iloœæ ¿etonów:\t{c}%s{/c} %s\n", strdot(pCasinoTokens[playerid]), ConvertTimeFormat(pCasinoTokens[playerid], "¿etonów", "¿eton", "¿etony", "¿etonów"));
    catstr(s, C_FAJNY, -1, "Postawionych ¿etonów:\t{c}%s{/c} %s\n", strdot(pRouletteTokens[playerid]), ConvertTimeFormat(pRouletteTokens[playerid], "¿etonów", "¿eton", "¿etony", "¿etonów"));
    strcat(s, " \n");
    catstr(s, C_WHITE, -1, "Postaw na {fd0000}czerwony\t"WHITE"x2.0\n");
    catstr(s, C_WHITE, -1, "Postaw na {2b2d30}czarny\t"WHITE"x2.0\n");
    catstr(s, C_WHITE, -1, "Postaw na {03fc0b}zielony\t"WHITE"x10.0\n");
    strcat(s, " \n");
    catstr(s, C_WHITE, -1, "Postaw na "BLUE2"konkretn¹ "WHITE"liczbê\t"WHITE"x5.0\n");
    return showDialog(playerid, DIALOG_ROULETTE_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Ruletka", s, "Wybierz", "Zamknij"), 1;
}

stock Roulette_Start(playerid, businessid, rouletteid, typeid)
{
    if(businessid == -1 || roulettePlayer[businessid][rouletteid] != -1) return 1;

    roulettePlayer[businessid][rouletteid] = playerid;
    
    pRouletteCount[playerid] = 10;
    pRouletteColor[playerid] = typeid;
    pRouletteTimer[playerid] = SetTimerEx("Roulette_Timer", 600, true, "ddd", playerid, businessid, rouletteid);

    new rstr[128]; format(rstr, sizeof(rstr), ""CORAL"Stó³ do ruletki "LY"(#%d)\n"LORANGE"Obecnie gra: "WHITE"%s(%d)\n\n", rouletteid + 1, getNick(playerid), playerid);
    return UpdateDynamic3DTextLabelText(rouletteLabel[businessid][rouletteid], -1, rstr), 1;
}

stock GetRouletteColor(typeid)
{
    new name[20];
    switch(typeid) {
        case ROULETTE_BLACK: name = "czarny";
        case ROULETTE_RED: name = "czerwony";
        case ROULETTE_GREEN: name = "zielony";
        default: name = "invalid";
    }
    return name;
}

function Roulette_Timer(playerid, businessid, rouletteid)
{
    if(businessid == -1 || rouletteid == -1 || roulettePlayer[businessid][rouletteid] != playerid) return KillTimer(pRouletteTimer[playerid]), 1;

    if(pRouletteCount[playerid] > 0) {
        new number = math_random(0, 36), typeid, color[20];
        if(number == 0)             { typeid = ROULETTE_GREEN;  color = "{03fc0b}"; }
        else if(number % 2 == 0)    { typeid = ROULETTE_RED;    color = "{fd0000}"; } 
        else                        { typeid = ROULETTE_BLACK;  color = "{2b2d30}"; }

        new rstr[128]; format(rstr, sizeof(rstr), ""CORAL"Stó³ do ruletki "LY"(#%d)\n"LORANGE"Obecnie gra: "WHITE"%s(%d)\n\n%s%d", rouletteid + 1, getNick(playerid), playerid, color, number);
        UpdateDynamic3DTextLabelText(rouletteLabel[businessid][rouletteid], -1, rstr);

        pRouletteCount[playerid] --;
        playSound(playerid, 33400);

        if(pRouletteCount[playerid] < 1) {
            if(pRouletteNumber[playerid] != -1) {
                if(pRouletteNumber[playerid] == number) {
                    new math = pRouletteTokens[playerid] * 5;
                    pCasinoTokens[playerid] += math;

                    AddBusinessMoney(businessid, (floatround((math * businessData[businessid][business_price]) * 0.08)));
                    msg(playerid, C_MINT, "Wygra³eœ(aœ) ruletkê, stawiaj¹c na {c}#%d{/c} numer! Zyskujesz {c}%s{/c} ¿etonów.", pRouletteNumber[playerid], strdot(math));
                } else {
                    AddBusinessMoney(businessid, (floatround((pRouletteTokens[playerid] * businessData[businessid][business_price]) * 0.12)));
                    msg(playerid, C_MINT, "Przegra³eœ(aœ) ruletkê, stawiaj¹c na {c}#%d{/c} numer! Tracisz {c}%s{/c} ¿etonów.", pRouletteNumber[playerid], strdot(pRouletteTokens[playerid]));
                }
            } else {
                if(typeid == pRouletteColor[playerid]) {           
                    new math = 0;
                    switch(typeid) {
                        case ROULETTE_RED, ROULETTE_BLACK:  math = pRouletteTokens[playerid] * 2;
                        case ROULETTE_GREEN:                math = pRouletteTokens[playerid] * 10;
                    }
                    pCasinoTokens[playerid] += math;

                    AddBusinessMoney(businessid, (floatround((math * businessData[businessid][business_price]) * 0.05)));
                    msg(playerid, C_MINT, "Wygra³eœ(aœ) ruletkê, stawiaj¹c na {c}%s{/c} kolor! Zyskujesz {c}%s{/c} ¿etonów.", GetRouletteColor(pRouletteColor[playerid]), strdot(math));
                } else {
                    AddBusinessMoney(businessid, (floatround((pRouletteTokens[playerid] * businessData[businessid][business_price]) * 0.15)));
                    msg(playerid, C_MINT, "Przegra³eœ(aœ) ruletkê, stawiaj¹c na {c}%s{/c} kolor! Tracisz {c}%s{/c} ¿etonów.", GetRouletteColor(pRouletteColor[playerid]), strdot(pRouletteTokens[playerid]));
                }
            }

            pRouletteColor[playerid] = pRouletteNumber[playerid] = -1;
            pRouletteTokens[playerid] = 0;

            playSound(playerid, 1186);
            SetTimerEx("Roulette_Refresh", 2000, false, "dd", businessid, rouletteid);
            KillTimer(pRouletteTimer[playerid]);
        }
    }
    return 1;
}

function Roulette_Refresh(businessid, rouletteid)
{
    roulettePlayer[businessid][rouletteid] = -1;

    new rstr[128]; format(rstr, sizeof(rstr), ""CORAL"Stó³ do ruletki "LY"(#%d)\n"LORANGE"U¿ycie: "WHITE"Y", rouletteid + 1);
    return UpdateDynamic3DTextLabelText(rouletteLabel[businessid][rouletteid], -1, rstr), 1;
}

// wymiana/zakup ¿etonów
stock ShowCasinoShop(playerid, businessid)
{
    if(businessid == -1) return 1;
    playerData[playerid][player_saveData][0] = businessid;

    s[0] = EOS;
    catstr(s, C_GITOWY, -1, "Posiadasz obecnie {c}%s{/c} ¿etonów.\n", strdot(pCasinoTokens[playerid]));
    strcat(s, " \n");
    catstr(s, C_ZAJEBISTY, -1, "  "ARROW" "WHITE"Zakup ¿etony "LY"(%s$)\n", strdot(businessData[businessid][business_price]));
    catstr(s, C_ZAJEBISTY, -1, "  "ARROW" "WHITE"Wymiana ¿etonów\n");
    catstr(s, C_ZAJEBISTY, -1, "  "ARROW" "WHITE"Podium najwiêkszych hazardzistów\n");
    return showDialog(playerid, DIALOG_CASINO_SHOP, DIALOG_STYLE_LIST, ""YELLOW"Sklep kasyna", s, "Wybierz", "Zamknij");
}

// 

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    new 
        businessid = playerData[playerid][player_business],
        fortuneid = pFortuneID[playerid], 
        slotid = pSlotsID[playerid];

    if(clickedid == INVALID_TEXT_DRAW) {
        if(fortuneid != -1) {
            if(fortunePlayer[businessid][fortuneid] == playerid && pFortuneCount[playerid] > 0) return msg(playerid, C_DRED, "Nie mo¿esz teraz anulowaæ ko³a fortuny!"), 1;
            fortunePlayer[businessid][fortuneid] = -1;

            pFortuneID[playerid] = -1;
            KillTimer(pFortuneTimer[playerid]);
            return Fortune_Hide(playerid), 1;
        }

        if(slotid != -1) {
            if(slotsPlayer[businessid][slotid] == playerid && pSlotsCount[playerid] > 0) return msg(playerid, C_DRED, "Nie mo¿esz teraz anulowaæ automatu do slotów!"), 1;
            slotsPlayer[businessid][slotid] = -1;

            new astr[128]; format(astr, sizeof(astr), ""LIME"Automat do gry "LY"(#%d)\n"YELLOW"U¿ycie: "WHITE"Y", slotid + 1);
            UpdateDynamic3DTextLabelText(slotsLabel[businessid][slotid], -1, astr);

            pSlotsID[playerid] = -1;
            KillTimer(pSlotsTimer[playerid]);
            return Slots_Hide(playerid), 1;
        }
    } else {
        if(clickedid == tdCasinoButton[playerid] && pFortuneCount[playerid] < 1) {
            if(pFortuneBlock[playerid] > gettime()) return Fortune_Hide(playerid), 1;
            fortunePlayer[businessid][fortuneid] = playerid;

            pFortuneCount[playerid] = 10;
            pFortuneTimer[playerid] = SetTimerEx("Fortune_Timer", 1000, true, "ddd", playerid, businessid, fortuneid);

            new fstr[128]; format(fstr, sizeof(fstr), ""LBLUE"Ko³o fortuny "LY"(#%d)\n"LIME"Obecnie gra: "WHITE"%s(%d)", fortuneid + 1, getNick(playerid), playerid);
            return UpdateDynamic3DTextLabelText(fortuneLabel[businessid][fortuneid], -1, fstr), 1;
        }

        if(clickedid == tdSlotButton[playerid][0] && pSlotsCount[playerid] < 1) {
            if(pCasinoTokens[playerid] < 100) return msg(playerid, C_DRED, "Aby zakupiæ wszystkie {c}zakrêcenia{/c}, musisz posiadaæ {c}100{/c} ¿etonów.");
            if(pSlotsSpin[playerid] >= 10) return msg(playerid, C_DRED, "Posiadasz ju¿ {c}maksymaln¹{/c} liczbê zakrêceñ!");

            pCasinoTokens[playerid] -= 100;
            pSlotsSpin[playerid] = 10;

            SetPlayerProgressBarValue(playerid, SlotsBar[playerid], pSlotsSpin[playerid]);
            SetPlayerProgressBarMaxValue(playerid, SlotsBar[playerid], 10);
            playerTextDrawSetString(playerid, tdCasinoSlot[11], "~w~~h~%d~r~~h~/~w~~h~10", pSlotsSpin[playerid]);
            return AddBusinessMoney(businessid, (20 * businessData[businessid][business_price])), 1;
        }

        if(clickedid == tdSlotButton[playerid][1] && pSlotsCount[playerid] < 1) {
            if(pSlotsSpin[playerid] < 1) return msg(playerid, C_DRED, "Nie posiadasz {c}¿adnych{/c} zakrêceñ.");

            pSlotsSpin[playerid] --;
            SetPlayerProgressBarValue(playerid, SlotsBar[playerid], pSlotsSpin[playerid]);
            SetPlayerProgressBarMaxValue(playerid, SlotsBar[playerid], 10);
            playerTextDrawSetString(playerid, tdCasinoSlot[11], "~w~~h~%d~r~~h~/~w~~h~10", pSlotsSpin[playerid]);

            slotsPlayer[businessid][slotid] = playerid;
            pSlotsCount[playerid] = 5;
            pSlotsTimer[playerid] = SetTimerEx("Slots_Timer", 700, true, "ddd", playerid, businessid, slotid);
           
            new astr[128]; format(astr, sizeof(astr), ""LIME"Automat do gry "LY"(#%d)\n"YELLOW"Obecnie gra: "WHITE"%s(%d)", slotid + 1, getNick(playerid), playerid);
            return UpdateDynamic3DTextLabelText(slotsLabel[businessid][slotid], -1, astr), 1;
        }
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_CASINO_TOKEN_BUY: {
            if(!response) return ShowCasinoShop(playerid, playerData[playerid][player_saveData][0]);

            new businessid = playerData[playerid][player_saveData][0];
            if(businessid == -1) return 1;

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1) {
                s[0] = EOS;
                catstr(s, C_FAJNY, -1, "Koszt zakupu {c}1 ¿etonu{/c} wynosi w tym kasynie: {c}%s${/c}.\n"FAJNY"Wpisz poni¿ej, ile {c}¿etonów{/c} chcesz zakupiæ:\n\n"RED"Nieprawid³owa iloœæ ¿etonów.", strdot(businessData[businessid][business_price]));
                return showDialog(playerid, DIALOG_CASINO_TOKEN_BUY, DIALOG_STYLE_INPUT, ""MINT"Zakup ¿etonów", s, "Zakup", "Cofnij");
            }

            new price = (businessData[businessid][business_price] * amount);
            if(getMoney(playerid) < price) {
                s[0] = EOS;
                catstr(s, C_FAJNY, -1, "Koszt zakupu {c}1 ¿etonu{/c} wynosi w tym kasynie: {c}%s${/c}.\n"FAJNY"Wpisz poni¿ej, ile {c}¿etonów{/c} chcesz zakupiæ:\n\n"RED"Nie staæ Ciê na zakup {c}%s{/c} ¿etonów.", strdot(businessData[businessid][business_price]), strdot(amount));
                return showDialog(playerid, DIALOG_CASINO_TOKEN_BUY, DIALOG_STYLE_INPUT, ""MINT"Zakup ¿etonów", s, "Zakup", "Cofnij");
            }

            pCasinoTokens[playerid] += amount;
            AddBusinessMoney(businessid, (floatround((amount * businessData[businessid][business_reward]) * 0.05)));

            giveMoney(playerid, -price);
            msg(playerid, C_ZAJEBISTY, "Kupi³eœ(aœ) {c}%s{/c} ¿etonów za: {c}%s${/c}.", strdot(amount), strdot(price));
            return ShowCasinoShop(playerid, businessid), 1;
        }

        case DIALOG_CASINO_TOKEN_EXCHANGE: {
            if(!response) return ShowCasinoShop(playerid, playerData[playerid][player_saveData][0]);

            new businessid = playerData[playerid][player_saveData][0];
            if(businessid == -1) return 1;

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1 || amount > pCasinoTokens[playerid]) {
                s[0] = EOS;
                catstr(s, C_FAJNY, -1, "Koszt zakupu {c}1 ¿etonu{/c} wynosi w tym kasynie: {c}%s${/c}.\n"FAJNY"Wpisz poni¿ej, ile {c}¿etonów{/c} chcesz zakupiæ:\n\n"RED"Nieprawid³owa iloœæ ¿etonów.", strdot(businessData[businessid][business_price]));
                return showDialog(playerid, DIALOG_CASINO_TOKEN_BUY, DIALOG_STYLE_INPUT, ""MINT"Zakup ¿etonów", s, "Zakup", "Cofnij");
            }

            new money = (amount * businessData[businessid][business_price]);
            pCasinoTokens[playerid] -= amount;
            giveMoney(playerid, money);

            msg(playerid, C_ZAJEBISTY, "Wymieni³eœ(aœ) {c}%s{/c} ¿etonów na: {c}%s${/c}.", strdot(amount), strdot(money));
            return ShowCasinoShop(playerid, businessid), 1;
        }

        case DIALOG_CASINO_SHOP: {
            if(!response) return 1;

            new businessid = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 2: {
                    s[0] = EOS;
                    catstr(s, C_FAJNY, -1, "Koszt zakupu {c}1 ¿etonu{/c} wynosi w tym kasynie: {c}%s${/c}.\n"FAJNY"Wpisz poni¿ej, ile {c}¿etonów{/c} chcesz zakupiæ:", strdot(businessData[businessid][business_price]));
                    showDialog(playerid, DIALOG_CASINO_TOKEN_BUY, DIALOG_STYLE_INPUT, ""MINT"Zakup ¿etonów", s, "Zakup", "Cofnij");
                }

                case 3: {
                    s[0] = EOS;
                    catstr(s, C_FAJNY, -1, "Wartoœæ {c}1 ¿etonu{/c} jest warta: {c}%s${/c}.\n"FAJNY"Wpisz poni¿ej, ile {c}¿etonów{/c} chcesz rozmieniæ na gotówkê:", strdot(businessData[businessid][business_price]));
                    showDialog(playerid, DIALOG_CASINO_TOKEN_EXCHANGE, DIALOG_STYLE_INPUT, ""MINT"Wymiana ¿etonów", s, "Wymieñ", "Cofnij");
                }

                case 4: {
                    msg(playerid, -1, "tutaj bedzie topka oooo");
                }

                default: ShowCasinoShop(playerid, businessid);
            }
            return 1;
        }

        case DIALOG_ROULETTE_TOKEN: {
            if(!response) return ShowRoulettePanel(playerid, playerData[playerid][player_saveData][0], playerData[playerid][player_saveData][1]);

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1 || amount > pCasinoTokens[playerid]) return showDialog(playerid, DIALOG_ROULETTE_TOKEN, DIALOG_STYLE_INPUT, ""BLUE2"Postaw ¿etony", ""WHITE"Wpisz poni¿ej, ile ¿etonów chcesz postawiæ na ruletkê:\n\n"RED"Nieprawid³owa iloœæ tokenów.", "Postaw", "Cofnij");
            
            pCasinoTokens[playerid] -= amount;
            pRouletteTokens[playerid] += amount;
            
            new businessid = playerData[playerid][player_saveData][0], rouletteid = playerData[playerid][player_saveData][1];
            if(GetPVarInt(playerid, "casino_token_listitem") > 0) {
                new _listitem = GetPVarInt(playerid, "casino_token_listitem");
                switch(_listitem) {
                    case 3: Roulette_Start(playerid, businessid, rouletteid, ROULETTE_RED);
                    case 4: Roulette_Start(playerid, businessid, rouletteid, ROULETTE_BLACK);
                    case 5: Roulette_Start(playerid, businessid, rouletteid, ROULETTE_GREEN);
                    default: ShowRoulettePanel(playerid, businessid, rouletteid);
                }
            }
            else ShowRoulettePanel(playerid, businessid, rouletteid);
            return 1;
        }

        case DIALOG_ROULETTE_NUMBER: {
            if(!response) return ShowRoulettePanel(playerid, playerData[playerid][player_saveData][0], playerData[playerid][player_saveData][1]);

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 0 || amount > 36) return showDialog(playerid, DIALOG_ROULETTE_NUMBER, DIALOG_STYLE_INPUT, ""VIOLET"Postaw konkrentn¹ liczbê", ""WHITE"Wpisz poni¿ej liczbê z przedzia³u "GREEN"0-36"WHITE", na któr¹ chcesz postawiæ:\n\n"RED"Musisz wpisaæ liczbê z przedzia³u "WHITE"0-36"RED".", "Postaw", "Cofnij");
            
            pRouletteNumber[playerid] = amount;
            return Roulette_Start(playerid, playerData[playerid][player_saveData][0], playerData[playerid][player_saveData][1], -1), 1;
        }

        case DIALOG_ROULETTE_PANEL: {
            if(!response) return 1;

            new businessid = playerData[playerid][player_saveData][0];
            if(pRouletteTokens[playerid] < 1 && (listitem == 3 || listitem == 4 || listitem == 5 || listitem == 7)) {
                SetPVarInt(playerid, "casino_token_listitem", listitem);
                return showDialog(playerid, DIALOG_ROULETTE_TOKEN, DIALOG_STYLE_INPUT, ""BLUE2"Postaw ¿etony", ""WHITE"Wpisz poni¿ej, ile ¿etonów chcesz postawiæ na ruletkê:", "Postaw", "Cofnij");
            }

            switch(listitem) {
                case 1: showDialog(playerid, DIALOG_ROULETTE_TOKEN, DIALOG_STYLE_INPUT, ""BLUE2"Postaw ¿etony", ""WHITE"Wpisz poni¿ej, ile ¿etonów chcesz postawiæ na ruletkê:", "Postaw", "Cofnij");
                case 3: Roulette_Start(playerid, businessid, playerData[playerid][player_saveData][1], ROULETTE_RED);
                case 4: Roulette_Start(playerid, businessid, playerData[playerid][player_saveData][1], ROULETTE_BLACK);
                case 5: Roulette_Start(playerid, businessid, playerData[playerid][player_saveData][1], ROULETTE_GREEN);
                case 7: showDialog(playerid, DIALOG_ROULETTE_NUMBER, DIALOG_STYLE_INPUT, ""VIOLET"Postaw konkrentn¹ liczbê", ""WHITE"Wpisz poni¿ej liczbê z przedzia³u "GREEN"0-36"WHITE", na któr¹ chcesz postawiæ:", "Postaw", "Cofnij");
                default: ShowRoulettePanel(playerid, businessid, playerData[playerid][player_saveData][1]);
            }
            return 1;
        }
    }
    return 0;
}