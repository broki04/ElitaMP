#include YSI_Coding\y_hooks

new 
    bankHeistPickup, 
    bool:bankHeistEnabled,
    Text3D:bankHeistLabel,
    bankHeistVault,
    bankHeistExit,
    bankHeistCheckpoint,
    bool:bankHeistVaultOpen,
    Text3D:bankHeistVaultLabel,
    bankHeistCode[10],
    heistRobbed,
    bool:bankHeistGas,
    bankHeistTimer,
    bankHeistCountDown,
    bankHeistHacker
;

new const Float:vaultPositionData[][] = {
    {0.0, 0.0, 0.0, 0.0}, // bo siê kruwa buguje coœ
    {2147.1655, 1629.3208, 993.5761, 270.4773},
    {2147.1655, 1633.2552, 993.5761, 268.8688},
    {2147.1655, 1637.4115, 993.5761, 268.8688},
    {2147.1655, 1641.2139, 993.5761, 270.9368},
    {2141.2017, 1641.1639, 993.5761, 90.8545},
    {2141.2024, 1637.2080, 993.5761, 90.8545},
    {2141.2048, 1633.1904, 993.5761, 90.8545},
    {2141.2017, 1629.2869, 993.5761, 90.8545}
};

enum e_vaultHeistData {
    bool:vault_robbed,
    vault_gas,
    Text3D:vault_label,
    vault_reward
};
new vaultHeistData[sizeof(vaultPositionData)][e_vaultHeistData];

stock Heist_Init()
{
    heistRobbed = 0;
    bankHeistEnabled = bankHeistVaultOpen = bankHeistGas = false;

    bankHeistExit = CreateDynamicPickup(19132, 1, 2148.2761, 1604.2834, 1001.9677, .worldid=VW_HEIST, .interiorid=1);
    CreateDynamic3DTextLabel(""YELLOW"PREMIUM "GREEN"BANK\n"WHITE"Powrót do banku", -1, 2148.2761, 1604.2834, 1001.9677 + 0.15, 30.0, .worldid=VW_HEIST, .interiorid=1);

    bankHeistPickup = CreateDynamicPickup(19132, 1, 1390.4954, 1318.5093, 1498.1134, .worldid=VW_BANK, .interiorid=VW_BANK);
    bankHeistLabel = CreateDynamic3DTextLabel(""ORANGE"WEJŒCIE DO SKARBCA\n"WHITE"Aktualnie "RED"nie mo¿na "WHITE"wejœæ!", -1, 1390.4954, 1318.5093, 1498.1134 + 0.15, 30.0, .worldid=VW_BANK, .interiorid=VW_BANK);

    bankHeistCheckpoint = CreateDynamicCP(2145.6897, 1626.4923, 993.6882, 1.0, .worldid=VW_HEIST, .interiorid=1);
    bankHeistVaultLabel = CreateDynamic3DTextLabel(""ORANGE"SKARBIEC\n"WHITE"Skarbiec "RED"nie jest "WHITE"otwarty.\n"WHITE"Aby otworzyæ, wejdŸ w "RED"marker"WHITE".", -1, 2145.6897, 1626.4923, 993.6882 + 1.0, 30.0, .worldid=VW_HEIST, .interiorid=1);

    bankHeistVault = CreateDynamicObject(19799, 2143.186, 1626.984, 994.243, 0.000, 0.000, 180.000, VW_HEIST, 1, -1, 1000.0);
	CreateDynamicObject(2904, 2148.304, 1604.651, 1002.262, 0.000, 0.000, 0.000, VW_HEIST, 1, -1, 1000.0);
	CreateDynamicObject(2904, 2148.304, 1604.651, 1004.882, 0.000, 0.000, 0.000, VW_HEIST, 1, -1, 1000.0);
	CreateDynamicObject(2904, 2148.304, 1604.651, 1007.502, 0.000, 0.000, 0.000, VW_HEIST, 1, -1, 1000.0);
	CreateDynamicObject(2904, 2149.989, 1602.867, 1004.891, 0.000, 0.000, 90.000, VW_HEIST, 1, -1, 1000.0);
	CreateDynamicObject(2904, 2149.989, 1602.867, 1002.271, 0.000, 0.000, 90.000, VW_HEIST, 1, -1, 1000.0);

    RandomVaultCode();
}

