#pragma once

class Log {
public:
    static void info(const char *pMessage, ...);

    static void debug(const char *pMessage, ...);

    static void warn(const char *pMessage, ...);

    static void error(const char *pMessage, ...);
};

#ifndef NDEBUG
#define droidblaster_Log_debug(...) Log::debug(__VA_ARGS__)
#else
#define droidblaster_Log_debug(...)
#endif