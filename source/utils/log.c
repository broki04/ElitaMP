enum {
    LOG_CHAT = 0,
    LOG_DEBUG = 1,
    LOG_PUNISH,
    LOG_CMD
};

stock Log(type, const string[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), string, va_start<2>);
	return WriteToLog(type, out), 1;
}

stock WriteToLog(type, const string[])
{
    new hour, minute, second, day, month, year;
    getdate(year, month, day), gettime(hour, minute, second);

    new s_date[32], s_file[32];
    format(s_date, sizeof(s_date), "[%02d:%02d:%02d]", hour, minute, second);

    switch(type) {
        case LOG_CHAT: format(s_file, sizeof(s_file), "logs/chat/%02d.%02d.%d.txt", day, month, year);
        case LOG_DEBUG: format(s_file, sizeof(s_file), "logs/debug/%02d.%02d.%d.txt", day, month, year);
        case LOG_PUNISH: format(s_file, sizeof(s_file), "logs/punish/%02d.%02d.%d.txt", day, month, year);
        case LOG_CMD: format(s_file, sizeof(s_file), "logs/cmd/%02d.%02d.%d.txt", day, month, year);
    }

    new out[512]; format(out, sizeof(out), string);
    strins(out, s_date, 0);
    strins(out, "\r\n", strlen(out));

    if(!fexist(s_file)) {
        new File:file = fopen(s_file, io_readwrite);
        fwrite(file, ""), fclose(file);
    }

    new File:file = fopen(s_file, io_append);
    fwrite(file, out);
    return fclose(file), 1;
}