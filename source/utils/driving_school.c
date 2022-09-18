#include YSI_Coding\y_hooks

#define KAT_B           0
#define KAT_A           1
#define KAT_C           2

enum e_drivingExamData {
    exam_type,
    exam_name[20],
    exam_cost,
};

new 
    drivingExamData[][e_drivingExamData] = {
        {KAT_B, "Kat. B",   1000},
        {KAT_A, "Kat. A",   3000},
        {KAT_C, "Kat. C",   5000}
    },

    bool:pDriveLicense[MAX_PLAYERS][sizeof(drivingExamData)],
    pDriveCP[MAX_PLAYERS], 
    pDriveStatus[MAX_PLAYERS],
    bool:pDriveExam[MAX_PLAYERS], 
    pDriveType[MAX_PLAYERS],
    pDriveVehicle[MAX_PLAYERS],
    pDriveTimer[MAX_PLAYERS]
;

new const Float:examPositions[][] = {
    {2079.3582, -1913.5422, 12.9097},
    {2005.4849, -1930.2965, 12.9112},
    {1926.1304, -1929.8087, 12.9472},
    {1826.6580, -1930.0406, 12.9513},
    {1823.9468, -1840.6227, 12.9803},
    {1823.7772, -1675.6608, 12.9477},
    {1823.6621, -1611.7041, 12.9516},
    {1528.0723, -1589.6956, 12.9497},
    {1527.1820, -1733.6116, 12.9556},
    {1564.3228, -1735.5726, 12.9528},
    {1567.8231, -1868.5421, 12.9534},
    {1642.0061, -1873.7715, 12.9494},
    {1691.4402, -1819.3850, 12.9489},
    {1737.1842, -1819.9116, 12.9340},
    {1817.3623, -1835.5435, 12.9841},
    {1819.3635, -1929.7391, 12.9399},
    {1915.9796, -1934.2758, 12.9487},
    {2025.6771, -1934.8939, 12.8946},
    {2080.5481, -1935.1223, 12.8713},
    {2083.7346, -1911.0309, 12.9508},
    {2061.5593, -1919.4403, 13.1281}
};

hook function ResetPlayerData(playerid) {
    for(new i = 0; i < sizeof(drivingExamData); i++) pDriveLicense[playerid][i] = false;
    pDriveStatus[playerid] = 0;
    pDriveExam[playerid] = false;
    pDriveType[playerid] = -1;
    return continue(playerid);
}

new WordCP;
stock Init_Driving()
{
    CreateDynamicPickup(19130, 1, 2055.4314, -1910.4348, 13.5654, .worldid=0);
    CreateDynamic3DTextLabel(""BLUE"SZKO£A JAZDY\n"YELLOW"Wejœcie", -1, 2055.4314, -1910.4348, 13.5654 + 0.25, 30.0, .worldid=0);

    CreateDynamicPickup(19130, 1, 2220.7351, -1921.4291, -11.4952, .worldid=VW_WORD, .interiorid=VW_WORD);
    CreateDynamic3DTextLabel(""BLUE"SZKO£A JAZDY\n"YELLOW"Wyjœcie", -1, 2220.7351, -1921.4291, -11.4952 + 0.25, 30.0, .worldid=VW_WORD, .interiorid=VW_WORD);

    WordCP = CreateDynamicCP(2224.6836, -1938.0756, -11.4952, 1.25, .worldid=VW_WORD, .interiorid=VW_WORD);
    CreateDynamic3DTextLabel(""BLUE"EGZAMINY\n"WHITE"Tutaj mo¿esz rozpocz¹æ "YELLOW"egzaminy "WHITE"na prawo jazdy!", -1, 2224.6836, -1938.0756, -11.4952 + 0.35, 30.0, .worldid=VW_WORD, .interiorid=VW_WORD);

    CreateDynamicActor(17, 2221.7512, -1937.9233, -11.4952, 0.7406, .worldid=VW_WORD, .interiorid=VW_WORD);
    CreateDynamicActor(17, 2226.4019, -1922.1366, -11.4952, 177.0640, .worldid=VW_WORD, .interiorid=VW_WORD);
    return 1;
}