stock RandomVaultReward()
{
    heistRobbed = 0;
    for(new i = 1; i < sizeof(vaultPositionData); i++) {
        if(IsValidDynamic3DTextLabel(vaultHeistData[i][vault_label])) DestroyDynamic3DTextLabel(vaultHeistData[i][vault_label]);

        vaultHeistData[i][vault_robbed] = false;
        vaultHeistData[i][vault_reward] = math_random(50000, 250000);

        new vstr[256]; format(vstr, sizeof(vstr), ""ORANGE"SEJF "LORANGE"#%d\n"WHITE"PodejdŸ i kliknij "BLUE"H"WHITE", by zacz¹æ okradaæ!\n"WHITE"Pozosta³o: "LBLUE"%s$", i, strdot(vaultHeistData[i][vault_reward]));
        vaultHeistData[i][vault_label] = CreateDynamic3DTextLabel(vstr, -1, unpackXYZ(vaultPositionData[i]), 30.0, .worldid=VW_HEIST, .interiorid=1);
        vaultHeistData[i][vault_gas] = CreateDynamicObject(18748, unpackXYZ(vaultPositionData[i]) + 100.0, 0.000, 0.000, 90.000, VW_HEIST, 1, -1, 1000.0);
    }
    return 1;
}

stock RandomVaultCode()
{
    static const bankCodeLetters[] = {
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'O', 'P', 'A', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 'd', 'f', 'g', 'h', 'j', 'k', 's', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '!', '@', '#', '.', ',', '^', '$', '&', '*', '/', '[', ']', '(', ')' 
    };

    for(new i = 0; i != 10; i++) format(bankHeistCode[i], 2, "%c", bankCodeLetters[random(sizeof(bankCodeLetters))]);
    return printf(" >>> [HEIST]: Kod do sejfu to: %s", bankHeistCode), 1;
}

stock Heist_Pickup(playerid, pickupid)
{
    if(pickupid == bankHeistPickup && bankHeistEnabled) {
        if(bankHeistHacker != -1 && bankHeistVaultOpen) msg(playerid, C_ORANGE, "Skarbiec zosta³ otwarty przez {c}%s(%d){/c}. Do zamkniêcia pozosta³o: {c}%s{/c}.", getNick(bankHeistHacker), bankHeistHacker, ConvertTime(bankHeistCountDown, TIME_TYPE_MS));
        return Teleport(playerid, false, 2147.7100, 1597.3134, 999.7676, 85.2769, 1, VW_HEIST);
    }
    
    if(pickupid == bankHeistExit) {
        if(playerData[playerid][player_robbed] > 0) {
            giveMoney(playerid, playerData[playerid][player_robbed]);
            
            msg(playerid, C_MINT, "Zdo³a³eœ(aœ) wykraœæ ze skarbca: {c}%s${/c}.", strdot(playerData[playerid][player_robbed]));
            playerData[playerid][player_robbed] = 0;

            if(CountPlayerHeist() < 1) {
                for(new i = 1; i < sizeof(vaultPositionData); i++) { if(IsValidDynamic3DTextLabel(vaultHeistData[i][vault_label])) DestroyDynamic3DTextLabel(vaultHeistData[i][vault_label]); }

                bankHeistEnabled = bankHeistVaultOpen = false;
                UpdateDynamic3DTextLabelText(bankHeistVaultLabel, -1, ""ORANGE"SKARBIEC\n"WHITE"Skarbiec "RED"nie jest "WHITE"otwarty.\n"WHITE"Aby otworzyæ, wejdŸ w "RED"marker"WHITE".");
                UpdateDynamic3DTextLabelText(bankHeistLabel, -1, ""ORANGE"WEJŒCIE DO SKARBCA\n"WHITE"Aktualnie "RED"nie mo¿na "WHITE"wejœæ!");

                MoveDynamicObject(bankHeistVault, 2143.186, 1626.984, 994.243, 0.050, 0.000, 0.000, 180.000);
                msgAll(C_LGREEN, "Skarbiec zosta³ {c}ca³kowicie{/c} zamkniêty! Wykradziono ³¹cznie: {c}%s${/c}.", strdot(heistRobbed));
                KillTimer(bankHeistTimer);
            }
        }
        return Teleport(playerid, false, 1390.4954, 1318.5093, 1498.1134, 359.1097, VW_BANK, VW_BANK);
    }
    return 1;
}

stock ShowVaultDialog(playerid, const string[])
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Próbujesz aktualnie "BLUE"w³amaæ"WHITE" siê do sejfu.\n"WHITE"Musisz poni¿ej przepisaæ kod, który zosta³ podany ni¿ej:\n"LY"%s", bankHeistCode);
    if(strlen(string) > 2) format(s, sizeof(s), "%s\n\n%s", s, string);
    return showDialog(playerid, DIALOG_HEIST_VAULT, DIALOG_STYLE_INPUT, ""BLUE"Próba w³amania do sejfu", s, "Hackuj", "Anuluj"), 1;
}

