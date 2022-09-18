#include YSI_Coding\y_hooks

enum e_businessData {
    business_name[50],
    business_ownerid,
    business_type,
    business_cost,
    Float:business_position[3],
    business_worldid,
    business_reward,
    business_price,
    business_ownername[25],
    bool:business_license,

    business_pickup,
    business_pickupExit,
    Text3D:business_label,
    business_icon,
};

/*
    System biznesów:

    1. Dodaæ opcjê zakupu dodatkowych rzeczy do biznesu:
        - (?) sto³y bilardowe w kasynie;
        - (+) licencje na sprzeda¿ granatów (granat [max: 10], mo³otowy [max: 10]);

    2. Dodaæ opcjê dodawania obiektów w œrodku oraz na zewn¹trz biznesu.
    3. Dodaæ opcjê ustawienia tablicy ko³o budynku biznesowego z dowolnym napisem.
    4. (?) Dodaæ opcjê reklamacji biznesu na billboardach serwera.
*/

new 
    pBusinessTimer[MAX_PLAYERS] = -1,
    
    businessData[MAX_BUSINESS][e_businessData],
    Iterator:Business<MAX_BUSINESS>
;

enum {
    BUSINESS_CASINO = 0,
    BUSINESS_BROTHEL = 1,
    BUSINESS_CLOTHES,
    BUSINESS_GUNSHOP,
};
#define MAX_BUSINESS_TYPES      BUSINESS_GUNSHOP + 1

stock GetBusinessType(typeid) {
    new name[30];
    switch(typeid) {
        case BUSINESS_CASINO:   name = "Kasyno";
        case BUSINESS_BROTHEL:  name = "Burdel";
        case BUSINESS_CLOTHES:  name = "Sklep odzie¿owy";
        case BUSINESS_GUNSHOP:  name = "Sklep z broni¹";
        default:                name = "---";
    }
    return name;
}

stock UpdateBusinessLabel(businessid)
{
    new bstr[256];
    format(bstr, sizeof(bstr), ""CORAL"%s "WHITE"\"%s\"\n", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]);
    
    if(businessData[businessid][business_ownerid] != -1) format(bstr, sizeof(bstr), "%s"LORANGE"W³aœciciel: "WHITE"%s\n", bstr, businessData[businessid][business_ownername]);
    else format(bstr, sizeof(bstr), "%s"LORANGE"Koszt zakupu: "WHITE"%s$\n", bstr, strdot(businessData[businessid][business_cost]));
    
    format(bstr, sizeof(bstr), "%s"LY"Aby zarz¹dzaæ biznesem, kliknij: "WHITE"H", bstr);
    return bstr;
}

stock GetNearestBusiness(playerid)
{
    new id = -1;
    foreach(new i : Business) {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, unpackXYZ(businessData[i][business_position]))) {
            id = i;
            break;
        }
    }
    return id;
}
stock GetBusinessWorld(businessid) return businessData[businessid][business_worldid];

CMD:biznesy(playerid)
{
    s[0] = EOS;
    s = ""ORANGE"Biznes\t"LY"Lokalizacja\n";

    new count = 0;
    foreach(new i : Business) {
        if(businessData[i][business_ownerid] == playerData[playerid][player_accountID]) {
            format(s, sizeof(s), "%s"ORANGE"%s "WHITE"\"%s\"\t"LY"%s, %s\n", s, GetBusinessType(businessData[i][business_type]), businessData[i][business_name], getZoneName(unpackXYZ(businessData[i][business_position])), getCityName(unpackXYZ(businessData[i][business_position])));
            playerData[playerid][player_businessData][count ++] = i;
        }
    }
    if(!count) return dialogBox(playerid, C_RED, "Nie posiadasz ¿adnych biznesów.");
    
    inline onBusinessList(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii
        if(!rr) return 1;

        new businessid = playerData[playerid][player_businessData][ll];   
        Teleport(playerid, false, unpackXYZ(businessData[businessid][business_position]), 0.0, 0, 0, false);
        return msg(playerid, C_BLUE2, "Teleportowa³eœ(aœ) siê pod budynek biznesowy: {c}%s \"%s\"{/c}.", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]), 1;
    }
    return Dialog_ShowCallback(playerid, using inline onBusinessList, DIALOG_STYLE_TABLIST_HEADERS, ""PINK"Posiadane biznesy", s, "Teleportuj", "Zamknij"), 1;
}

stock CountFreeBusiness()
{
    new count = 0;
    foreach(new i : Business) {
        if(businessData[i][business_ownerid] == -1) {
            count ++;
        }
    }
    return count;
}

alias:wolnybiznes("freebusiness", "freebiznes")
CMD:wolnybiznes(playerid)
{
    if(!CountFreeBusiness()) return dialogBox(playerid, C_RED, "Obecnie nie ma ¿adnych wolnych biznesów.");

    findBusiness:
    new businessid = Iter_Random(Business);
    if(businessData[businessid][business_ownerid] != -1) goto findBusiness;
    return msg(playerid, C_CHAT, "Teleportowa³eœ(aœ) siê pod: {c}%s \"%s\"{/c}.", GetBusinessType(businessData[businessid][business_type]), businessData[businessid][business_name]), 1;
}