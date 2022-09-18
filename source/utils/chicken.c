#include YSI_Coding\y_hooks

enum e_KurczakPositions {
    Float:chicken_object[4],
    Float:chicken_spawn[4]
};

new 
    KurczakObject,
    Text3D:KurczakLabel,
    Float:KurczakHealth,
    KurczakReward,
    bool:KurczakEnabled,
    Float:KurczakDamage[MAX_PLAYERS],
    KurczakPositionType,

    KurczakPositions[][e_KurczakPositions] = {
        {{2325.52, 1417.42, 42.73, 180.48},                 {2344.0679, 1493.3666, 42.8203, 101.9965}},
        {{2804.88, -1441.11, 39.04, 0.00},                  {2792.5459, -1463.5669, 40.0625, 322.9258}},
        {{1523.16, 1750.90, 9.79, -39.72},                  {1510.2260, 1720.5848, 10.8203, 94.1112}},
        {{2158.78, 1062.03, 9.81, -15.60},                  {2151.8020, 1050.4854, 10.8301, 156.4197}},
        {{2310.43, 948.01, 9.81, -88.62},                   {2310.7024, 957.3066, 10.8203, 78.8414}}
    }
;

hook function ResetPlayerData(playerid) {
    KurczakDamage[playerid] = 0.0;
    return continue(playerid);
}

stock CreateKurczak(Float:health, money) 
{
    if(KurczakEnabled) return 1;

    if(IsValidDynamicObject(KurczakObject)) DestroyDynamicObject(KurczakObject);
    if(IsValidDynamic3DTextLabel(KurczakLabel)) DestroyDynamic3DTextLabel(KurczakLabel);

    KurczakEnabled = true;
    KurczakHealth = health;
    KurczakReward = money;

    KurczakPositionType = random(sizeof(KurczakPositions));
    KurczakObject = CreateDynamicObject(16776, unpackXYZ(KurczakPositions[KurczakPositionType][chicken_object]), 0.0, 0.0, KurczakPositions[KurczakPositionType][chicken_object][3], .worldid=0, .interiorid=0);
    KurczakLabel = CreateDynamic3DTextLabel(" ", -1, unpackXYZ(KurczakPositions[KurczakPositionType][chicken_object]) + 0.30, 30.0, .worldid=0, .interiorid=0);

    new health_str[50];
    switch(KurczakHealth) {
        case 10000..15000:  health_str = "{73ff77}||||||{363636}";
        case 7000..9999:    health_str = "{abff73}|||||{363636}|";
        case 5000..6999:    health_str = "{d7ff73}||||{363636}||";
        case 3000..4999:    health_str = "{ffe373}|||{363636}|||";
        case 1000..2999:    health_str = "{ff9f73}||{363636}||||";
        default:            health_str = "{ff7373}|{363636}|||||";
    }

    msgAll(-1, " ");
    msgAll(C_LRED, "Na mapie pojawi³ siê {c}kurczak{/c}, którego ¿ycie wynosi: {c}%.2f HP{/c}.", health);
    msgAll(C_LRED, "Za jego zabójstwo, ka¿dy w pobli¿u otrzymuje: {c}%s${/c}.", strdot(money));

    new kstr[128]; format(kstr, sizeof(kstr), ""YELLOW"Kurczak Robinson\n"LY"Strzelaj w niego, by zdobyæ nagrodê!\n%s", health_str);
    return UpdateDynamic3DTextLabelText(KurczakLabel, -1, kstr), 1;
}

stock DestroyKurczak() 
{
    if(!KurczakEnabled) return 1;
    if(IsValidDynamicObject(KurczakObject)) DestroyDynamicObject(KurczakObject);
    if(IsValidDynamic3DTextLabel(KurczakLabel)) DestroyDynamic3DTextLabel(KurczakLabel);

    KurczakEnabled = false;
    KurczakReward = 0;
    KurczakHealth = 0.0;
    return 1;
}

hook OnPlayerShootDynObject(playerid, weaponid, STREAMER_TAG_OBJECT:objectid, Float:x, Float:y, Float:z)
{
    if(objectid == KurczakObject && KurczakEnabled) {
        new Float:damage = math_random_float(1.0, 4.0);
        KurczakHealth -= damage;
        KurczakDamage[playerid] += damage;

        if(KurczakHealth <= 0.0) {
            if(KurczakDamage[playerid] > 0) {
                achievement(playerid, ACH_KURCZAK);
                giveMoney(playerid, KurczakReward);

                msg(playerid, C_ORANGE, "Zabi³eœ(aœ) kurczaka - nagroda za ubicie: {c}%s${/c}.", strdot(KurczakReward));
                msg(playerid, C_ORANGE, "Zada³eœ(aœ) mu ³¹cznie: {c}%.2f{/c} pkt. obra¿eñ!", KurczakDamage[playerid]);

                KurczakDamage[playerid] = 0;
            }
            DestroyKurczak();
        }
    }
    return 1;
}

CMD:kurczaktp(playerid)
{
    if(!KurczakEnabled) return sendError(playerid, ""RED"Obecnie kurczak nie jest aktywowany!");
    return Teleport(playerid, false, unpackXYZR(KurczakPositions[KurczakPositionType][chicken_spawn]), 0, 0, false), 1;
}
