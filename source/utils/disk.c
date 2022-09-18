#define MAX_DISK    300

new diskCount = 0, bool:diskActive[MAX_DISK], diskPickup[MAX_DISK], Text3D:diskLabel[MAX_DISK], diskReward[MAX_DISK];

stock Init_Disks()
{
    if(diskCount == MAX_DISK - 1) return 1;

    new Float:x, Float:y, Float:z;
    for(new i = 0; i < MAX_DISK; i++) {
        validDiskLoading:
        GetRandomCoord(x, y, z);
        if(z <= 32.0 || IsPosInWater(x, y, z)) goto validDiskLoading;

        diskActive[i] = true;
        diskReward[i] = math_random(100, 500);
        diskPickup[i] = CreateDynamicPickup(1277, 1, x, y, z, 0, 0);

        new str[128]; format(str, sizeof(str), ""BLUE"Dyskietka "WHITE"#%d\n"BLUE"Nagroda: "WHITE"%d pkt. doœwiadczenia", i + 1, diskReward[i]);
        diskLabel[i] = CreateDynamic3DTextLabel(str, -1, x, y, z, 50.0, .worldid=0, .interiorid=0);
    
        diskCount ++;
    }
    printf(" >>> [DISK]: Wczytanych dyskietek: %d", diskCount);
    return msgAll(C_BLUE2, ""PYTAJNIK""BLUE2"Wszystkie {c}dyskietki{/c} zmieni³y swoje miejsca!"), 1;
}

stock GetClosestDisk(pickupid)
{
    new id = -1;
    for(new i = 0; i < MAX_DISK; i++) {
        if(diskActive[i] && pickupid == diskPickup[i]) {
            id = i;
            break;
        } 
    }
    return id;
}

stock CollectDisk(playerid, pickupid)
{
    new diskid = GetClosestDisk(pickupid);
    if(diskid != -1 && pickupid == diskPickup[diskid] && diskActive[diskid]) {
        if(!diskActive[diskid] || diskid == -1 || pickupid != diskPickup[diskid]) return 1;

        if(IsValidDynamicPickup(diskPickup[diskid])) DestroyDynamicPickup(diskPickup[diskid]);
        if(IsValidDynamic3DTextLabel(diskLabel[diskid])) DestroyDynamic3DTextLabel(diskLabel[diskid]);
        
        diskActive[diskid] = false;

        s[0] = EOS;
        catstr(s, C_BLUE, -1, "Zebra³eœ(aœ) dyskietkê {c}#%d{/c}.\n", diskid + 1);
        catstr(s, C_BLUE, -1, "Otrzymujesz z niej: {c}%d pkt. doœwiadczenia{/c}.", diskReward[diskid]);
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE2"Dyskietka", s, "OK", #);
        return addLevelScore(playerid, diskReward[diskid]);
    }
    return 1;
}