#define CHAT_TYPE_GLOBAL            0
#define CHAT_TYPE_LOCAL             1
#define CHAT_TYPE_GANGS             2

stock getChatType(typeid)
{
    new name[25];
    switch(typeid) {
        case CHAT_TYPE_GLOBAL:  name = "Globalny";
        case CHAT_TYPE_LOCAL:   name = "Lokalny";
        case CHAT_TYPE_GANGS:   name = "Gangowy";
    }
    return name;
}

stock SendGlobalMessage(color, text[])
{
    new limit[2][144];
    if(strlen(text) > 144) {
        strmid(limit[0], text, 0, 139);
        format(limit[0], 144, "%s", limit[0]);

        strmid(limit[1], text, 139, strlen(text));
        format(limit[1], 144, "%s", limit[1]);

        SendClientMessageToAll(color, limit[0]);
        SendClientMessageToAll(color, limit[1]);
    }
    else SendClientMessageToAll(color, text);
	return 1;
}

stock SendLocalMessage(playerid, color, text[])
{   
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new limit[2][144];
    foreach(new i : Player) {
        if(IsPlayerInRangeOfPoint(i, 30.0, x, y, z)) {
            if(strlen(text) > 144) {
                strmid(limit[0], text, 0, 139);
                format(limit[0], 144, "%s", limit[0]);

                strmid(limit[1], text, 139, strlen(text));
                format(limit[1], 144, "%s", limit[1]);

                SendClientMessage(i, color, limit[0]);
                SendClientMessage(i, color, limit[1]);
            }
            else SendClientMessage(i, color, text);
        }
    }
    return 1;
}

alias:local("l")
CMD:local(playerid, params[])
{
    if(isnull(params) || strlen(params) < 3 || strlen(params) > 35) return syntaxError(playerid, "/local [wiadomoœæ]\n\n"LY"Mo¿esz równie¿ prze³¹czyæ tryb czatowania na "WHITE"lokalny "LY"w "LY"/konto"WHITE".");

    new string[255];
    format(string, sizeof(string), "{AFAFAF}[local]: {858585}%s'%d: {AFAFAF}%s", getNick(playerid), playerid, params);
    return SendLocalMessage(playerid, -1, string), 1;
}