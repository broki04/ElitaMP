public OnGameModeInit()
{
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(false);
    DisableNameTagLOS();

    Streamer_SetVisibleItems(0, 1000);

    EnableAntiCheat(0, 1); EnableAntiCheat(1, 1); EnableAntiCheat(2, 1); EnableAntiCheat(3, 1); EnableAntiCheat(4, 0);
    EnableAntiCheat(5, 0); EnableAntiCheat(6, 0); EnableAntiCheat(7, 1); EnableAntiCheat(8, 1); EnableAntiCheat(9, 1);
    EnableAntiCheat(10, 1); EnableAntiCheat(11, 0); EnableAntiCheat(15, 1); EnableAntiCheat(17, 0); EnableAntiCheat(19, 1);
    EnableAntiCheat(23, 1); EnableAntiCheat(26, 1); EnableAntiCheat(27, 0); EnableAntiCheat(28, 1); EnableAntiCheat(29, 1);
    EnableAntiCheat(31, 1); EnableAntiCheat(32, 1); EnableAntiCheat(35, 1); EnableAntiCheat(40, 0); EnableAntiCheat(42, 1);
    EnableAntiCheat(39, 0); EnableAntiCheat(42, 0); EnableAntiCheat(43, 1); EnableAntiCheat(44, 1); EnableAntiCheat(45, 1);
    EnableAntiCheat(46, 1); EnableAntiCheat(47, 1); EnableAntiCheat(48, 0); EnableAntiCheat(50, 1); EnableAntiCheat(51, 1); 
    EnableAntiCheat(52, 1);

    EnableAntiNOP(0, 0); EnableAntiNOP(1, 0); EnableAntiNOP(2, 0); EnableAntiNOP(3, 0);
    EnableAntiNOP(4, 0); EnableAntiNOP(5, 0); EnableAntiNOP(6, 0); EnableAntiNOP(7, 0);
    EnableAntiNOP(8, 1); EnableAntiNOP(9, 1); EnableAntiNOP(10, 0); EnableAntiNOP(11, 0);

    SetVehiclePassengerDamage(true);
    SetDisableSyncBugs(true);
    SetDamageSounds(6401, 17802);

    CA_Init();
    Init_MySQL();
    return 1;
}

public OnGameModeExit()
{
    return mysql_close(dbconn);
}

stock Init_MySQL()
{
    new MySQLOpt:optionid = mysql_init_options();
    mysql_set_option(optionid, AUTO_RECONNECT, true);

    //dbconn = mysql_connect("127.0.0.1", "root", "", "elita", optionid);
    dbconn = mysql_connect("51.83.217.86", "k16961_s26970", "d68d24308cd9", "db_26970", optionid);
    if(dbconn == MYSQL_INVALID_HANDLE || mysql_errno(dbconn) != 0) {
        print(" >>> [MYSQL]: Nie po³¹czono siê z baz¹ danych - wy³¹czanie serwera...");
        SendRconCommand("exit");
    }
    else Init_Map();
    return 1;
}

stock Init_Map()
{
    repeat SecondTimer();
    repeat MinuteTimer();

    CreateDesign();
    RunObjects();

    Iter_Init(Permission);
    Iter_Init(Premium);
    Iter_Init(Vehicles);

    for(new i = 0; i <= 311; i++) AddPlayerClass(i, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);

    // pomniki kurwa
    CreateDynamicObject(817, 2163.79, 1073.11, 10.10,   0.00, 0.00, 0.00, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamicObject(325, 2163.74, 1073.79, 12.05,   -33.90, 90.96, -24.00, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);

    CreateDynamicObject(14467, 2164.00, 1073.19, 12.45, 0.00, 0.00, -68.22, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamic3DTextLabel(""GOLD"Najwiêkszy jebacz "YELLOW"Szyneczki\n"BROWN"carbonara", -1, 2164.00, 1073.19, 12.45, 100.0, .worldid=0);

    CreateDynamicObject(14467, 2159.85, 1075.16, 12.45, 0.00, 0.00, 1.08, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamic3DTextLabel(""GOLD"Podrywacz "PINK"dziewczyn SA-MP\n"VIOLET"roke", -1, 2159.85, 1075.16, 12.45, 100.0, .worldid=0);

    CreateDynamicObject(14467, 2163.50, 1077.84, 12.45,   0.00, 0.00, -24.60, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamic3DTextLabel(""GOLD"Wielki jebacz "PINK"Mysiów Pysiów\n{388EEA}uhqqow", -1, 2163.50, 1077.84, 12.45, 100.0, .worldid=0);

    CreateDynamicObject(14467, 2157.74, 1073.87, 12.45,   0.00, 0.00, 40.08, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamic3DTextLabel(""GOLD"Paryski napierdalaka "YELLOW"Elity\n"GREEN"Lou", -1, 2157.74, 1073.87, 12.45, 100.0, .worldid=0);

    CreateDynamicObject(14467, 2155.39819, 1070.60742, 12.44351,   0.00000, 0.00000, 9.71997, .worldid=0, .streamdistance=100.0, .drawdistance=100.0);
    CreateDynamic3DTextLabel(""GOLD"Pierwsza dama "YELLOW"Elity\n"PINK"Rokitka", -1, 2155.39819, 1070.60742, 12.44351, 100.0, .worldid=0);

    m_pquery_format("UPDATE es_accounts SET bonus_vcxz=0");

    // admin actors
    inline onFetchAdmins() 
    {
        new astr[128], nick[25], admin, skin, Float:pos[4];
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name(i, "nick", nick);
            cache_get_value_name_int(i, "admin", admin);
            cache_get_value_name_int(i, "skin", skin);

            pos[0] = 2114.8857 - 1.5 - i;
            pos[1] = 986.1318;
            pos[2] = 10.8301;
            pos[3] = 0.4427;

            format(astr, sizeof(astr), "{%06x}%s\n"WHITE"%s", getRankColor(admin) >>> 8, getRankName(admin), nick);
            CreateDynamic3DTextLabel(astr, -1, unpackXYZ(pos) + 0.50, 30.0, .worldid=0, .interiorid=0);
            CreateDynamicActor(skin, unpackXYZR(pos), .worldid=0, .interiorid=0);
        }
    }
    m_tquery_inline(dbconn, using inline onFetchAdmins, "SELECT nick, admin, skin FROM es_accounts WHERE admin > 0 ORDER BY admin DESC");

    // 
    CreateDynamicActor(16, 440.1974, -846.5050, 29.8050, 209.4823, .worldid=0, .interiorid=0);
    CreateDynamic3DTextLabel(""BROWN"Górnik "WHITE"Steve\n"LY"Tutaj sprzedasz swoje ¿y³y!\n"LY"Aby sprzedaæ, kliknij: "WHITE"Y", -1, 440.1974, -846.5050, 29.8050 + 0.25, 30.0, .worldid=0, .interiorid=0);

    OreDay = -1;

    // initialization
    Init_Teleports();
    Init_Gangs();
    Init_AdminTop();
    Init_Jobs();
    Init_Areny();
    Init_Quiz();
    Init_PrivCars();
    Init_Stations();
    Init_Bank();
    Init_Disks();
    Init_Figure();
    Init_Gielda();
    Init_Games();
    Init_Fishing();
    Init_Driving();
    Init_Records();
    Init_NoDM();
    Init_Mining();
    Init_GangBases();
    Init_Vehicles();
    Init_Truck();
    Init_Business();
    return 1;    
}
