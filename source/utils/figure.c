#define MAX_FIGURE    10
new figureCount = 0, figurePickup[MAX_FIGURE], Text3D:figureLabel[MAX_FIGURE], bool:figureActive[MAX_FIGURE];

stock Init_Figure()
{
    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_FIGURE; i++) {
        validFigureLoading:
        GetRandomCoord(x, y, z);
        if(z <= 32.0 || IsPosInWater(x, y, z)) goto validFigureLoading;

        figureActive[i] = true;
        figurePickup[i] = CreateDynamicPickup(1276, 1, x, y, z, 0, 0);

        new str[128]; format(str, sizeof(str), ""GREEN"Figurka "WHITE"#%d\n"LY"WejdŸ w ni¹, by odebraæ nagrodê!", i + 1);
        figureLabel[i] = CreateDynamic3DTextLabel(str, -1, x, y, z, 50.0, .worldid=0, .interiorid=0);

        figureCount ++;
    }
    printf(" >>> [FIGURE]: Wczytanych figurek: %d", figureCount);
    return msgAll(C_GREEN, ""PYTAJNIK""GREEN"Wszystkie {c}figurki{/c} zmieni³y swoje miejsca!");
}

stock GetClosestFigure(pickupid)
{
    new id = -1;
    for(new i = 0; i < figureCount; i++) {
        if(figureActive[i] && pickupid == figurePickup[i]) {
            id = i;
            break;
        }
    }
    return id;
}

stock CollectFigure(playerid, pickupid)
{
    new figureid = GetClosestFigure(pickupid);
    if(figureid != -1 && pickupid == figurePickup[figureid] && figureActive[figureid]) {
        if(!figureActive[figureid]) return 1;

        if(IsValidDynamicPickup(figurePickup[figureid])) DestroyDynamicPickup(figurePickup[figureid]);
        if(IsValidDynamic3DTextLabel(figureLabel[figureid])) DestroyDynamic3DTextLabel(figureLabel[figureid]);

        figurePickup[figureid] = -1;
        figureLabel[figureid] = Text3D:-1;
        figureActive[figureid] = false;

        new money = math_random(10000, 30000);
        giveMoney(playerid, money);
        addLevelScore(playerid, math_random(300, 500));

        s[0] = EOS;
        catstr(s, C_GREEN, -1, "Zebra³eœ(aœ) figurkê {c}#%d{/c}.\n", figureid + 1);
        catstr(s, C_GREEN, -1, "Otrzymujesz z niej: {c}%s${/c}.", strdot(money));

        if(percent(30)) {
            switch(random(3)) {
                case 0: {
                    givePremium(playerid, 3);
                    catstr(s, C_CORAL, -1, "\n\n"CORAL"Otrzymujesz dodatkow¹ nagrodê: {c}Konto Premium na 3 dni{/c}!\n");
                    catstr(s, C_CORAL, -1, "Wiêcej informacji o {c}koñcie Premium{/c} znajdziesz pod: {c}/premium{/c}.");
                }

                case 1: {
                    playerData[playerid][player_levelPoints] += 3;
                    catstr(s, C_CORAL, -1, "\n\n"CORAL"Otrzymujesz dodatkow¹ nagrodê: {c}3 pkt. umiejêtnoœci{/c}!\n");
                    catstr(s, C_CORAL, -1, "\n\n"CORAL"Mo¿esz je {c}rozdysponowaæ{/c} pod: {c}/ulepszenia{/c}.");
                }

                case 2: {
                    playerData[playerid][player_job_points] += 5;
                    catstr(s, C_CORAL, -1, "\n\n"CORAL"Otrzymujesz dodatkow¹ nagrodê: {c}5 pkt.{/c} do pracy!\n");
                    catstr(s, C_CORAL, -1, "\n\n"CORAL"Mo¿esz je {c}rozdysponowaæ{/c} w jakiejkolwiek {c}pracy{/c}.");
                }
            }
        }
        return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Figurka", s, "OK", #);
    }
    return 1;
}