stock Task:RunDriveExams(playerid)
{
    new Task:t = task_new();

    s[0] = EOS;
    s = ""WHITE"Kategoria\t"BLUE"Cena za praktykê\t"RED"Sta"GREEN"tus\n";
    for(new i = 0; i < sizeof(drivingExamData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"BLUE"%s$\t%s\n", s, drivingExamData[i][exam_name], strdot(drivingExamData[i][exam_cost]), (pDriveLicense[playerid][i]) ? (""GREEN"ZDANE") : (""RED"NIEZDANE"));
    return showDialog(playerid, DIALOG_DRIVING_EXAM, DIALOG_STYLE_TABLIST_HEADERS, ""ORANGE"Egzaminy serwerowe", s, "Wybierz", "Zamknij"), t;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(checkpointid == WordCP) return task_await(RunDriveExams(playerid));
    return 1;
}

stock WordPickup(playerid, pickupid)
{
    #pragma unused pickupid
    if(IsPlayerInRangeOfPoint(playerid, 2.0, 2055.4314, -1910.4348, 13.5654)) return Teleport(playerid, false, 2220.6653, -1923.7905, -11.4952, 183.1855, VW_WORD, VW_WORD, false);
    if(IsPlayerInRangeOfPoint(playerid, 2.0, 2220.7351, -1921.4291, -11.4952)) return Teleport(playerid, false, 2055.4314, -1910.4348, 13.5654, 91.0595, 0, 0, false);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_DRIVING_EXAM: {
            if(!response) return 1;

            if(getMoney(playerid) < drivingExamData[listitem][exam_cost]) return valueError(playerid, drivingExamData[listitem][exam_cost]);
            if(pDriveLicense[playerid][listitem]) return sendError(playerid, ""RED"Posiadasz ju¿ prawo jazdy tej kategorii!");

            SavePlayerPosition(playerid);
            ResetPlayerWeapons(playerid);
            SetHealth(playerid, 100.0, 100.0);
            DisableRemoteVehicleCollisions(playerid, true);
            giveMoney(playerid, -drivingExamData[listitem][exam_cost]);

            pDriveStatus[playerid] = 0;
            pDriveExam[playerid] = playerData[playerid][player_god] = true;
            pDriveType[playerid] = listitem;

            new modelid = -1;
            switch(listitem) {
                case KAT_A: modelid = 521;
                case KAT_B: modelid = 550;
                case KAT_C: modelid = 456;
            }

            switch(random(3)) {
                case 0: pDriveVehicle[playerid] = CreateVehicle(modelid, 2068.5161, -1918.6473, 13.2692, 340.7401, -1, -1, -1);
                case 1: pDriveVehicle[playerid] = CreateVehicle(modelid, 2065.1072, -1918.4330, 13.2692, 337.6257, -1, -1, -1);
                case 2: pDriveVehicle[playerid] = CreateVehicle(modelid, 2060.8948, -1919.0604, 13.2692, 340.8019, -1, -1, -1);
            }

            Teleport(playerid, false, 2055.4314 + math_random_float(-3.0, 3.0), -1910.4348, 13.5654, 0.0, 0, 0, false);
            SetVehicleVirtualWorld(pDriveVehicle[playerid], 0);
            PutPlayerInVehicle(playerid, pDriveVehicle[playerid], 0);

            s[0] = EOS;
            catstr(s, C_NICK, -1, "Rozpoczêto {c}egzamin praktyczny{/c} na prawo jazdy: {c}%s{/c}!\n", drivingExamData[listitem][exam_name]);
            catstr(s, C_NICK, -1, "Koszt za jednorazowy egzamin wynosi {c}%s${/c}.\n", strdot(drivingExamData[listitem][exam_cost]));
            catstr(s, C_NICK, -1, "JedŸ zgodnie z {c}wytycznymi{/c} egzaminatora!");
            showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Egzamin praktyczny", s, "OK", #);

            if(IsValidDynamicRaceCP(pDriveCP[playerid])) DestroyDynamicRaceCP(pDriveCP[playerid]);
            pDriveCP[playerid] = CreateDynamicRaceCP(0, unpackXYZ(examPositions[0]), unpackXYZ(examPositions[1]), 5.0, .worldid=0, .playerid=playerid); 
            return msg(playerid, C_YELLOW, "Rozpoczêto {c}egzamin praktyczny{/c} na prawo jazdy {c}%s{/c}. JedŸ zgodnie z wskazaniami!", drivingExamData[listitem][exam_name]);
        }
    }
    return 0;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(pDriveExam[playerid] && pDriveType[playerid] != -1 && pDriveVehicle[playerid] == vehicleid) {
        pDriveTimer[playerid] = 10;
        showGameDraw(playerid, "~r~~h~MASZ ~w~~h~10 ~r~~h~SEKUND BY WROCIC DO POJAZDU!", 3);
    }
    return 1;
}

stock DriveExamTimer(playerid)
{
    if(pDriveTimer[playerid] > 0 && pDriveExam[playerid] && pDriveType[playerid] != -1) {
        if(!IsPlayerInVehicle(playerid, pDriveVehicle[playerid])) {
            pDriveTimer[playerid] --;
            if(pDriveTimer[playerid] < 1) {
                if(IsValidDynamicRaceCP(pDriveCP[playerid])) DestroyDynamicRaceCP(pDriveCP[playerid]);

                pDriveStatus[playerid] = 0;
                pDriveExam[playerid] = playerData[playerid][player_god] = false;
                pDriveType[playerid] = -1;

                LoadPlayerPosition(playerid);
                DisableRemoteVehicleCollisions(playerid, false);
                showGameDraw(playerid, "~r~~h~NIE WROCILES(AS) DO POJAZDU NA CZAS!~n~~r~~h~EGZAMIN ANULOWANY!");
            }
            else showGameDraw(playerid, form("~r~~h~MASZ ~w~~h~%d ~r~~h~%s BY WROCIC DO POJAZDU!", pDriveTimer[playerid], ConvertTimeFormat(pDriveTimer[playerid], "SEKUND", "SEKUNDE", "SEKUNDY", "SEKUND")), 3);
        }
        else pDriveTimer[playerid] = 0;
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(pDriveExam[playerid] && pDriveType[playerid] != -1) {
        pDriveStatus[playerid] = 0;
        pDriveExam[playerid] = playerData[playerid][player_god] = false;
        pDriveType[playerid] = -1;

        DisableRemoteVehicleCollisions(playerid, false);
        showGameDraw(playerid, "~r~~h~ZGINALES(AS) W CZASIE EGZAMINU!~n~~r~~h~EGZAMIN ANULOWANY!");
    }
    return 1;
}

hook OnPlayerEnterDynRaceCP(playerid, checkpointid)
{
    if(checkpointid == pDriveCP[playerid] && pDriveExam[playerid] && pDriveType[playerid] != -1) {
        if(IsValidDynamicRaceCP(pDriveCP[playerid])) DestroyDynamicRaceCP(pDriveCP[playerid]);
        playSound(playerid, 1139);
    
        pDriveStatus[playerid] ++;
        if(pDriveStatus[playerid] == sizeof(examPositions) - 1) {
            new typeid = pDriveType[playerid];
            pDriveLicense[playerid][typeid] = true;

            pDriveStatus[playerid] = 0;
            pDriveExam[playerid] = playerData[playerid][player_god] = false;
            pDriveType[playerid] = -1;

            achievement(playerid, ACH_SCHOOL);
            LoadPlayerPosition(playerid);
            DisableRemoteVehicleCollisions(playerid, false);
            m_pquery_format("UPDATE es_accounts SET license='%d, %d, %d' WHERE id=%d LIMIT 1;", unpackXYZ(pDriveLicense[playerid]), playerData[playerid][player_accountID]);
            return msgAll(C_CHAT, "Gracz {c}%s(%d){/c} zda³ {c}egzamin{/c} na prawo jazdy {c}%s{/c}!", getNick(playerid), playerid, drivingExamData[typeid][exam_name]), 1;
        }

        new count = pDriveStatus[playerid];
        pDriveCP[playerid] = CreateDynamicRaceCP((count == sizeof(examPositions) - 2) ? (1) : (0), unpackXYZ(examPositions[count]), unpackXYZ(examPositions[count + 1]), 5.0, .worldid=0, .playerid=playerid, .streamdistance=99999.0, .priority=1);
    }
    return 1;
}