stock CheckVaultNear(playerid)
{
    new id = -1;
    for(new i = 1; i < sizeof(vaultPositionData); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, unpackXYZ(vaultPositionData[i]))) {
            id = i;
            break;
        }
    }
    return id;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new vaultid = CheckVaultNear(playerid);
    if(vaultid != -1 && PRESSED(KEY_CTRL_BACK) && GetPlayerVirtualWorld(playerid) == VW_HEIST && bankHeistEnabled && bankHeistVaultOpen && !vaultHeistData[vaultid][vault_robbed] && vaultHeistData[vaultid][vault_reward] != 0) {
        vaultHeistData[vaultid][vault_robbed] = true;
        ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.0, true, false, false, true, 0, 0);
        SetTimerEx("OnPlayerHeist", 1000 * math_random(3, 7), false, "dd", playerid, vaultid);
    }
    return 1;
}

function OnPlayerHeist(playerid, vaultid)
{
    if(!bankHeistEnabled || !bankHeistVaultOpen) return 1;
    ClearAnimations(playerid);

    new money = floatround(vaultHeistData[vaultid][vault_reward] * ((vaultHeistData[vaultid][vault_reward] < 10000) ? (1.00) : (0.35)));
    vaultHeistData[vaultid][vault_reward] -= money;
    vaultHeistData[vaultid][vault_robbed] = false;
    heistRobbed += money;

    playerData[playerid][player_robbed] += money;
    achievement(playerid, ACH_HEIST);

    if(!vaultHeistData[vaultid][vault_reward]) vaultHeistData[vaultid][vault_reward] = 0;

    new vstr[256]; 
    if(!vaultHeistData[vaultid][vault_reward]) format(vstr, sizeof(vstr), ""ORANGE"SEJF "LORANGE"#%d\n"WHITE"Sejf zosta³ "RED"w ca³oœci "WHITE"obrabowany.", vaultid);
    else format(vstr, sizeof(vstr), ""ORANGE"SEJF "LORANGE"#%d\n"WHITE"PodejdŸ i kliknij "BLUE"H"WHITE", by zacz¹æ okradaæ!\n"WHITE"Pozosta³o: "LBLUE"%s$", vaultid, strdot(vaultHeistData[vaultid][vault_reward]));
    UpdateDynamic3DTextLabelText(vaultHeistData[vaultid][vault_label], -1, vstr);

    s[0] = EOS;
    catstr(s, C_FAJNY, -1, "Przeszuka³eœ(aœ) sejf {c}#%d{/c}.\n", vaultid + 1);
    catstr(s, C_FAJNY, -1, "Wykrad³eœ(aœ) z niego: {c}%s${/c}.\n\n", strdot(money));
    catstr(s, C_DRED, -1, "£¹czny wykradziony ³up: {c}%s${/c}.", strdot(playerData[playerid][player_robbed]));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Sejf przeszukany", s, "OK", #), 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(checkpointid == bankHeistCheckpoint && !bankHeistVaultOpen) {
        if(gettime() < GetPVarInt(playerid, "bank_heist_time")) return dialogBox(playerid, C_RED, ""RED"Wyczerpa³eœ(aœ) próby na przepisanie kodu! Spróbuj ponownie za: "WHITE"%s"RED".", ConvertTime(GetPVarInt(playerid, "bank_heist_time"), TIME_TYPE_UNIX));
        return ShowVaultDialog(playerid, "");
    }
    return 1;
}

stock OnDialogHeist(playerid, dialogid, response, inputtext[])
{
    switch(dialogid) {
        case DIALOG_HEIST_VAULT: {
            if(!response || !bankHeistEnabled || bankHeistVaultOpen) return 1;

            if(!strmatch(bankHeistCode, inputtext, true)) {
                playerData[playerid][player_heist_attempt] ++;
                if(playerData[playerid][player_heist_attempt] >= 3) {
                    dialogBox(playerid, C_RED, ""RED"Wyczerpa³eœ(aœ) próby na przepisanie kodu! Spróbuj ponownie za "WHITE"minutê"RED".");
                    SetPVarInt(playerid, "bank_heist_time", gettime() + 60);
                }
                else ShowVaultDialog(playerid, form(""RED"Przepisa³eœ(aœ) Ÿle kod! Próby: "WHITE"%d/3", playerData[playerid][player_heist_attempt]));
                return 1;
            }

            bankHeistVaultOpen = true;
			MoveDynamicObject(bankHeistVault, 2143.186, 1626.984, 994.243, 0.050, 0.000, 0.000, 110.000);

            new time = math_random(1, 3);
            msgAll(C_LGREEN, "Wejœcie do skarbca zosta³o zhackowane przez: {c}%s(%d){/c}.", getNick(playerid), playerid);
            msgAll(C_LGREEN, "Po up³ywie {c}%d minut{/c}, skarbiec zostanie {c}zagazowany{/c}.", time);
            
            bankHeistHacker = playerid;
            bankHeistCountDown = time * 60;
            bankHeistTimer = SetTimer("VaultHeistTimer", 1000, true);

            RandomVaultReward();
            return UpdateDynamic3DTextLabelText(bankHeistVaultLabel, -1, ""ORANGE"SKARBIEC\n"WHITE"Skarbiec "GREEN"zosta³ "WHITE"otwarty."), 1;
        }
    }
    return 0;
}

