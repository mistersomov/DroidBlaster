#include <cstdarg>
#include <android/log.h>
#include "Log.h"

void Log::info(const char *pMessage, ...) {
    va_list args;
    va_start(args, pMessage);
    __android_log_vprint(ANDROID_LOG_INFO, "DroidBlaster", pMessage, args);
    va_end(args);
}

void Log::debug(const char *pMessage, ...) {
    va_list args;
    va_start(args, pMessage);
    __android_log_vprint(ANDROID_LOG_DEBUG, "DroidBlaster", pMessage, args);
    va_end(args);
}

void Log::warn(const char *pMessage, ...) {
    va_list args;
    va_start(args, pMessage);
    __android_log_vprint(ANDROID_LOG_WARN, "DroidBlaster", pMessage, args);
    va_end(args);
}

void Log::error(const char *pMessage, ...) {
    va_list args;
    va_start(args, pMessage);
    __android_log_vprint(ANDROID_LOG_ERROR, "DroidBlaster", pMessage, args);
    va_end(args);
}