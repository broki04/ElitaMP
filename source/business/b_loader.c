stock Init_Business()
{
    Iter_Init(Business);

    inline onBusinessLoad() {
        if(!cache_num_rows()) return 1;
        
        new id = -1;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", id);
            CreateBusiness(id, i, false);
        }    
        return printf(" >>> [BIZNES]: Wczytano %d biznesów", Iter_Count(Business)), 1;
    }

    m_pquery_inline(dbconn, using inline onBusinessLoad, "SELECT * FROM es_business LIMIT %d;", MAX_BUSINESS);
    return 1;
}

stock CreateBusiness(businessid, loopid, bool:query = false)
{
    new Cache:result;
    if(query) result = mysql_query(dbconn, form("SELECT * FROM es_business WHERE id=%d LIMIT 1;", businessid));

    cache_get_value_name(loopid, "name", businessData[businessid][business_name]);
    cache_get_value_name_int(loopid, "ownerid", businessData[businessid][business_ownerid]);
    cache_get_value_name_int(loopid, "type", businessData[businessid][business_type]);
    cache_get_value_name_int(loopid, "price", businessData[businessid][business_price]);
    cache_get_value_name_int(loopid, "cost", businessData[businessid][business_cost]);
    cache_get_value_name_int(loopid, "reward", businessData[businessid][business_reward]);
    cache_get_value_name_bool(loopid, "gun_bonus", businessData[businessid][business_license]);

    new buffer[90];
    cache_get_value_name(loopid, "position", buffer);
    sscanf(buffer, "p<,>a<f>[3]", businessData[businessid][business_position]);

    //

    dbstr[0] = EOS;
    format(dbstr, sizeof(dbstr), "SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", businessData[businessid][business_ownerid]);
    m_pquery(dbstr, "FetchBusinessName", "d", businessid);

    Iter_Add(Business, businessid);
    return (query) ? (cache_delete(result)) : (1);
}

function FetchBusinessName(businessid)
{
    if(cache_num_rows() > 0) cache_get_value(0, 0, businessData[businessid][business_ownername], 25);
    
    if(IsValidDynamicPickup(businessData[businessid][business_pickup])) DestroyDynamicPickup(businessData[businessid][business_pickup]);
    if(IsValidDynamic3DTextLabel(businessData[businessid][business_label])) DestroyDynamic3DTextLabel(businessData[businessid][business_label]);
    if(IsValidDynamicMapIcon(businessData[businessid][business_icon])) DestroyDynamicMapIcon(businessData[businessid][business_icon]);

    businessData[businessid][business_worldid] = VW_BUSINESS + 200 + businessid;
    businessData[businessid][business_pickup] = CreateDynamicPickup(1318, 1, unpackXYZ(businessData[businessid][business_position]), 0, 0);
    businessData[businessid][business_label] = CreateDynamic3DTextLabel(UpdateBusinessLabel(businessid), -1, unpackXYZ(businessData[businessid][business_position]) + 0.25, 30.0, .worldid=0, .interiorid=0);

    switch(businessData[businessid][business_type]) {
        case BUSINESS_CASINO: FetchCasino(businessid);
        case BUSINESS_GUNSHOP: FetchGunShop(businessid);
    }
    return 1;
}