new bool:heist_kurwa = false;
function VaultHeistTimer()
{
    bankHeistCountDown --;
    new vstr[256]; format(vstr, sizeof(vstr), ""ORANGE"SKARBIEC\n"WHITE"Skarbiec "GREEN"zosta³ "WHITE"otwarty.\n\n"WHITE"Otwarte przez: "LY"%s(%d)\n"WHITE"Pozosta³o "RED"%s "WHITE"do zamkniêcia skarbca!", getNick(bankHeistHacker), bankHeistHacker, ConvertTime(bankHeistCountDown, TIME_TYPE_MS));
    UpdateDynamic3DTextLabelText(bankHeistVaultLabel, -1, vstr);

    if(bankHeistCountDown < 1) {
        for(new i = 1; i < sizeof(vaultPositionData); i++) { if(IsValidDynamic3DTextLabel(vaultHeistData[i][vault_label])) DestroyDynamic3DTextLabel(vaultHeistData[i][vault_label]); }

        bankHeistEnabled = bankHeistVaultOpen = false;
        UpdateDynamic3DTextLabelText(bankHeistVaultLabel, -1, ""ORANGE"SKARBIEC\n"WHITE"Skarbiec "RED"nie jest "WHITE"otwarty.\n"WHITE"Aby otworzyæ, wejdŸ w "RED"marker"WHITE".");
        UpdateDynamic3DTextLabelText(bankHeistLabel, -1, ""ORANGE"WEJŒCIE DO SKARBCA\n"WHITE"Aktualnie "RED"nie mo¿na "WHITE"wejœæ!");

        MoveDynamicObject(bankHeistVault, 2143.186, 1626.984, 994.243, 0.050, 0.000, 0.000, 180.000);
        msgAll(C_LGREEN, "Skarbiec zosta³ {c}ca³kowicie{/c} zamkniêty! Wykradziono ³¹cznie: {c}%s${/c}.", strdot(heistRobbed));
        return KillTimer(bankHeistTimer);
    }

    if(bankHeistCountDown < 11 && !heist_kurwa) {
        bankHeistGas = heist_kurwa = true;
        foreach(new playerid : Player) {
            if(GetPlayerVirtualWorld(playerid) == VW_HEIST && IsPlayerInArea(playerid, 2140.753, 1627.333, 2147.677, 1643.257) && bankHeistVaultOpen) {
                setOdliczanie(playerid, 10);
            }
        }

        for(new i = 0; i < sizeof(vaultPositionData); i++) MoveDynamicObject(vaultHeistData[i][vault_gas], unpackXYZ(vaultPositionData[i]) + 1.0, 1000.0, 0.0, 0.0, 90.0);
        msgAll(C_LGREEN, "Skarbiec zostanie {c}zamkniêty{/c} za {c}10 sekund{/c}! Zaczyna {c}ulatniaæ siê{/c} gaz.");
    }
    return 1;
}

stock CheckHeistVault(playerid)
{
    if(IsPlayerInArea(playerid, 2140.753, 1627.333, 2147.677, 1643.257)) {
        if(bankHeistGas && GetPlayerVirtualWorld(playerid) == VW_HEIST) {
            new Float:health; GetPlayerHealth(playerid, health);
            SetPlayerHealth(playerid, health - 5.0);

            if(!playerData[playerid][player_vault]) playerData[playerid][player_vault] = true;
        }
    }
    else playerData[playerid][player_vault] = false;
}

stock CountPlayerHeist()
{
    new count = 0;
    foreach(new i : Player) {
        if(GetPlayerVirtualWorld(i) == VW_HEIST && IsPlayerInArea(i, 2140.753, 1627.333, 2147.677, 1643.257)) {
            count ++;
        }
    }
    return count;
}