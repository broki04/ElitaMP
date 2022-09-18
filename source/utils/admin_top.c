stock Init_AdminTop() 
{
    inline onAdminTop()
    {
        new userid, nick[25], admin_points, admin_online, event_points, event_count;
        if(cache_num_rows() > 0) {
            cache_get_value_name_int(0, "id", userid);
            cache_get_value_name(0, "nick", nick, 25);
            cache_get_value_name_int(0, "admin_points", admin_points);
            cache_get_value_name_int(0, "admin_online", admin_online);

            format(adminWeekName, 25, "%s", nick);

            inline onEventTop() 
            {
                cache_get_value_name_int(0, "id", userid);
                cache_get_value_name(0, "nick", nick, 25);

                cache_get_value_name_int(0, "event_points", event_points);
                cache_get_value_name_int(0, "event_count", event_count);

                format(eventWeekName, 25, "%s", nick);

                s[0] = EOS;
                format(s, sizeof(s), "~w~~h~Najlepszy admin tygodnia: ~r~~h~%s ~w~~h~(%s pkt, %02dh %02d min)~n~", adminWeekName, strdot(admin_points), floatround(((admin_online) / 3600)), floatround(((admin_online) / 60) % 60));
                format(s, sizeof(s), "%s~w~~h~Najczesciej robiacy eventy: ~y~~h~%s ~w~~h~(%s pkt, %d ev)~n~", s, eventWeekName, strdot(event_points), event_count);
                TextDrawSetString(tdWelcome[3], s);

                printf(" >>> [WEEK-TOP]: Najlepszy admin: %s", adminWeekName);
                printf(" >>> [WEEK-TOP]: Najlepszy eventowicz: %s", eventWeekName);
            }
            m_pquery_inline(dbconn, using inline onEventTop, "SELECT * FROM es_accounts WHERE admin > 0 ORDER BY event_points DESC LIMIT 1;");
        }
    }
    m_pquery_inline(dbconn, using inline onAdminTop, "SELECT * FROM es_accounts WHERE admin > 0 ORDER BY admin_points DESC LIMIT 1;");
    return 1;
}